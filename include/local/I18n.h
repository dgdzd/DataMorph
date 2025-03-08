#ifndef DM_TRANSLATOR_H
#define DM_TRANSLATOR_H

#include <map>
#include <string>

enum Lang : int {
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

extern std::string LangToString(Lang lang);
extern std::string LangToString(int lang);

class I18n {
	std::map<std::string, std::string> translations;

public:
	Lang lang;
	I18n(Lang lang = en_us);
	const std::string& t(const std::string& key);
};

#endif