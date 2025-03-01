#include <gui/FluidWindow.h>

#include <App.h>
#include <FontManager.h>
#include <implot.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <Utils.h>


using namespace ImGui;

FluidWindow::FluidWindow(Project* project) {
	this->project = project;
	this->name = "Fluid Simulation Window";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoSavedSettings;
	this->settings = Settings::instance;
	this->width = 900.0;
	this->height = 900.0;
	this->size = 4.0;
	this->densities = {};
	for (int x = 0; x < (this->width - 100) / size; x++) {
		this->densities.push_back({});
		for (int y = 0; y < (this->height - 100) / size; y++) {
			this->densities[x].push_back(0.0);
		}
	}
}

void FluidWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void FluidWindow::onDetach() {

}

void FluidWindow::onPreRender() {
	PushFont(this->font23);
}

void FluidWindow::onPostRender() {
	PopFont();
}

void FluidWindow::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);
	SetWindowSize(ImVec2(this->width, this->width));

	if (BeginChild("##canva", ImVec2(this->width-100, this->width - 100))) {
		render();

		EndChild();
	}
	SameLine();
	//ImPlot::PushColormap(ImPlotColormap_Hot);
	//ImPlot::ColormapScale("Density", 0.0, 100.0, ImVec2(0, 0), "%g dB");
}

void FluidWindow::render() {
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 p_min = ImGui::GetCursorScreenPos();
	ImVec2 p_max = ImVec2(p_min.x + this->width - 100, p_min.y + this->width - 100);

	draw_list->AddRectFilled(p_min, p_max, IM_COL32(0, 0, 0, 255));
	draw_list->AddRect(p_min, p_max, IM_COL32(255, 255, 255, 255), 2.0f);

	for (int x = 0; x < densities.size(); x++) {
		for (int y = 0; y < densities[x].size(); y++) {
			draw_list->AddRectFilled(ImVec2(x*size + p_min.x, y*size + p_min.y), ImVec2(size, size), IM_COL32(0, 255, 0, 255));
		}
	}
}

void FluidWindow::message(std::string header, ...) {
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