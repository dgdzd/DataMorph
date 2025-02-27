#ifndef DM_IMGUIEXTENSION_H
#define DM_IMGUIEXTENSION_H

#include <string>

namespace ImGui {
	void CenteredText(const char* text, ...);
	void CenteredText(std::string text, ...);
	void HeaderText(const char* text, const char* fontname, float scale, ...);
	void HeaderText(std::string text, const char* fontname, float scale, ...);
}

#endif