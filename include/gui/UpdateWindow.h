#ifndef DM_UPDATEWINDOW_H
#define DM_UPDATEWINDOW_H

#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <imgui_internal.h>
#include <string>
#include <Constants.h>

using namespace ImGui;

class UpdateWin : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;

public:
	UpdateWin();
	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif