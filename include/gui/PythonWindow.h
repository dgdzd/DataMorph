#ifndef DM_PYTHONWINDOW_H
#define DM_PYTHONWINDOW_H

#include <App.h>
#include <core/gui/Window.h>
#include <core/Project.h>
#include <string>

using namespace ImGui;

class PythonWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Project* project;
	std::string editor_text;

public:
	static PythonWindow* current;

	PythonWindow(Project* project);

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif