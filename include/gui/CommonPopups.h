#ifndef DM_COMMONPOPUPS_H
#define DM_COMMONPOPUPS_H

#include <core/Expression.h>
#include <core/Graph.h>
#include <core/gui/Window.h>
#include <core/Settings.h>
#include <core/Stats.h>
#include <imgui/imgui_stdlib.h>
#include <iostream>
#include <functional>
#include <Utils.h>
#include <vector>

class MainWindow;

class NewVarPopup : public Window {
	static NewVarPopup* inst;
	MainWindow* parent;
	char* newSymbol;
	char* newUnit;
	char* expression;
	std::pair<int, int> derivate;
	std::pair<float, float> linespace;
	std::pair<int, int> integral;
	std::vector<double> args;

public:
	NewVarPopup(MainWindow* parent);
	void onRender() override;
	static NewVarPopup* getInstance(MainWindow* mw);
	static void removeInstance();
};


class EditVarPopup : public Window {
	static EditVarPopup* inst;
	MainWindow* parent;
	Project* pr;
	unsigned int id;
	std::string newSymbol;
	std::string newUnit;
	Expression expression;

public:
	EditVarPopup(MainWindow* parent, unsigned int id);
	void onRender() override;
	static EditVarPopup* getInstance(MainWindow* mw, unsigned int id);
	static bool hasInstance();
	static void removeInstance();
};


class NewGraphPopup : public Window {
	static NewGraphPopup* inst;
	MainWindow* parent;
	Project* pr;
	Settings* settings;

public:
	NewGraphPopup(MainWindow* parent);
	void onRender() override;
	static NewGraphPopup* getInstance(MainWindow* mw);
	static void removeInstance();
};


class NewStatsPopup : public Window {
	static NewStatsPopup* inst;
	MainWindow* parent;
	Project* pr;

public:
	NewStatsPopup(MainWindow* parent);
	void onRender() override;
	static NewStatsPopup* getInstance(MainWindow* mw);
	static void removeInstance();
};


class ResolveEquation : public Window {
	static ResolveEquation* inst;
	MainWindow* parent;
	Project* pr;
	std::string equation;
	std::string result;
	std::string error;
	double from;
	double to;

	std::vector<double> resolveEqu(std::string l_expression, std::string r_expression, std::string& error, int max_iter=10e3);

public:
	ResolveEquation(MainWindow* parent);
	void onRender() override;
	static ResolveEquation* getInstance(MainWindow* mw);
	static void removeInstance();
};

#endif