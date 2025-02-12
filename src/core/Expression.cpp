#include <core/Expression.h>

#include <core/Project.h>
#include <iostream>

Expression::Expression(Header* parent, std::string expression) {
	this->parent = parent;
	this->expression = expression;
}

Expression::Expression(Header* parent, ExpressionSpecs specs) {
	this->parent = parent;
	this->expression = "";
	this->specs = specs;
}

Expression::Expression(Header* parent, std::string expression, ExpressionSpecs specs) {
	this->parent = parent;
	this->expression = expression;
	this->specs = specs;
}

void Expression::addVars() {
	if (this->specs.type != FORMULA) {
		return;
	}
	this->symbol_map["i"] = 0;
	this->symbol_table.add_variable("i", this->symbol_map["i"]);
	Project* p = this->parent->parent;
	for (int i = 0; i < p->symbols.size(); i++) {
		std::string symbol = p->symbols[i];
		this->symbol_map[symbol] = p->headers[symbol].values[0];
		this->symbol_table.add_variable(symbol, this->symbol_map[symbol]);
	}
}

void Expression::compileExpression() {
	if (this->specs.type == DERIVATIVE) {
		Header* h = this->parent;
		Project* pr = h->parent;
		h->values[0] = 0.0;
		for (int i = 1; i < h->values.size(); i++) {
			h->values[i] = (this->specs.header_dy->values[i] - this->specs.header_dy->values[i - 1]) / (this->specs.header_dx->values[i] - this->specs.header_dx->values[i - 1]);
		}
		return;
	}
	if (this->specs.type == DUMMY) {
		return;
	}
	parser_t parser;
	this->expr.register_symbol_table(this->symbol_table);
	parser.compile(this->expression, this->expr);
}

void Expression::updateValues() {
	Header* h = this->parent;
	Project* pr = h->parent;
	if (this->specs.type == DUMMY) {
		return;
	}
	if (this->specs.type == DERIVATIVE) {
		h->values[0] = 0.0;
		for (int i = 1; i < h->values.size() - 1; i++) {
			h->values[i] = (this->specs.header_dy->values[i] - this->specs.header_dy->values[i - 1]) / (this->specs.header_dx->values[i] - this->specs.header_dx->values[i - 1]);
		}
	}
	else {
		for (double i = 0; i < h->values.size(); i++) {
			this->symbol_map["i"] = i;
			for (int j = 0; j < pr->symbols.size(); j++) {
				std::string symbol = pr->symbols[j];
				this->symbol_map[symbol] = pr->headers[symbol].values[i];
			}
			h->values[i] = this->expr.value();
		}
	}
}