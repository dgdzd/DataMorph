#ifndef DM_MODEL_H
#define DM_MODEL_H

#include <exprtk.hpp>
#include <string>
#include <vector>

struct Model {
	std::string expr_str;
	double a, b, c, x;
	std::vector<double> values;
	exprtk::symbol_table<double> symbol_table;
	exprtk::expression<double> expression_t;

	Model(std::string expression, std::vector<std::string> symbols, std::vector<double> values);
	void addVars();
	bool compile();
	double value(double x);

};

#endif 