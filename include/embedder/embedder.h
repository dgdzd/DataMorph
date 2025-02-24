#ifndef EMBEDDER_H
#define EMBEDDER_H

#include <string>
#include <vector>
#include <fstream>

struct embed_context {
	std::string file_name;
	std::string file_extension;
	std::string file_data;
};

namespace Embed {
	void embed_file(const std::string& file_path, std::string& output) {
		std::string file_name = file_path.substr(file_path.find_last_of("\\") + 1);
		std::string file_extension = file_name.substr(file_name.find_last_of(".") + 1);
		unsigned char* file_data;
		int file_size;
		std::ifstream file(file_path, std::ios::binary);
		if (file.is_open()) {
			file.seekg(0, std::ios::end);
			file_size = file.tellg();
			file_data = new unsigned char[file_size];
			file.seekg(0, std::ios::beg);
			file.read((char*)file_data, file_size);
			file.close();
		}
		std::ofstream output_file("resources\\" + file_name + ".h");
		if (output_file.is_open()) {
			output_file << "#ifndef " << file_name << "_H\n";
			output_file << "#define " << file_name << "_H\n\n";
			output_file << "#include <string>\n\n";
			output_file << "namespace " << file_name << " {\n";
			output_file << "\tconst std::string data = \"";
			for (int i = 0; i < file_size; i++) {
				char c = file_data[i];
				output_file << "\\x" << std::hex << (int)c;
			}
			output_file << "\";\n";
			output_file << "}\n\n";
			output_file << "#endif\n";
			output_file.close();
		}
	}
}

#endif