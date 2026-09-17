#include "auth.h"
#include "../../config/json.hpp"
#include "../utils/error_manager.h"
#include "../ui/login_window.h"
#include "../hwid/hwid.h"
#include "sha256/sha256.h"
#include "base64.h" 

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <oxorany/oxorany_include.h>

// OpenSSL for cryptography
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/kdf.h>

using json = nlohmann::json;

// --- CONFIGURATION ---
// Live credentials live in auth_secrets.h (gitignored). See auth_secrets.example.h.
// The previous hardcoded key/salt/endpoint must be treated as burned — rotate before any use.
#include "auth_secrets.h"
#ifndef VERIFY_LINK
#define VERIFY_LINK "https://example.invalid/verify.php"
#endif
#ifndef MASTER_KEY
#define MASTER_KEY "REPLACE_WITH_ROTATED_KEY"
#endif
#ifndef PBKDF2_SALT
#define PBKDF2_SALT "REPLACE_WITH_ROTATED_SALT"
#endif
#define PBKDF2_ITERATIONS 10000

// --- CRYPTO HELPER FUNCTIONS ---

// Derives a key using PBKDF2-HMAC-SHA256
std::vector<unsigned char> derive_key() {
	std::vector<unsigned char> derived_key(32); // AES-256 key size
	std::string master = oxorany(MASTER_KEY);
	std::string salt = oxorany(PBKDF2_SALT);

	PKCS5_PBKDF2_HMAC(
		master.c_str(), master.length(),
		(const unsigned char*)salt.c_str(), salt.length(),
		PBKDF2_ITERATIONS,
		EVP_sha256(),
		derived_key.size(),
		derived_key.data()
	);
	return derived_key;
}

// Encrypts using AES-256-GCM
bool encrypt_gcm(const std::string& plaintext, const std::vector<unsigned char>& key,
	std::vector<unsigned char>& iv, std::vector<unsigned char>& ciphertext, std::vector<unsigned char>& tag) {
	iv.resize(12); // GCM recommended IV size
	if (RAND_bytes(iv.data(), iv.size()) != 1) {
		return false;
	}

	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (!ctx) return false;

	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) { EVP_CIPHER_CTX_free(ctx); return false; }
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), NULL)) { EVP_CIPHER_CTX_free(ctx); return false; }
	if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key.data(), iv.data())) { EVP_CIPHER_CTX_free(ctx); return false; }

	ciphertext.resize(plaintext.length() + 16); // Make room for potential block padding
	int len;
	if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (const unsigned char*)plaintext.c_str(), plaintext.length())) {
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}
	int ciphertext_len = len;

	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}
	ciphertext_len += len;
	ciphertext.resize(ciphertext_len);

	tag.resize(16); // GCM tag size
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, tag.size(), tag.data())) {
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	EVP_CIPHER_CTX_free(ctx);
	return true;
}

// Decrypts using AES-256-GCM
bool decrypt_gcm(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key,
	const std::vector<unsigned char>& iv, const std::vector<unsigned char>& tag, std::string& decryptedtext) {
	EVP_CIPHER_CTX* ctx;
	int len;
	int plaintext_len;
	std::vector<unsigned char> plaintext(ciphertext.size());

	if (!(ctx = EVP_CIPHER_CTX_new())) return false;
	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) { EVP_CIPHER_CTX_free(ctx); return false; }
	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), NULL)) { EVP_CIPHER_CTX_free(ctx); return false; }
	if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key.data(), iv.data())) { EVP_CIPHER_CTX_free(ctx); return false; }

	if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) {
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}
	plaintext_len = len;

	if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, tag.size(), (void*)tag.data())) {
		EVP_CIPHER_CTX_free(ctx);
		return false;
	}

	int ret = EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
	EVP_CIPHER_CTX_free(ctx);

	if (ret > 0) {
		plaintext_len += len;
		plaintext.resize(plaintext_len);
		decryptedtext.assign(plaintext.begin(), plaintext.end());
		return true;
	}
	return false;
}

// --- MAIN AUTH LOGIC ---

DatabaseAuth::DatabaseAuth() : curl(curl_easy_init()) { }

