#include <local/I18n.h>

#include <core/FileHandler.h>
#include <Utils.h>

std::string LangToString(Lang lang) {
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
	}
}

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
	}
}

I18n::I18n(Lang lang) {
	this->lang = lang;

	std::string content = files::get_content("resources\\local\\" + LangToString(lang) + ".i18n");
	std::vector<std::string> lines = std::split(content, '\n');

	for (const std::string& line : lines) {
		std::vector<std::string> args = std::split(line, '=');
		if (args.size() > 1) {
			translations[args[0]] = args[1];
		}
	}
}

const std::string& I18n::t(const std::string& key) {
	if (translations.contains(key)) {
		return translations[key];
	}
	else {
		return key;
	}
}