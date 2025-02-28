#include <core/Console.h>

Console::Console(const std::string& name, std::ostream& to_replace) : to_replace(to_replace) {
	this->name = name;
	this->old_buf = nullptr;
}

void Console::use() {
	this->old_buf = std::cout.rdbuf(redirect_stream.rdbuf());
}

void Console::unuse() {
	std::cout.rdbuf(this->old_buf);
}