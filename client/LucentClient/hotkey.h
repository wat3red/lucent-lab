#include <config/config_var.h>
#include <imgui/imgui.h>

#define IS_KEY_PRESSED(key) (GetAsyncKeyState(key) & 0x8000)

namespace ImGuiEx {
	void Hotkey(const char* label, ConfigVar<int>& hotkey);
}
