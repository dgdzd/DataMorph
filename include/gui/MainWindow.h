#ifndef DM_MAINWINDOW_H
#define DM_MAINWINDOW_H

#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <App.h>
#include <imgui_internal.h>
#include <string>
#include <iostream>

using namespace ImGui;

struct Project {
	std::string name;
	std::string path;
	std::vector<std::string> symbols;
	std::vector<std::string> units;
	std::vector<std::vector<double>> values;
	int values_rows;

	Project(std::string name, std::string path) {
		this->name = name;
		this->path = path;
		this->units = {};
		this->symbols = {};
		this->values = {};
		this->values_rows = 1;
	}

	void initValues() {
		double newValue = 0.0;
		for (int i = 0; i < this->units.size(); i++) {
			std::cout << "a\n";
			this->values.push_back({ 0 });
		}
	}

	void resize_values(int rows, int columns) {
		for (int i; i < values.size(); i++) {
			for (int j; j < values[i].size(); j++) {
				this->values[i][j] = *new double(this->values[i][j]);
			}
		}
		this->values.resize(columns);
		
		for (int i; i < rows; i++) {
			this->values.resize(rows);
		}
	}
};

struct State {
	bool hasOpenProject;
	Project* openProject;

	State() {
		this->openProject = nullptr;
	}
};

class MainWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	State* state;

public:
	MainWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif