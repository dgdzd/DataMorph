#ifndef DM_MAINWINDOW_H
#define DM_MAINWINDOW_H

#include <App.h>
#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <core/Project.h>
#include <core/Header.h>
#include <exprtk.hpp>
#include <imgui_internal.h>
#include <iostream>
#include <map>
#include <string>

using namespace ImGui;

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