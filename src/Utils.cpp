#include <Utils.h>

std::string std::trim_trailing(const std::string& str, const char c) {
	if (str[0] == c) {
		return trim_trailing(str.substr(1), c);
	}
	else {
		return str;
	}
}

std::string std::trim_leading(const std::string& str, const char c) {
	if (str[str.size() - 1] == c) {
		return trim_leading(str.substr(0, str.size() - 2), c);
	}
	else {
		return str;
	}
}

std::string std::trim(const std::string& str, const char c) {
	return trim_leading(trim_trailing(str, c), c);
}