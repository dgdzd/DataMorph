#ifndef DM_NEWPROJECTWINDOW_H
#define DM_NEWPROJECTWINDOW_H

#include <core/gui/Window.h>
#include <string>
#include <vector>

using namespace ImGui;

class NewProjectWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	std::vector<char*> inputs;
	std::vector<std::vector<std::string>> symbols;
	std::vector<std::vector<std::string>> units;
	std::vector<std::vector<double>> values;
	std::string path;
	std::string filename;
	int filesize;
	int graphics;
	char project_name[32] = "New project";

public:
	NewProjectWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif