#ifndef DM_IMGUIEXTENSION_H
#define DM_IMGUIEXTENSION_H

#include <string>

namespace ImGui {
	void CenteredText(const char* text, ...);
	void CenteredText(std::string text, ...);

}

#endif