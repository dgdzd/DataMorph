#include <gui/StatsWindow.h>

#include <App.h>
#include <FontManager.h>
#include <implot.h>
#include <iostream>
#include <Utils.h>

using namespace ImGui;

StatsWindow::StatsWindow(Project* project) {
	this->project = project;
	this->name = "Stats - " + project->name;
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoSavedSettings;
}

void StatsWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void StatsWindow::onDetach() {

}

void StatsWindow::onPreRender() {
	PushFont(this->font23);
}

void StatsWindow::onPostRender() {
	PopFont();
}

void StatsWindow::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);
	SetWindowSize(ImVec2(1200.0f, 500.0f));

	if (BeginTabBar("stats_tabs")) {
		for (int i = 0; i < project->stats.size(); i++) {
			Stats& s = project->stats[i];
			if (BeginTabItem(s.name.c_str())) {
				Bar& b = s.bar;
				int _size = s.xHeader->values.size();
				double bar_size = 1200.0/ static_cast<double>(_size);
				if (ImPlot::BeginPlot("Stats")) {
					ImPlot::PushStyleColor(ImPlotCol_Line, *b.color);
					ImPlot::PlotBars((b.header->name + "##Plot").c_str(), &s.xHeader->values[0], &b.header->values[0], _size, bar_size);
					ImPlot::PopStyleColor();

					ImPlot::EndPlot();
				}

				EndTabItem();
			}
		}
		EndTabBar();
	}
}

void StatsWindow::message(std::string header, ...) {
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