#include <FontManager.h>

#include <iostream>

FontManager* FontManager::inst;

FontManager::FontManager() {
	this->fonts = std::map<const char*, ImFont*>();
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesGreek());
	builder.AddChar(0x0305); // Combining overline char
	builder.BuildRanges(&ranges);
}

FontManager* FontManager::getInstance() {
	if (!inst) {
		inst = new FontManager();
	}

	return inst;
}

ImFont* FontManager::loadFontFromPath(const char* path, float size, const char* name) {
	if (name == "") {
		name = path;
	}
	ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, size, 0, this->ranges.Data);
	this->fonts.insert(std::pair<const char*, ImFont*>(name, font));
	return font;
}

ImFont* FontManager::getFont(const char* name) {
	return this->fonts[name];
}