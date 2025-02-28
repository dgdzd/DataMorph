#ifndef DM_FONTMANAGER_H
#define DM_FONTMANAGER_H

#include <imgui/imgui.h>
#include <map>

class FontManager {
	static FontManager* inst;
	std::map<const char*, ImFont*> fonts;
	ImVector<ImWchar> ranges;

	FontManager();

public:
	static FontManager* getInstance();

	ImFont* loadFontFromPath(const char* path, float size, const char* name = "");
	ImFont* getFont(const char* name);
};

#endif