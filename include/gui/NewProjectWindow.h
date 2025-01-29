#ifndef DM_NEWPROJECTWINDOW_H
#define DM_NEWPROJECTWINDOW_H

#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <App.h>
#include <imgui_internal.h>
#include <string>

using namespace ImGui;

class NewProjectWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;

public:
	NewProjectWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
};

#endif