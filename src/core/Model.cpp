#include <core/Model.h>

Model::Model(std::string name, std::string expression, std::vector<std::string> symbols, std::vector<double> values) {
	this->name = name;
	this->expr_str = expression;
	this->values = { 0.0, 0.0 };
	this->addVars();
}

void Model::addVars() {
	this->symbol_table.add_variable("a", this->a);
	this->symbol_table.add_variable("b", this->b);
	this->symbol_table.add_variable("c", this->c);
	this->expression_t.register_symbol_table(this->symbol_table);
}