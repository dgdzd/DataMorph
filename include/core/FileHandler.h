#ifndef DM_FILEHANDLER_H
#define DM_FILEHANDLER_H

#include <fstream>
#include <string>
#include <vector>

namespace FileHandler {
	static std::string options_file = "options.opt";
	static std::string options_data = "";

	bool read_csv_headers(const std::string& path, std::vector<std::string>& data, int& size, std::string& filename);
	bool read_csv_values(const std::string& path, std::vector<std::vector<double>>& data, int& size, std::string& filename);

	std::string get_option(const std::string& option);
	void set_option(const std::string& option, const std::string& value);
	
}

#endif