DatabaseAuth::~DatabaseAuth() {
	curl_easy_cleanup(curl);
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string DatabaseAuth::SendLoginRequest(const std::string& key, const std::string& hwid) {
	if (!curl) return "";

	std::string readBuffer;

	// 1. Derive the encryption key
	auto encryption_key = derive_key();

	// 2. Prepare the plaintext payload
	std::string timestamp = std::to_string(std::time(nullptr));
	std::string hash = sha256(key + hwid + timestamp + oxorany(MASTER_KEY));
	json payload_json = {
		{oxorany("key"), key},
		{oxorany("hwid"), hwid},
		{oxorany("ts"), timestamp},
		{oxorany("hash"), hash}
	};
	std::string plaintext_payload = payload_json.dump();

	// 3. Encrypt the payload
	std::vector<unsigned char> iv, ciphertext, tag;
	if (!encrypt_gcm(plaintext_payload, encryption_key, iv, ciphertext, tag)) {
		ErrorManager::SetError(oxorany("Failed to encrypt request."));
		return "";
	}

	// 4. Create the final JSON to send to the server
	json request_json = {
		{oxorany("data"), Base64::Encode(std::string(ciphertext.begin(), ciphertext.end()))},
		{oxorany("iv"), Base64::Encode(std::string(iv.begin(), iv.end()))},
		{oxorany("tag"), Base64::Encode(std::string(tag.begin(), tag.end()))}
	};

	std::string postFields = request_json.dump();

	// 5. Send the request via cURL
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, oxorany("Content-Type: application/json"));
	headers = curl_slist_append(headers, oxorany("User-Agent: Lucent/1.0"));

	curl_easy_setopt(curl, CURLOPT_URL, oxorany(VERIFY_LINK));
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_perform(curl);
	curl_slist_free_all(headers);

	return readBuffer;
}

bool DatabaseAuth::LoginUser(const std::string& key, const std::string& hwid) {
	// Lab/portfolio default: offline auth. Remote auth only if you define
	// ENABLE_REMOTE_AUTH and supply auth_secrets.h after rotating credentials.
#if !defined(ENABLE_REMOTE_AUTH)
	(void)key;
	(void)hwid;
	LoginWindow::expiry = 0x7FFFFFFF;
	return true;
#else
	try {
		//printf(oxorany("[*] Sending login request...\n"));
		std::string response = SendLoginRequest(key, hwid);

		if (response.empty()) {
			ErrorManager::SetError(oxorany("Empty response from server."));
			return false;
		}

		json responseJson = json::parse(response);

		if (responseJson.contains(oxorany("error"))) {
			ErrorManager::SetError(responseJson[oxorany("error")].get<std::string>().c_str());
			return false;
		}

		if (!responseJson.contains(oxorany("data")) || !responseJson.contains(oxorany("iv")) || !responseJson.contains(oxorany("tag"))) {
			ErrorManager::SetError(oxorany("Malformed server response."));
			return false;
		}

		// 1. Decode the response from Base64
		std::string ciphertext_b64 = responseJson[oxorany("data")];
		std::string iv_b64 = responseJson[oxorany("iv")];
		std::string tag_b64 = responseJson[oxorany("tag")];

		auto _ciphertext = Base64::Decode(ciphertext_b64);
		auto _iv = Base64::Decode(iv_b64);
		auto _tag= Base64::Decode(tag_b64);

		std::vector<unsigned char> ciphertext(_ciphertext.begin(), _ciphertext.end());
		std::vector<unsigned char> iv(_iv.begin(), _iv.end());
		std::vector<unsigned char> tag(_tag.begin(), _tag.end());

		// 2. Derive the key (must be identical to encryption)
		auto encryption_key = derive_key();

		// 3. Decrypt the payload
		std::string decrypted_payload;
		if (!decrypt_gcm(ciphertext, encryption_key, iv, tag, decrypted_payload)) {
			ErrorManager::SetError(oxorany("Failed to authenticate server response."));
			return false;
		}

		//printf(oxorany("[*] Decrypted response: %s\n"), decrypted_payload.c_str());
		json decryptedJson = json::parse(decrypted_payload);

		if (!decryptedJson.value(oxorany("success"), false)) {
			std::string message = decryptedJson.value(oxorany("message"), oxorany("Unknown error."));
			ErrorManager::SetError(message.c_str());
			return false;
		}

		LoginWindow::expiry = decryptedJson.value(oxorany("expiry"), 0);
		std::time_t serverTime = decryptedJson.value(oxorany("server_time"), 0);

		// Optional: you can still verify time drift if desired
		//printf(oxorany("[*] Login successful. Expiry set.\n"));
		return true;

	}
	catch (const json::exception& e) {
		//printf(oxorany("[!] JSON parse error: %s\n"), e.what());
		ErrorManager::SetError(oxorany("Invalid format from server."));
		return false;
	}
	catch (const std::exception& e) {
		//printf(oxorany("[!] Exception during login: %s\n"), e.what());
		ErrorManager::SetError(oxorany("An unexpected error occurred."));
		return false;
	}
	return false;
#endif // ENABLE_REMOTE_AUTH
}