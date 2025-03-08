#include <core/FileHandler.h>

#include <iostream>
#include <Utils.h>

namespace files {
	bool read_csv_headers(const std::string& path, std::vector<std::string>& data, int& size, std::string& filename) {
		filename = path.substr(path.find_last_of("\\") + 1);
		std::string file_extension = filename.substr(filename.find_last_of(".") + 1);
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (file.is_open()) {
			file.seekg(0, std::ios::end);
			size = file.tellg();
			file.seekg(0, std::ios::beg);
			std::string file_data = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

			int line_delimiter = file_data.find("\n");
			std::string line = file_data.substr(0, line_delimiter);
			data = std::split(line, ';');

			file.close();

			return true;
		}
		return false;
	}

	bool read_csv_values(const std::string& path, std::vector<std::vector<double>>& data, int& size, std::string& filename) {
		filename = path.substr(path.find_last_of("\\") + 1);
		std::string file_extension = filename.substr(filename.find_last_of(".") + 1);
		std::ifstream file(path);
		if (file.is_open()) {
			file.seekg(0, std::ios::end);
			size = file.tellg();
			file.seekg(0, std::ios::beg);
			std::string file_data = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

			std::string first_line = file_data.substr(0, file_data.find("\n"));

			int columns = std::count(first_line.begin(), first_line.end(), ';') + 1;
			file_data = file_data.substr(file_data.find("\n") + 1);
			std::vector<std::vector<std::string>> lines = {};
			for (std::string line : std::split(file_data, '\n')) {
				if (line.empty()) {
					continue;
				}
				lines.push_back(std::split(line, ';'));
			}

			data = std::vector<std::vector<double>>(columns, {});
			for (int i = 0; i < data.size(); i++) {
				for (int j = 0; j < lines.size(); j++) {
					data[i].push_back(std::stod(lines[j][i]));
				}
			}
			file.close();
			return true;
		}
		return false;
	}

	std::string get_option(const std::string& option) {
		if (options_data.empty()) {
			std::ifstream file(options_file);
			if (file.is_open()) {
				options_data = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
				file.close();
			}
		}
		std::vector<std::string> lines = std::split(options_data, '\n');
		for (std::string line : lines) {
			std::vector<std::string> parts = std::split(line, '=');
			if (parts[0] == option) {
				return parts[1];
			}
		}
		return "";
	}

	char* get_content(const std::string& filepath) {
		std::ifstream file(filepath, std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open()) {
			file.seekg(0, std::ios::end);
			int size = file.tellg();
			file.seekg(0, std::ios::beg);
			char* file_data = new char[size];
			file.read(file_data, size);

			return file_data;
		}
		return new char[] { "" };
	}
}