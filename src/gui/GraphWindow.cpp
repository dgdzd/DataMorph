#include <gui/GraphWindow.h>

#include <App.h>
#include <cmath>
#include <core/Model.h>
#include <core/Regression.h>
#include <FontManager.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <iostream>
#include <Utils.h>

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
	this->wflags = ImGuiWindowFlags_NoSavedSettings;
	this->statsVar = 0;
	this->model_text = "";
	this->numbersVar = 0;
	this->settings = Settings::instance;
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
	SetWindowSize(ImVec2(1200.0f, 800.0f));

	if (BeginTabBar("graphs_tabs")) {
		for (int i = 0; i < project->graphs.size(); i++) {
			Graph& g = project->graphs[i];
			if (BeginTabItem(g.name.c_str())) {
				if (BeginChild("Infos", ImVec2(GetContentRegionAvail().x * 0.5f + 100, 300), ImGuiChildFlags_Borders, ImGuiWindowFlags_MenuBar)) {
					if (BeginMenuBar()) {
						if (BeginMenu("Infos")) {
							ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
					}
					Text("Modeling : ");
					if (!g.model->dataset) {
						g.model->dataset = &g.lines[0];
					}
					std::string x = g.xHeader->name;
					std::string y = g.model->dataset->header->name;
					if (BeginCombo("Choose a model", ModelTypeToString(g.model->type).c_str())) {
						if (Selectable("Custom")) {
							g.model->type = ModelType::CUSTOM;
							g.model->expr_str = y + "=" + x;
							g.model->refresh();
						}
						if (Selectable("Linear")) {
							g.model->type = ModelType::LINEAR;
							g.model->expr_str = y + "=a*" + x;
							g.model->refresh();
						}
						if (Selectable("Affine")) {
							g.model->type = ModelType::AFFINE;
							g.model->expr_str = y + "=a*" + x + "+b";
							g.model->refresh();
						}
						if (Selectable("Quadratic")) {
							g.model->type = ModelType::QUADRATIC;
							g.model->expr_str = y + "=a*" + x + "^2+b*" + x + "+c";
							g.model->refresh();
						}
						if (Selectable("Cubic")) {
							g.model->type = ModelType::CUBIC;
							g.model->expr_str = y + "=a*" + x + "^3+b*" + x + "^2+c*x+d";
							g.model->refresh();
						}
						if (Selectable("Sine")) {
							g.model->type = ModelType::SINUS;
							g.model->expr_str = y + "=a*sin(b*" + x + "+c)";
							g.model->refresh();
						}

						BeginDisabled();
						{
							if (Selectable("Exponential")) {
								g.model->type = ModelType::LINEAR;
								g.model->expr_str = y + "=b*a^("+ x +")";
								//e = 2.71828
							}
							if (Selectable("Logarithmic")) {
								g.model->type = ModelType::LINEAR;
								g.model->expr_str = y + "=a*log[b](" + x + ")+c";
							}
							if (Selectable("Square Root")) {
								g.model->type = ModelType::LINEAR;
								g.model->expr_str = y + "=a*sqrt(" + x + ")+c";
							}
							if (Selectable("Sigmoid")) {
								g.model->type = ModelType::LINEAR;
								g.model->expr_str = y + "=1/(1+2.71828^-" + x + ")";
								//e = 2.71828
							}
							EndDisabled();
						}
						EndCombo();
					}
					if (BeginCombo("Choose dataset", g.model->dataset->header->name.c_str())) {
						for (int i = 0; i < g.lines.size(); i++) {
							if (Selectable(g.lines[i].header->name.c_str())) {
								g.model->dataset = &g.lines[i];
							}
						}
						EndCombo();
					}

					if (g.model->type == CUSTOM) {
						InputText("No blank", &g.model->expr_str);
					}
					else {
						InputText("No blank", &g.model->expr_str, ImGuiInputTextFlags_ReadOnly);
					}
					TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
					if (IsItemHovered()) {
						SetTooltip("Select a model from the options above. \n The computer will automaticly determine the values of a, c and b. \n Note : the experimentals data musn't reverse the order of the model");
					}

					if (Button("Update model")) {
						if (g.model->expr_str.size() == 0) {
							model_text = "model empty";
						}
						else if (g.model->expr_str == g.xHeader->name+"="+x) {
							model_text = "model invalid";

						}
						else if (g.model->type == AFFINE) { // Affine
							if (Regression::affine(g.xHeader->values, g.model->dataset->header->values, g.model->b, g.model->a)) {
								Model* m = g.model;
								m->values = {};
								for (int i = 0; i < g.xHeader->values.size(); i++) {
									m->values.push_back(m->value(g.xHeader->values[i]));
								}
								model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b);
							}
						}
						else if (g.model->type == LINEAR) { // Linear
							if (Regression::linear(g.xHeader->values, g.model->dataset->header->values, g.model->a)) {
								Model* m = g.model;
								m->values = {};
								for (int i = 0; i < g.xHeader->values.size(); i++) {
									m->values.push_back(m->value(g.xHeader->values[i]));
								}
								model_text = "a = " + std::to_string(g.model->a);
							}
						}
						else if (g.model->type == CUSTOM) {
							if (Regression::custom(g.xHeader->values, g.model->dataset->header->values, g.model->expr_str, g.model->a, g.model->b, g.model->c, g.model->xlabel)) {
								Model* m = g.model;
								m->values = {};
								for (int i = 0; i < g.xHeader->values.size(); i++) {
									m->values.push_back(m->value(g.xHeader->values[i]));
								}
								model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n" + "c = " + std::to_string(g.model->c) + "\n";
							}
						}
						else if (g.model->type == QUADRATIC) {
							if (Regression::quadratic(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->c)) {
								Model* m = g.model;
								m->values = {};
								for (int i = 0; i < g.xHeader->values.size(); i++) {
									m->values.push_back(m->value(g.xHeader->values[i]));
								}
								model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n" + "c = " + std::to_string(g.model->c) + "\n";
							}
						}
						else if (g.model->type == CUBIC) {
							if (Regression::cubic(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->c, g.model->d)) {
								Model* m = g.model;
								m->values = {};
								for (int i = 0; i < g.xHeader->values.size(); i++) {
									m->values.push_back(m->value(g.xHeader->values[i]));
								}
								model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n" + "c = " + std::to_string(g.model->c) + "\n" + "d = " + std::to_string(g.model->d);
							}
						}
						else if (g.model->type == SINUS) {
							if (Regression::sinusoidal(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->c)) {
								Model* m = g.model;
								m->values = {};
								for (int i = 0; i < g.xHeader->values.size(); i++) {
									m->values.push_back(m->value(g.xHeader->values[i]));
								}
								model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n" + "c = " + std::to_string(g.model->c) + "\n";
							}
						}
						else {
							
						}
					}
					Text(model_text.c_str());

					EndChild();
				}
				SameLine();
				if (ImPlot::BeginPlot("Graph")) {
					for (int j = 0; j < g.lines.size(); j++) {
						Line& l = g.lines[j];
	
						if (l.scatter) {
							int _size = g.xHeader->values.size();
							ImPlot::SetNextLineStyle(*l.color);
							ImPlot::SetNextMarkerStyle(l.marker);
							ImPlot::PlotScatter((l.header->name + "##Plot" + std::to_string(j)).c_str(), &g.xHeader->values[0], &l.header->values[0], _size);
						}
						else {
							int _size = g.xHeader->values.size();
							ImPlot::SetNextLineStyle(*l.color);
							ImPlot::SetNextMarkerStyle(l.marker);
							ImPlot::PlotLine((l.header->name + "##Plot" + std::to_string(j)).c_str(), &g.xHeader->values[0], &l.header->values[0], _size);
						}
					}
					if (g.model && g.model->values.size() != 0) {
						ImPlot::SetNextLineStyle(ImPlot::GetColormapColor(g.lines.size()+1, this->settings->get_int("graphs_cmap")));
						ImPlot::SetNextMarkerStyle(ImPlotMarker_None);
						g.limits = ImPlot::GetPlotLimits();

						if (g.model->type == AFFINE || g.model->type == LINEAR) {
							ImPlot::PlotLineG("Model##Plot", [](int idx, void* data) -> ImPlotPoint {
								Graph& g = *(Graph*)data;
								double x = g.limits.Min().x + idx * abs(g.limits.Max().x - g.limits.Min().x);
								return ImPlotPoint(x, g.model->value(x));
							}, (void*)&g, 2);
						}
						else {
							ImPlot::PlotLineG("Model##Plot", [](int idx, void* data) -> ImPlotPoint {
								Graph& g = *(Graph*)data;
								double x = g.limits.Min().x + (idx / 30.0f) * abs(g.limits.Max().x - g.limits.Min().x);
								return ImPlotPoint(x, g.model->value(x));
							}, (void*)&g, 31);
						}
					}
					ImPlot::EndPlot();
				}
				if (TreeNode("Statistics")) {
					if (BeginChild("##StatsChild", ImVec2(0, 0), ImGuiChildFlags_Borders)) {
						if (BeginCombo("Data", this->statsVar == 0 ? "None" : project->headers[this->statsVar].name.c_str())) {
							if (Selectable("None")) {
								this->statsVar = 0;
							}
							for (int i = 0; i < project->headers.size(); i++) {
								if (Selectable(project->headers[project->ids[i]].name.c_str())) {
									this->statsVar = project->ids[i];
								}
							}
							EndCombo();
						}
						if (BeginCombo("Numbers", this->numbersVar == 0 ? "None" : project->headers[this->numbersVar].name.c_str())) {
							if (Selectable("None")) {
								this->numbersVar = 0;
							}
							for (int i = 0; i < project->headers.size(); i++) {
								if (Selectable(project->headers[project->ids[i]].name.c_str())) {
									this->numbersVar = project->ids[i];
								}
							}
							EndCombo();
						}
						if (this->statsVar != 0) {
							Header* h = &project->headers[this->statsVar];
							Header* h2 = &project->headers[this->numbersVar];

							std::vector<double> numbers = {};
							if (this->numbersVar == 0) {
								for (int i = 0; i < h->values.size(); i++) {
									numbers.push_back(1.0);
								}
							}
							else {
								for (int i = 0; i < h2->values.size(); i++) {
									numbers.push_back(h2->values[i]);
								}
							}

							// Average
							double average = 0;
							double coeff = 0;
							for (int v = 0; v < h->values.size(); v++) {
								average += h->values[v]*numbers[v];
							}

							for (int i = 0; i < numbers.size(); i++) {
								coeff += numbers[i];
							}
							average /= coeff;

							// Standard deviation
							double standard_deviation = 0;
							for (int v = 0; v < h->values.size(); v++) {
								standard_deviation += pow(average - h->values[v]*numbers[v], 2);
							}
							standard_deviation /= h->values.size();
							standard_deviation = sqrt(standard_deviation);

							// Sample standard deviation
							double standard_deviation1 = 0;
							for (int v = 0; v < h->values.size(); v++) {
								standard_deviation1 += pow(average - h->values[v]*numbers[v], 2);
							}
							standard_deviation1 /= h->values.size()-1;
							standard_deviation1 = sqrt(standard_deviation1);

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The number of values in %s.", h->name);
							}
							SameLine();
							Text("N(%s) = %d", h->name, h->values.size());

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The smallest value of %s.", h->name);
							}
							SameLine();
							Text("min(%s) = %.5f %s", h->name, std::min(h->values), h->unit);

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The highest value of %s.", h->name);
							}
							SameLine();
							Text("max(%s) = %.5f %s", h->name, std::max(h->values), h->unit);

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The scope of %s.", h->name);
							}
							SameLine();
							Text("scope(%s) = %.5f %s", h->name, std::max(h->values) - std::min(h->values), h->unit);

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The average %s.", h->name);
							}
							SameLine();
							Text((const char*)u8"%s\u2098 = %.5f %s", h->name, average, h->unit);

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The sum of all the values of %s.", h->name);
							}
							SameLine();
							Text((const char*)u8"Σ(%s) = %.5f %s", h->name, std::sum(h->values), h->unit);

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The standard deviation of %s.", h->name);
							}
							SameLine();
							Text((const char*)u8"σ(%s) = %.5f %s", h->name, standard_deviation, h->unit);

							TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
							if (IsItemHovered()) {
								SetTooltip("The sample standard deviation of %s.", h->name);
							}
							SameLine();
							Text((const char*)u8"s(%s) = %.5f %s", h->name, standard_deviation1, h->unit);
						}
						EndChild();
					}

					TreePop();
				}

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