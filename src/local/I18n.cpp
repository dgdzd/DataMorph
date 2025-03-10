#include <local/I18n.h>

#include <core/FileHandler.h>
#include <iostream>
#include <Utils.h>

std::string LangToString(int lang) {
	switch (lang) {
	case en_us:
		return "English (US)";
	case en_uk:
		return "English (UK)";
	case fr_fr:
		return (const char*)u8"Français (France)";
	case fr_ca:
		return (const char*)u8"Français (Canada)";
	case de:
		return "Deutsch";
	case es:
		return (const char*)u8"Español";
	case ru:
		return (const char*)u8"Русский";
	case uk:
		return (const char*)u8"українська";
	default:
		return "None";
	}
}

std::string LangToCode(int lang) {
	switch (lang) {
	case en_us:
		return "en_us";
	case en_uk:
		return "en_uk";
	case fr_fr:
		return "fr_fr";
	case fr_ca:
		return "fr_ca";
	case de:
		return "de";
	case es:
		return "es";
	case ru:
		return "ru";
	case uk:
		return "uk";
	default:
		return "None";
	}
}

I18n::I18n(int lang, Lang fallback) {
	this->lang = lang;
	this->fallback = nullptr;
	if (fallback != None && lang != fallback) {
		this->fallback = new I18n(fallback, None);
	}

	char* content = files::get_content("resources\\local\\" + LangToCode(lang) + ".i18n");
	std::vector<std::string> lines = std::split(content, '\n');

	for (const std::string& line : lines) {
		std::vector<std::string> args = std::split(line, '=', 1);
		if (args.size() > 1) {
			translations[args[0]] = args[1];
		}
	}
}

const char* I18n::t(const std::string& key, const std::string& id) {
	std::string append = id.empty() ? "" : "###" + id;
	if (translations.contains(key)) {
		std::string s = translations[key] + append;
		char* result = new char[s.size()+1];
		strcpy(result, s.c_str());
		return result;
	}
	else if (fallback) {
		return fallback->t(key, id);
	}
	else {
		char* result = new char[key.size()+1];
		result[key.size()] = '\0';
		strcpy(result, key.c_str());
		return result;
	}
}