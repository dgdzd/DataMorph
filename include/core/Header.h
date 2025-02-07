#ifndef DM_HEADER_H
#define DM_HEADER_H

#include <exprtk.hpp>
#include <map>
#include <string>
#include <vector>

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>   expression_t;
typedef exprtk::parser<double>       parser_t;
struct Project;

struct Header {
	Project* parent;
	std::string name;
	std::string unit;
	const char* expression;
	std::vector<double> values;
	std::map<std::string, double> symbol_map;
	symbol_table_t symbol_table;
	expression_t expr;

	Header();
	Header(Project* parent, std::string name, std::string unit, std::vector<double> values, const char* expression = "");

	void addVars();
	void compileExpression();
	void updateValues();
};

#endif