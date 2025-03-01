#ifndef DM_FUILDWINDOW_H
#define DM_FLUIDWINDOW_H

#include <App.h>
#include <core/gui/Window.h>
#include <core/Project.h>
#include <core/imgui_extension.h>
#include <string>

using namespace ImGui;

class FluidWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Project* project;
	double width;
	double height;
	double size;
	std::vector<std::vector<double>> densities;

public:
	FluidWindow(Project* project);

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void render();
	void message(std::string header, ...) override;
};

#endif