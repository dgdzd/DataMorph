#ifndef DM_MAINWINDOW_H
#define DM_MAINWINDOW_H

#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <App.h>
#include <imgui_internal.h>
#include <string>

using namespace ImGui;

struct Project {
	std::string name;
	std::string path;
	std::vector<std::string> symbols;
	std::vector<std::string> units;
	std::vector<std::vector<double*>> values;

	Project(std::string name, std::string path) {
		this->name = name;
		this->path = path;
		this->units = {};
		this->symbols = {};
		this->values = std::vector<std::vector<double*>>(symbols.size(), std::vector<double*>(1, nullptr));
		double newValue = 0.0;
		for (int i = 0; i < this->values.size(); i++) {
			this-> values[i][0] = new double(newValue);
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
	int max_rows;

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