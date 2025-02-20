#include <FontManager.h>

FontManager* FontManager::inst;

FontManager::FontManager() {
	this->fonts = std::map<const char*, ImFont*>();
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
	ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, size, 0, ImGui::GetIO().Fonts->GetGlyphRangesGreek());
	this->fonts.insert(std::pair<const char*, ImFont*>(name, font));
	return font;
}

ImFont* FontManager::getFont(const char* name) {
	return this->fonts[name];
}