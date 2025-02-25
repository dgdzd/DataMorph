#ifndef DM_MODEL_H
#define DM_MODEL_H

#define exprtk_disable_caseinsensitivity
#include <exprtk.hpp>
#include <string>
#include <vector>
#include <core/Graph.h>

enum ModelType : int {
	NONE = -1,
	CUSTOM,
	LINEAR,
	AFFINE,
	PONYNOMIAL,
	QUADRATIC,
	CUBIC,
	EXPONENTIAL,
	LOGARITHMIC,
	POWER,
	OTHER
};

struct Model {
	ModelType type;
	std::string expr_str;
	double a, b, c, x;
	Line* dataset;
	std::string xlabel;
	std::vector<double> values;
	exprtk::symbol_table<double> symbol_table;
	exprtk::expression<double> expression_t;

	Model(std::string expression, std::vector<std::string> symbols, std::vector<double> values, std::string xlabel = "x");
	void refresh();
	void addVars();
	bool compile();
	double value(double x);

};

#endif 