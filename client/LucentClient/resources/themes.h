#pragma once
#include "../imgui/imgui.h"

struct MenuTheme {
	const char* name;

	ImVec4 bgDark;
	ImVec4 bgMed;
	ImVec4 accent;
	ImVec4 text;
};

extern MenuTheme allThemes[150];
extern void LoadImGuiStyle(const MenuTheme& theme);