#include <gui/GraphWindow.h>
#include <App.h>
#include <FontManager.h>
#include <Utils.h>
#include <iostream>

using namespace ImGui;

GraphWindow::GraphWindow(Project* project) {
	this->project = project;
	this->name = "Graphs - " + project->name;
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_None;
}

void GraphWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void GraphWindow::onDetach() {
	
}

void GraphWindow::onPreRender() {
	PushFont(this->font23);
}

void GraphWindow::onPostRender() {
	PopFont();
}

void GraphWindow::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);
	SetWindowSize(ImVec2(900.0f, 500.0f));

	if (BeginTabBar("graphs_tabs")) {
		for (int i = 0; i < project->graphs.size(); i++) {
			Graph& g = project->graphs[i];
			if (BeginTabItem(g.name.c_str())) {
				// TODO : Render graph

				EndTabItem();
			}
		}
		EndTabBar();
	}
}

void GraphWindow::message(std::string header, ...) {
	va_list args;
	va_start(args, header);
	std::string message = header;
	message += "\n";
	std::string line;
	while ((line = va_arg(args, std::string)) != "") {
		message += line;
		message += "\n";
	}
	va_end(args);
	std::cout << message;
}