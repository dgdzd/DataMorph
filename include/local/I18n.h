#ifndef DM_TRANSLATOR_H
#define DM_TRANSLATOR_H

#include <format>
#include <map>
#include <string>

enum Lang : int {
	None = -1,
	en_us,
	en_uk,
	fr_fr,
	fr_ca,
	de,
	es,
	ru,
	uk,
};
const int LANG_COUNT = 8;

extern std::string LangToString(int lang);
extern std::string LangToCode(int lang);

class I18n {
	std::map<std::string, std::string> translations;
	I18n* fallback;

public:
	int lang;
	I18n(int lang = en_us, Lang fallback = en_us);
	const char* t(const std::string& key, const std::string& id = "");

	template <typename... Args>
	const char* tf(const std::string& key, std::string id, Args&&... args) {
		std::string append = id.empty() ? "" : "###" + id;
		if (translations.contains(key)) {
			std::string s = translations[key] + append;
			s = std::vformat(s, std::make_format_args(args...));
			char* result = new char[s.size()];
			strcpy(result, s.c_str());
			return result;
		}
		else if (fallback) {
			return fallback->tf(key, id, args...);
		}
		else {
			std::string s = key + append;
			char* result = new char[s.size()];
			strcpy(result, s.c_str());
			return result;
		}
	}
};

#endif