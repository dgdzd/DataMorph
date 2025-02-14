#ifndef DM_EXPRESSION_H
#define DM_EXPRESSION_H

#include <exprtk.hpp>
#include <map>
#include <string>

typedef int                          expr_type;
typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>   expression_t;
typedef exprtk::parser<double>       parser_t;

struct Header;

enum ExpressionType : expr_type {
	DUMMY = -1,
	FORMULA = 0,
	DERIVATIVE = 1,
	INTEGRAL = 2,
	LINESPACE = 3
};

struct ExpressionSpecs {
	ExpressionType type;
	Header* header_dx;
	Header* header_dy;

	ExpressionSpecs() : type(DUMMY), header_dx(nullptr), header_dy(nullptr) {}
	ExpressionSpecs(ExpressionType type, Header* header_dx, Header* header_dy) {
		this->type = type;
		this->header_dx = header_dx;
		this->header_dy = header_dy;
	}
};

class Expression {
	std::map<std::string, double> symbol_map;
	expression_t expr;
	symbol_table_t symbol_table;

public:
	Header* parent;
	ExpressionSpecs specs;
	std::string expression;
	std::vector<float> args;

	Expression(Header* parent, std::string expression);
	Expression(Header* parent, ExpressionSpecs specs);
	Expression(Header* parent, std::string expression, ExpressionSpecs specs);
	Expression(Header* parent, ExpressionSpecs specs, std::vector<float> args);

	void addVars();
	void compileExpression();
	void updateValues();
};

#endif