#include "imgui_config.h"
#include <imgui/imgui_internal.h>

bool ImGuiEx::Combo(const char* label, ConfigVar<int>& currentItem, const char* const items[], int itemsCount)
{
	int value = currentItem.GetValue();
	if (ImGui::Combo(label, &value, items, itemsCount)) {
		currentItem = value;
		return true;
	}
	return false;
}

bool ImGuiEx::Checkbox(const char* label, ConfigVar<bool>& var, const char* tooltipText)
{
	bool value = var.GetValue();
	if (ImGui::Checkbox(label, &value, tooltipText)) {
		var = value;
		return true;
	}
	return false;
}

bool ImGuiEx::SliderFloat(const char* label, ConfigVar<float>& var, float min, float max)
{
	float value = var.GetValue();
	if (ImGui::SliderFloat(label, &value, min, max)) {
		var = value;
		return true;
	}
	return false;
}

bool ImGuiEx::SliderFloat2(const char* label, ConfigVar<Vector2>& var, float min, float max)
{
	Vector2 value = var.GetValue();
	float vec[3] = { value.x, value.y };

	if (ImGui::SliderFloat2(label, vec, min, max)) {
		var = Vector2{ vec[0], vec[1] };
		return true;
	}

	return false;
}

bool ImGuiEx::SliderInt(const char* label, ConfigVar<int>& var, int min, int max, const char* format)
{
	int value = var.GetValue();
	if (ImGui::SliderInt(label, &value, min, max, format)) {
		var = value;
		return true;
	}
	return false;
}

bool ImGuiEx::SliderFloat3(const char* label, ConfigVar<Vector3>& var, float min, float max)
{
	Vector3 value = var.GetValue(); // Fetch current value
	float vec[3] = { value.x, value.y, value.z };

	if (ImGui::SliderFloat3(label, vec, min, max)) {
		var = Vector3{ vec[0], vec[1], vec[2] }; // Update config variable
		return true;
	}

	return false;
}

bool ImGuiEx::ColorEdit4(const char* label, ConfigVar<ImColor>& var, bool showMainLabel)
{
	if (!showMainLabel) {
		ImGui::SameLine();
		ImVec2 min = ImGui::GetCursorPos();
		ImVec2 avail = ImGui::GetContentRegionAvail();
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGui::SetCursorPosX(min.x + avail.x - ImGui::GetFrameHeight() * 2 - style.ItemSpacing.x - style.WindowPadding.x - (ImGui::GetCurrentWindow()->ScrollbarY ? style.ScrollbarSize : 0.f));
	}

	float* value = &var.GetValue().Value.x;

	if (ImGui::ColorEdit4(label, value, ImGuiColorEditFlags_NoInputs | (showMainLabel ? ImGuiColorEditFlags_None : ImGuiColorEditFlags_NoLabel))) {
		var = ImColor(value[0], value[1], value[2], value[3]);
		return true;
	}
	return false;
}

bool ImGuiEx::ColorEdit4(const char* label, ConfigVar<ImVec4>& var, bool showMainLabel)
{
	if (!showMainLabel) {
		ImGui::SameLine();
		ImVec2 min = ImGui::GetCursorPos();
		ImVec2 avail = ImGui::GetContentRegionAvail();
		const ImGuiStyle& style = ImGui::GetStyle();
		ImGui::SetCursorPosX(min.x + avail.x - ImGui::GetFrameHeight() * 2 - style.ItemSpacing.x - style.WindowPadding.x - (ImGui::GetCurrentWindow()->ScrollbarY ? style.ScrollbarSize : 0.f));
	}

	ImVec4 color = var.GetValue();
	float value[4] = { color.x, color.y, color.z, color.w };
	if (ImGui::ColorEdit4(label, value, ImGuiColorEditFlags_NoInputs | (showMainLabel ? ImGuiColorEditFlags_None : ImGuiColorEditFlags_NoLabel))) {
		var = ImVec4{ value[0], value[1], value[2], value[3] };
		return true;
	}

	return false;
}

