#ifndef DM_FILEHANDLER_H
#define DM_FILEHANDLER_H

#include <string>
#include <vector>
#include <fstream>

namespace FileHandler {
	bool read_csv_headers(const std::string& path, std::vector<std::string>& data, int& size, std::string& filename);
	bool read_csv_values(const std::string& path, std::vector<std::vector<double>>& data, int& size, std::string& filename);
}

#endif