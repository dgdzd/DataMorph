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

Header::Header(Project* parent, std::string name, std::string unit, std::vector<double> values, const char* expression, std::tuple<std::string, int, int> spec_expression) {
	this->parent = parent;
	this->name = name;
	this->unit = unit;
	this->expression = expression;
	this->spec_expression = spec_expression;
	this->values = values;
	this->symbol_map = {};
}

void Header::addVars() {
	if (this->expression[0] == '\0' && std::get<0>(this->spec_expression) == "") {
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
	if (std::get<0>(this->spec_expression) == "DIFF") {
		Project* pr = this->parent;
		int index_dy = std::get<1>(this->spec_expression);
		int index_dx = std::get<2>(this->spec_expression);
		Header* header_dy = &pr->headers[pr->symbols[index_dy]];
		Header* header_dx = &pr->headers[pr->symbols[index_dx]];
		for (int i = 0; i < this->values.size() - 1; i++) {
			this->values[i] = (header_dy->values[i + 1] - header_dy->values[i]) / (header_dx->values[i + 1] - header_dx->values[i]);
		}
		int i = this->values.size() - 1;
		this->values[i] = (header_dy->values[i] - header_dy->values[i - 1]) / (header_dx->values[i] - header_dx->values[i - 1]);
		return;
	}
	if (this->expression[0] == '\0') {
		return;
	}
	parser_t parser;
	this->expr.register_symbol_table(this->symbol_table);
	parser.compile(this->expression, this->expr);
}

void Header::updateValues() {
	if (std::get<0>(this->spec_expression) == "DIFF") {
		return;
	}
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