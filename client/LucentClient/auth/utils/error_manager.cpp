#include <imgui/imgui.h>
#include <oxorany/oxorany_include.h>

#include "error_manager.h"

const char* ErrorManager::error = "";

void ErrorManager::SetError(const char* errMsg) {
	error = errMsg;
}

const char* ErrorManager::GetError() {
	return error;
}

void ErrorManager::ErrorPopup() {
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowFocus();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 10, 10 });

	if (ImGui::Begin(oxorany("Error"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(oxorany("%s"), error);
		if (ImGui::Button(oxorany("OK"), ImVec2(ImGui::GetWindowSize().x - 20, 25))) {
			error = "";
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}