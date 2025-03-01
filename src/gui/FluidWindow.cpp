# include <gui/FluidWindow.h>

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
	this->editor_text = "# New python file\n\nprint(\"Hello World!\")";
	this->settings = Settings::instance;
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
	SetWindowSize(ImVec2(1200.0f, 900.0f));

	ImPlot::ShowColormapSelector();

	if (ImPlot::BeginPlot("##CustomRend", ImVec2(800.0f, 800.0f))) {
		ImPlot::GetPlotDrawList()->AddRect(ImVec2(-400, 400), ImVec2(400, -400), IM_COL32(128, 0, 255, 255));


		ImPlot::EndPlot();
	}
	ImPlot::ColormapScale("Density", 0.0, 1.0, ImVec2(0, 0), "%g dB");


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