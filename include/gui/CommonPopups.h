#ifndef DM_COMMONPOPUPS_H
#define DM_COMMONPOPUPS_H

#include <core/Expression.h>
#include <core/Graph.h>
#include <core/Stats.h>
#include <core/gui/Window.h>
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
	std::string symbol;
	std::string newSymbol;
	std::string newUnit;
	Expression expression;

public:
	EditVarPopup(MainWindow* parent, std::string symbol);
	void onRender() override;
	static EditVarPopup* getInstance(MainWindow* mw, std::string symbol);
	static bool hasInstance();
	static void removeInstance();
};


class NewGraphPopup : public Window {
	static NewGraphPopup* inst;
	MainWindow* parent;
	Project* pr;

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
	double from;
	double to;

public:
	ResolveEquation(MainWindow* parent);
	void onRender() override;
	static ResolveEquation* getInstance(MainWindow* mw);
	static void removeInstance();
};

#endif