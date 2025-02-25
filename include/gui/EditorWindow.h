#ifndef DM_EDITORWINDOW_H
#define DM_EDITORWINDOW_H

#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <App.h>
#include <core/Project.h>
#include <imgui_internal.h>
#include <string>
#include <vector>
#include <utility>

using namespace ImGui;

class EditorWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Project* project;
	std::string filepath;

public:
	EditorWindow(Project* project);

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif