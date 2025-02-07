#include <core/Header.h>
#include <core/Project.h>

Header::Header() {
	this->name = "";
	this->unit = "";
	this->expression = "";
	this->values = {};
	this->symbol_map = {};
	this->parent = nullptr;
}

Header::Header(Project* parent, std::string name, std::string unit, std::vector<double> values, const char* expression) {
	this->parent = parent;
	this->name = name;
	this->unit = unit;
	this->expression = expression;
	this->values = values;
	this->symbol_map = {};
}

void Header::addVars() {
	if (this->expression[0] == '\0') {
		return;
	}
	this->symbol_map["i"] = 0;
	this->symbol_table.add_variable("i", this->symbol_map["i"]);
	for (int i = 0; i < parent->symbols.size(); i++) {
		std::string symbol = parent->symbols[i];
		this->symbol_map[symbol] = parent->headers[symbol].values[0];
		this->symbol_table.add_variable(symbol, this->symbol_map[symbol]);
	}
}

void Header::compileExpression() {
	if (this->expression[0] == '\0') {
		return;
	}
	parser_t parser;
	this->expr.register_symbol_table(this->symbol_table);
	parser.compile(this->expression, this->expr);
}

void Header::updateValues() {
	if (this->expression[0] == '\0') {
		return;
	}
	for (double i = 0; i < this->values.size(); i++) {
		this->symbol_map["i"] = i;
		for (int j = 0; j < parent->symbols.size(); j++) {
			std::string symbol = parent->symbols[j];
			this->symbol_map[symbol] = parent->headers[symbol].values[i];
		}
		this->values[i] = this->expr.value();
	}
}