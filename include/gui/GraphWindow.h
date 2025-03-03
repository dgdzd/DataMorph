#ifndef DM_GRAPHWINDOW_H
#define DM_GRAPHWINDOW_H

#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <App.h>
#include <core/Project.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include <utility>

using namespace ImGui;

class GraphWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Project* project;
	unsigned int statsVar;
	std::string model_message;
	ImPlotRect limits;
	std::string model_text;
	unsigned int numbersVar;

public:
	static GraphWindow* current;

	GraphWindow(Project* project);

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif