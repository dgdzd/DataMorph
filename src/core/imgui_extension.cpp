#include <core/imgui_extension.h>

#include <FontManager.h>
#include <imgui.h>

void ImGui::CenteredText(const char* fmt, ...) {
	ImVec2 textSize = CalcTextSize(fmt);
	ImVec2 windowSize = GetWindowSize();
	ImVec2 windowPos = GetWindowPos();
	float textPos = (windowSize.x - textSize.x) / 2;
	SetCursorPosX(textPos);

	va_list args;
	va_start(args, fmt);
	Text(fmt, args);
	va_end(args);
}

void ImGui::CenteredText(std::string fmt, ...) {
	ImVec2 textSize = CalcTextSize(fmt.c_str());
	ImVec2 windowSize = GetWindowSize();
	ImVec2 windowPos = GetWindowPos();
	float textPos = (windowSize.x - textSize.x) / 2;
	SetCursorPosX(textPos);

	va_list args;
	va_start(args, fmt);
	Text(fmt.c_str(), args);
	va_end(args);
}

void ImGui::HeaderText(const char* fmt, const char* fontname, float scale ...) {
	PushFont(FontManager::getInstance()->getFont(fontname));
	va_list args;
	va_start(args, fmt);
	SetWindowFontScale(scale);
	Text(fmt, args);
	SetWindowFontScale(1.0f);
	va_end(args);
	PopFont();
}

void ImGui::HeaderText(std::string fmt, const char* fontname, float scale, ...) {
	PushFont(FontManager::getInstance()->getFont(fontname));
	va_list args;
	va_start(args, fmt);
	SetWindowFontScale(scale);
	Text(fmt.c_str(), args);
	SetWindowFontScale(1.0f);
	va_end(args);
	PopFont();
}