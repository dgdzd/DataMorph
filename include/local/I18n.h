#ifndef DM_TRANSLATOR_H
#define DM_TRANSLATOR_H

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
	const char* t(const std::string& key);
};

#endif