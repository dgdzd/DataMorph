#include <core/Expression.h>
#include <core/Header.h>
#include <core/Project.h>

Header::Header() {
	this->name = "";
	this->unit = "";
	this->expression = nullptr;
	this->values = {};
	this->parent = nullptr;
}

Header::Header(Project* parent, std::string name, std::string unit, std::vector<double> values, const char* expression, ExpressionSpecs* specs, std::vector<float> args) {
	this->parent = parent;
	this->name = name;
	this->unit = unit;
	this->values = values;
	if (args.size() == 0) {
		this->expression = new Expression(this, ExpressionSpecs(), args);
	}
	else {
		this->expression = new Expression(this, expression, specs ? *specs : ExpressionSpecs());
	}
}