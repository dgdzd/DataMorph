#ifndef DM_HEADER_H
#define DM_HEADER_H

#include <core/Expression.h>
#include <map>
#include <string>
#include <vector>

class Expression;
struct Project;

struct Header {
	Project* parent;
	Expression* expression;
	std::string name;
	std::string unit;
	std::vector<double> values;

	Header();
	Header(Project* parent, std::string name, std::string unit, std::vector<double> values, const char* expression = "", ExpressionSpecs* specs = new ExpressionSpecs());
};

#endif