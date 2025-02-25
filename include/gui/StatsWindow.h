#ifndef DM_STATSWINDOW_H
#define DM_STATSWINDOW_H

#include <App.h>
#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <core/Project.h>
#include <imgui_internal.h>
#include <string>
#include <utility>
#include <vector>

using namespace ImGui;

class StatsWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Project* project;

public:
	StatsWindow(Project* project);

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif