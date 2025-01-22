#include <imgui.h>
#include <core/imgui_extension.h>

void ImGui::CenteredText(const char* text, ...) {
	ImVec2 textSize = CalcTextSize(text);
	ImVec2 windowSize = GetWindowSize();
	ImVec2 windowPos = GetWindowPos();

	float textPos = (windowSize.x - textSize.x) / 2;
	SetCursorPosX(textPos);
	Text(text);
}

void ImGui::CenteredText(std::string text, ...) {
	ImVec2 textSize = CalcTextSize(text.c_str());
	ImVec2 windowSize = GetWindowSize();
	ImVec2 windowPos = GetWindowPos();
	float textPos = (windowSize.x - textSize.x) / 2;
	SetCursorPosX(textPos);
	Text(text.c_str());
}