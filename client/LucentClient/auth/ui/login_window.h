#pragma once

#include "../../imgui/imgui.h"
#include <string>

class LoginWindow {
public:
	inline static int expiry;

	static void Init();
	static bool Draw();
private:
	static char key[30];
	static bool versionAvailable;
	static bool initiated;
	static ImVec2 windowSize;
	static ImVec2 windowPos;

	static bool LoadKeyFromFile();
	static void SaveKeyToFile(const std::string& key);
};
