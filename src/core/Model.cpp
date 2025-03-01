#include <core/Model.h>

Model::Model(std::string expression, std::vector<std::string> symbols, std::vector<double> values, std::string xlabel) {
	this->type = ModelType::NONE;
	this->expr_str = expression;
	this->values = { };
	this->xlabel = xlabel;
	this->dataset = nullptr;
	this->a = 0;
	this->b = 0;
	this->c = 0;
	this->d = 0;
	this->n = 0;
	this->x = 0;
	this->u = 0;
	this->addVars();
	this->compile();
}

void Model::refresh() {
	this->values.clear();
	this->addVars();
	this->compile();
}

void Model::addVars() {
	this->symbol_table.add_variable("a", this->a);
	this->symbol_table.add_variable("b", this->b);
	this->symbol_table.add_variable("c", this->c);
	this->symbol_table.add_variable("d", this->d);
	this->symbol_table.add_variable("n", this->n);
	this->symbol_table.add_variable(xlabel, this->x);
	this->expression_t.register_symbol_table(this->symbol_table);
}

bool Model::compile() {
	exprtk::parser<double> parser;
	int equ_i = this->expr_str.find('=');
	if (equ_i != std::string::npos) {
		this->expr_str = this->expr_str.substr(equ_i + 1);
	}
	if (!parser.compile(this->expr_str, this->expression_t)) {
		return false;
	}
	return true;
}

double Model::value(double x) {
	this->x = x;
	double val = this->expression_t.value();
	return val;
}