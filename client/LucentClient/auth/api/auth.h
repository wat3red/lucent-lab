#pragma once

#include <string>
#include <curl/curl.h>

class DatabaseAuth {
public:
	DatabaseAuth();
	~DatabaseAuth();

	std::string SendLoginRequest(const std::string& key, const std::string& hwid);
	bool LoginUser(const std::string& key, const std::string& hwid);

private:
	CURL* curl;
};
