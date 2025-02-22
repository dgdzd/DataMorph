#ifndef DM_HEADER_H
#define DM_HEADER_H

#include <core/Expression.h>
#include <map>
#include <string>
#include <vector>

class Expression;
struct Project;

struct Header {
	unsigned int id;
	Project* parent;
	Expression* expression;
	std::vector<float> args;
	std::string name;
	std::string unit;
	std::vector<double> values;
	bool lockedValues;

	Header();
	Header(Project* parent, std::string name, std::string unit, std::vector<double> values, const char* expression = "", ExpressionSpecs* specs = new ExpressionSpecs(), std::vector<double> args = {});
};

#endif