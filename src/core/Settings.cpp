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

std::string Settings::get_option(const std::string& option) {
	return options_data[option];
}

void Settings::set_option(const std::string& option, const std::string& value) {
	options_data[option] = value;
}

void Settings::load_options() {
	std::ifstream file(options_file);
	options_data["Theme"] = "Dark";
	options_data["Graphs colormap"] = "1";
	options_data["Python executable path"] = "resources\\Python\\python3.exe";
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			std::vector<std::string> parts = std::split(line, '=', 1);
			options_data[parts[0]] = parts[1];
		}
		file.close();
	}
	else {
		
	}
}

void Settings::write_options() {
	std::ofstream file(options_file);
	if (file.is_open()) {
		for (auto& [key, value] : options_data) {
			file << key << "=" << value << "\n";
		}
		file.close();
	}
}