#ifndef DM_PYTHONWINDOW_H
#define DM_PYTHONWINDOW_H


#include <core/gui/Window.h>
#include <App.h>
#include <core/Project.h>
#include <string>
#include <vector>
#include <utility>

using namespace ImGui;

class PythonWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Project* project;
	char* editor_text;

public:
	PythonWindow(Project* project);

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif