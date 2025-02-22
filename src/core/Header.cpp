#include <core/Header.h>

#include <App.h>
#include <core/Expression.h>
#include <core/Project.h>
#include <iostream>
#include <random>

Header::Header() {
	this->name = "";
	this->unit = "";
	this->expression = nullptr;
	this->values = {};
	this->parent = nullptr;
	this->lockedValues = false;
}

Header::Header(Project* parent, std::string name, std::string unit, std::vector<double> values, const char* expression, ExpressionSpecs* specs, std::vector<double> args) {
	this->parent = parent;
	this->name = name;
	this->unit = unit;
	this->values = values;
	this->lockedValues = false;
	if (specs->type == DUMMY) {
		this->lockedValues = false;
		this->expression = new Expression(this, ExpressionSpecs());
	}
	else {
		if (expression[0] == '\0') {
			this->expression = new Expression(this, specs ? *specs : ExpressionSpecs(), args);
		}
		else {
			this->expression = new Expression(this, expression, specs ? *specs : ExpressionSpecs());
		}
		this->lockedValues = true;
	}

	// Generate ID
	srand(DataMorph::getTime());
	this->id = (int)(100000 + (rand() / RAND_MAX) * 899999); // Génère un ID entre 100000 et 999999
}