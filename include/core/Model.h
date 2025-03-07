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
	QUADRATIC,
	CUBIC,
	LOG10,
	LOG,
	LOGN,
	SINUS,
	EXPN,
	EXP,
	SQRT,
	INV,
	OTHER,

	AS,
	LS,
	HS,
	LIS,
	COCH,
	RHO,
	RHO2,
	BL,
	CARD,
	LP,
	EH,
	LC,
	CO
};

struct Model {
	ModelType type;
	std::string expr_str;
	double a, b, c, d, n, x;
	double u;
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