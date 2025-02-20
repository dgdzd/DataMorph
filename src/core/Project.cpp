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
	Expression* expr = header->expression;
	std::string name = header->name;
	this->headers[name] = *header;
	if (this->headers.size() != 0) {
		this->headers[name].values = std::vector<double>(this->headers[this->symbols[0]].values.size(), 0.0);
	}
	else {
		this->headers[name].values = { 0.0 };
	}
	if (expr->specs.type != DUMMY) {
		this->headers[name].expression->parent = &this->headers[name];
		this->headers[name].expression->addVars();
		this->headers[name].expression->compileExpression();
		this->headers[name].expression->updateValues();
	}
}

void Project::removeColumn(std::string name) {
	auto it = this->headers.find(name);
	if (it == this->headers.end()) {
		return;
	}
	this->headers.erase(it);
	auto ne = std::find(this->symbols.begin(), this->symbols.end(), name);
	auto d = std::distance(this->symbols.begin(), ne);
	auto ne2 = std::find(this->units.begin(), this->units.end(), this->units[d]);

	this->symbols.erase(ne, ne+1);
	this->units.erase(ne2, ne2+1);

}