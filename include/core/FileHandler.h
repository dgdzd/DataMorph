#ifndef DM_FILEHANDLER_H
#define DM_FILEHANDLER_H

#include <fstream>
#include <string>
#include <vector>

namespace files {
	static std::string options_file = "options.opt";
	static std::string options_data = "";

	extern bool read_csv_headers(const std::string& path, std::vector<std::string>& data, int& size, std::string& filename);
	extern bool read_csv_values(const std::string& path, std::vector<std::vector<double>>& data, int& size, std::string& filename);

	extern std::string get_option(const std::string& option);
	extern void set_option(const std::string& option, const std::string& value);

	extern char* get_content(const std::string& filepath);
	extern char8_t* get_u8content(const std::string& filepath);
}

#endif