#ifndef DM_CONSOLE_H
#define DM_CONSOLE_H

#include <iostream>
#include <sstream>
#include <string>

class Console {
	std::streambuf* old_buf;

public:
	std::string name;
	std::ostream& to_replace;
	std::stringstream redirect_stream;
	std::string content;

	Console(const std::string& name, std::ostream& to_replace);

	void use();
	void unuse();
	std::string& update_content();
};

#endif