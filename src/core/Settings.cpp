#include <core/Settings.h>

#include <core/FileHandler.h>
#include <fstream>
#include <iostream>
#include <Utils.h>

std::string Settings::options_file = "options.opt";
Settings* Settings::instance = nullptr;

Settings::Settings(const std::string& file) {
	options_file = file;
	load_options();
}

bool& Settings::get_bool(const std::string& option) {
	return data_bools[option];
}

int& Settings::get_int(const std::string& option) {
	return data_ints[option];
}

float& Settings::get_float(const std::string& option) {
	return data_floats[option];
}

double& Settings::get_double(const std::string& option) {
	return data_doubles[option];
}

std::string& Settings::get_string(const std::string& option) {
	return data_strings[option];
}

ImVec4& Settings::get_vec(const std::string& option) {
	return data_vecs[option];
}

void Settings::set_bool(const std::string& option, bool value) {
	data_bools[option] = value;
}

void Settings::set_int(const std::string& option, int value) {
	data_ints[option] = value;
}

void Settings::set_float(const std::string& option, float value) {
	data_floats[option] = value;
}

void Settings::set_double(const std::string& option, double value) {
	data_doubles[option] = value;
}

void Settings::set_string(const std::string& option, const std::string& value) {
	data_strings[option] = value;
}

void Settings::set_vec(const std::string& option, const ImVec4& value) {
	data_vecs[option] = value;
}

void Settings::init_defaults() {
	set_int("lang", 0);
	set_bool("autocheck_updates", 1);

	set_string("theme", "dark");
	set_int("graphs_cmap", 1);
	set_int("default_marker", 1);

	set_string("python_exec_path", "resources\\Python\\python3.exe");
	set_vec("py_keyw_highlight_c", ImVec4(197.0f/255, 134.0f/255, 192.0f/255, 1.0f));
	set_vec("py_comment_c", ImVec4(56.0f/255, 56.0f/255, 56.0f/255, 1.0f));
	set_vec("py_str_c", ImVec4(212.0f/255, 157.0f/255, 133.0f/255, 1.0f));
	set_vec("py_func_def_c", ImVec4(45.0f/255, 122.0f/255, 214.0f/255, 1.0f));
}

void Settings::load_options() {
	init_defaults();
	std::ifstream file(options_file);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::vector<std::string> parts = std::split(line, '=', 1);
			std::vector<std::string> name_part = std::split(parts[0], ':', 1);

			if (name_part[1] == "bol") {
				data_bools[name_part[0]] = std::stoi(parts[1]);
			}
			if (name_part[1] == "int") {
				data_ints[name_part[0]] = std::stoi(parts[1]);
			}
			else if (name_part[1] == "flt") {
				data_floats[name_part[0]] = std::stof(parts[1]);
			}
			else if (name_part[1] == "dbl") {
				data_doubles[name_part[0]] = std::stod(parts[1]);
			}
			else if (name_part[1] == "str") {
				data_strings[name_part[0]] = parts[1];
			}
			else if (name_part[1] == "vec") {
				std::vector<std::string> comps = std::split(parts[1], ',', 3);
				ImVec4 vec;
				vec.x = std::stof(comps[0]);
				vec.y = std::stof(comps[1]);
				vec.z = std::stof(comps[2]);
				vec.w = std::stof(comps[3]);
				data_vecs[name_part[0]] = vec;
			}
		}
		file.close();
	}
	else {
		
	}
}

void Settings::write_options() {
	std::ofstream file(options_file);
	if (file.is_open()) {
		for (auto& [key, value] : data_bools) {
			file << key << ":bol=" << value << "\n";
		}
		for (auto& [key, value] : data_ints) {
			file << key << ":int=" << value << "\n";
		}
		for (auto& [key, value] : data_floats) {
			file << key << ":flt=" << value << "\n";
		}
		for (auto& [key, value] : data_doubles) {
			file << key << ":dbl=" << value << "\n";
		}
		for (auto& [key, value] : data_strings) {
			file << key << ":str=" << value << "\n";
		}
		for (auto& [key, value] : data_vecs) {
			file << key << ":vec=" << value.x << "," << value.y << "," << value.z << "," << value.w << "\n";
		}
		file.close();
	}
}