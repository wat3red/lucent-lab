#pragma once

#include "config_var.h"
#include "../vector3.h"
#include "../vector2.h"

namespace ImGuiEx {
	bool Combo(const char* label, ConfigVar<int>& currentItem, const char* const items[], int itemsCount);

	bool Checkbox(const char* label, ConfigVar<bool>& var, const char* tooltipText = nullptr);

	bool SliderFloat(const char* label, ConfigVar<float>& var, float min, float max);

	bool SliderFloat2(const char* label, ConfigVar<Vector2>& var, float min, float max);

	bool SliderInt(const char* label, ConfigVar<int>& var, int min, int max, const char* format = oxorany("%d"));

	template<typename T>
	bool InputText(const char* label, ConfigVar<T>& var, size_t buf_size = 256) {
		char buffer[256];
		strncpy(buffer, var.GetValue().c_str(), buf_size);
		if (ImGui::InputText(label, buffer, buf_size)) {
			var = buffer;
			return true;
		}
		return false;
	}

	bool SliderFloat3(const char* label, ConfigVar<Vector3>& var, float min, float max);

	bool ColorEdit4(const char* label, ConfigVar<ImColor>& var, bool showMainLabel = false);

	bool ColorEdit4(const char* label, ConfigVar<ImVec4>& var, bool showMainLabel = false);
}