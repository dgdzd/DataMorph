#include <core/Project.h>
#include <iostream>

Project::Project(std::string name, std::string path) {
	 this->name = name;
	 this->path = path;
	 this->units = {};
	 this->symbols = {};
	 this->values = {};
}

void Project::initValues() {
	for (int i = 0; i < this->symbols.size(); i++) {
		this->headers[this->symbols[i]] = Header(this, this->symbols[i], this->units[i], { 0.0 });
	}
}

void Project::addRow() {
	for (int i = 0; i < this->symbols.size(); i++) {
		this->headers[this->symbols[i]].values.push_back(0.0);
	}
}

void Project::removeRow() {
	for (int i = 0; i < this->symbols.size(); i++) {
		this->headers[this->symbols[i]].values.pop_back();
	}
}

void Project::addColumn(Header* header) {
	const char* expr = header->expression;
	if (expr[0] == '\0') {
		this->headers[header->name] = *header;
		this->headers[header->name].values = std::vector<double>(this->headers[this->symbols[0]].values.size(), 0.0);
	}
	else {
		std::string name = header->name;
		this->headers[name] = *header;
		this->headers[name].values = std::vector<double>(this->headers[this->symbols[0]].values.size(), 0.0);
		this->headers[name].addVars();
		this->headers[name].compileExpression();
		this->headers[name].updateValues();
	}
}

void Project::removeColumn(std::string name) {
	auto it = this->headers.find(name);
	if (it == this->headers.end()) {
		return;
	}
	std::cout << "Removing column " << (*it).first << std::endl;
	this->headers.erase(it);
	auto ne = std::remove(this->symbols.begin(), this->symbols.end(), name);
	auto d = std::distance(this->symbols.begin(), ne);
	auto ne2 = std::remove(this->units.begin(), this->units.end(), this->units[d]);

	this->symbols.erase(ne, this->symbols.end());
	this->units.erase(ne2, this->units.end());

}