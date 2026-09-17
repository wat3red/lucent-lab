#include "login_window.h"
#include "../utils/error_manager.h"
#include "../api/auth.h"
#include "../hwid/hwid.h"

#include <fstream>
#include <format>
#include <iostream>
#include <oxorany/oxorany_include.h>

char LoginWindow::key[30];
bool LoginWindow::versionAvailable;
bool LoginWindow::initiated;
ImVec2 LoginWindow::windowSize;
ImVec2 LoginWindow::windowPos;

void LoginWindow::Init() {
	versionAvailable = true;
	initiated = false;
	windowSize = { 300, 200 };
	windowPos = { 1920 / 2 - windowSize.x / 2, 1080 / 2 - windowSize.y / 2 };
	expiry = 0;
	memset(key, 0, sizeof(key));
}

std::string GetEnvVariable(const std::string_view& varname) {
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, varname.data()) != 0 || buf == nullptr) {
		return "";
	}
	std::string result(buf);
	free(buf);
	return result;
}

bool LoginWindow::LoadKeyFromFile() {
	std::string licenseFileName = GetEnvVariable(oxorany("USERPROFILE")) + std::string(oxorany("\\Lucent\\license.key"));
	std::ifstream file(licenseFileName);

	if (file.is_open()) {
		std::string savedKey;
		file >> savedKey;
		file.close();
		strcpy_s(key, sizeof(key), savedKey.c_str());
		return true;
	}
	return false;
}

void LoginWindow::SaveKeyToFile(const std::string& key) {
	std::string licenseFileName = GetEnvVariable(oxorany("USERPROFILE")) + std::string(oxorany("\\Lucent\\license.key"));
	std::ofstream file(licenseFileName);

	if (file.is_open()) {
		file << key;
		file.close();
	}
}

bool LoginWindow::Draw() {
	static DatabaseAuth databaseAuth;
	if (!initiated) {
		LoadKeyFromFile();

		initiated = true;
	}

	if (strcmp(ErrorManager::GetError(), "") != 0) {
		ErrorManager::ErrorPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10, 10 });

	ImGui::SetNextWindowPos(ImVec2(windowPos.x, windowPos.y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(windowSize.x, windowSize.y));
	ImGui::Begin(oxorany("Input key"),
		nullptr,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration);

	ImGui::PopStyleVar();

	float widgetWidth = ImGui::GetWindowSize().x - 20;
	ImGui::SetNextItemWidth(widgetWidth);

	ImGui::InputTextWithHint(oxorany("##key"), oxorany("Key"), key, sizeof(key));

	if (ImGui::Button(oxorany("Login"), ImVec2(widgetWidth, 0))) {
		std::string keyString(key);
		std::string hwid = GetHardwareID();

		if (databaseAuth.LoginUser(keyString, hwid)) {
			SaveKeyToFile(keyString);
			ImGui::End();
			return true;
		}
		//else {
			//ErrorManager::SetError(oxorany("Subscription is not valid"));
		//}
	}


	if (ImGui::Button(oxorany("Buy license"), ImVec2(widgetWidth, 0))) {
		//ShellExecute(NULL, obfuscate(oxorany("open")), obfuscate(oxorany("https://t.me/divine_cheats")), NULL, NULL, SW_SHOWDEFAULT);
	}

	ImGui::End();
	return false;
}