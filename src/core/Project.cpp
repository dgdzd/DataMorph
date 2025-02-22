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
		Header h = Header(this, this->symbols[i], this->units[i], { 0.0 });
		this->headers[h.id] = h;
	}
}

void Project::addRow() {
	for (int i = 0; i < this->ids.size(); i++) {
		this->headers[this->ids[i]].values.push_back(0.0);
	}
}

void Project::removeRow() {
	for (int i = 0; i < this->ids.size(); i++) {
		this->headers[this->ids[i]].values.pop_back();
	}
}

void Project::addColumn(Header* header) {
	Expression* expr = header->expression;
	unsigned int id = header->id;
	this->headers[id] = *header;
	if (this->headers.size() != 0) {
		this->headers[id].values = std::vector<double>(this->headers[this->ids[0]].values.size(), 0.0);
	}
	else {
		this->headers[id].values = { 0.0 };
	}
	if (expr->specs.type != DUMMY) {
		this->headers[id].expression->parent = &this->headers[id];
		this->headers[id].expression->addVars();
		this->headers[id].expression->compileExpression();
		this->headers[id].expression->updateValues();
	}
}

void Project::removeColumn(unsigned int id) {
	auto it = this->headers.find(id);
	if (it == this->headers.end()) {
		return;
	}
	this->headers.erase(it);
	auto ne = std::find(this->symbols.begin(), this->symbols.end(), id);
	auto d = std::distance(this->symbols.begin(), ne);
	auto ne2 = std::find(this->units.begin(), this->units.end(), this->units[d]);

	this->symbols.erase(ne, ne+1);
	this->units.erase(ne2, ne2+1);

}