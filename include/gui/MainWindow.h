#ifndef DM_MAINWINDOW_H
#define DM_MAINWINDOW_H

#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <App.h>
#include <imgui_internal.h>
#include <string>
#include <map>
#include <iostream>

using namespace ImGui;

struct Project {
	std::string name;
	std::string path;
	std::vector<std::string> symbols;
	std::vector<std::string> units;
	std::vector<std::vector<double>> values;

	Project(std::string name, std::string path) {
		this->name = name;
		this->path = path;
		this->units = {};
		this->symbols = {};
		this->values = {};
	}

	void initValues() {
		this->values = { std::vector<double>(this->symbols.size(), 0.0) };
		
	}

	void addRow() {
		this->values.push_back(std::vector<double>(this->symbols.size(), 0.0));
	}

	void removeRow() {
		this->values.pop_back();
	}

	void addColumn() {
		for (int i = 0; i < this->values.size(); i++) {
			this->values[i].push_back(0.0);
		}
	}

	void removeColumn() {
		for (int i = 0; i < this->values.size(); i++) {
			this->values[i].pop_back();
		}
	}

	/*void resize_values(int rows, int columns) {
		for (int i; i < values.size(); i++) {
			for (int j; j < values[i].size(); j++) {
				this->values[i][j] = this->values[i][j];
			}
		}
		this->values.resize(columns);
		
		for (int i; i < rows; i++) {
			this->values.resize(rows);
		}
	}*/
};

struct State {
	bool hasOpenProject;
	Project* openProject;
	std::map<std::string, bool> popups;

	State() {
		this->openProject = nullptr;
	}
};

class MainWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
public:
	State* state;

	MainWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif