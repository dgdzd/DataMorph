#ifndef DM_SETTINGS_H
#define DM_SETTINGS_H

#include <string>
#include <map>
#include <imgui.h>

struct Settings {
	static std::string options_file;
	static Settings* instance;
	std::map<std::string, std::string> data_strings;
	std::map<std::string, int> data_ints;
	std::map<std::string, float> data_floats;
	std::map<std::string, double> data_doubles;
	std::map<std::string, ImVec4> data_vecs;

	Settings(const std::string& file);

	int& get_int(const std::string& option);
	float& get_float(const std::string& option);
	double& get_double(const std::string& option);
	std::string& get_string(const std::string& option);
	ImVec4& get_vec(const std::string& option);

	void set_int(const std::string& option, int value);
	void set_float(const std::string& option, float value);
	void set_double(const std::string& option, double value);
	void set_string(const std::string& option, const std::string& value);
	void set_vec(const std::string& option, const ImVec4& value);

	void init_defaults();
	void load_options();
	void write_options();
};

#endif