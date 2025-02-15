#ifndef DM_COMMONPOPUPS_H
#define DM_COMMONPOPUPS_H

#include <core/Expression.h>
#include <core/Graph.h>
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
	std::vector<int> integral;
	std::vector<double> args;

public:
	NewVarPopup(MainWindow* parent);
	void onRender() override;
	static NewVarPopup* getInstance(MainWindow* mw);
};


class NewGraphPopup : public Window {
	static NewGraphPopup* inst;
	MainWindow* parent;
	Project* pr;

public:
	NewGraphPopup(MainWindow* parent);
	void onRender() override;
	static NewGraphPopup* getInstance(MainWindow* mw);
};

#endif