#ifndef DM_SETTINGS_H
#define DM_SETTINGS_H

#include <string>
#include <map>

struct Settings {
	static std::string options_file;
	static Settings* instance;
	std::map<std::string, std::string> options_data;

	Settings(const std::string& file);

	std::string get_option(const std::string& option);
	void set_option(const std::string& option, const std::string& value);

	void load_options();
	void write_options();
};

#endif