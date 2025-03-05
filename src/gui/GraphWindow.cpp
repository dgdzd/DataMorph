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

GraphWindow* GraphWindow::current = nullptr;

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

	GraphWindow::current = this;
}

void GraphWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void GraphWindow::onDetach() {
	GraphWindow::current = nullptr;
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
				if (BeginChild("UpLayer", ImVec2(0, GetContentRegionAvail().y * 0.75f), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY)) {
					if (BeginChild("Model", ImVec2(GetContentRegionAvail().x * 0.5f, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX, ImGuiWindowFlags_MenuBar)) {
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
							if (Selectable("Linear (y=ax)")) {
								g.model->type = ModelType::LINEAR;
								g.model->expr_str = y + "=a*" + x;
								g.model->refresh();
							}
							if (Selectable("Affine (y=ax+b)")) {
								g.model->type = ModelType::AFFINE;
								g.model->expr_str = y + "=a*" + x + "+b";
								g.model->refresh();
							}
							if (Selectable("Quadratic (y=ax^2+bx+c)")) {
								g.model->type = ModelType::QUADRATIC;
								g.model->expr_str = y + "=a*" + x + "^2+b*" + x + "+c";
								g.model->refresh();
							}
							if (Selectable("Cubic (y=ax^3+bx^2+cx+d)")) {
								g.model->type = ModelType::CUBIC;
								g.model->expr_str = y + "=a*" + x + "^3+b*" + x + "^2+c*x+d";
								g.model->refresh();
							}
							if (Selectable("Sine (y=a*sin(bx+c)")) {
								g.model->type = ModelType::SINUS;
								g.model->expr_str = y + "=a*sin(b*" + x + "+c)";
								g.model->refresh();
							}
							if (Selectable("Logarithmic Base 10 (y=a*log10(x)+b)")) {
								g.model->type = ModelType::LOG10;
								g.model->expr_str = y + "=a*log10(" + x + ")+b";
								g.model->refresh();
							}
							if (Selectable("Neperian Logarithmic (y=a*log(x)+b)")) {
								g.model->type = ModelType::LOG;
								g.model->expr_str = y + "=a*log(" + x + ")+b";
								g.model->refresh();
							}
							if (Selectable("Logarithmic Base n (y=a*logn(x,n)+b)")) {
								g.model->type = ModelType::LOGN;
								g.model->expr_str = y + "=a*logn(" + x + ",n)+b";
								g.model->refresh();
							}
							if (Selectable("Exponential (y=a*exp(b*x)")) {
								g.model->type = ModelType::EXP;
								g.model->expr_str = y + "=a*exp(b*" + x + ")";
								g.model->refresh();
								//e = 2.71828
							}
							if (Selectable("Exponential Base n (y=a*n^(b*x)")) {
								g.model->type = ModelType::EXPN;
								g.model->expr_str = y + "=a*n^(b*" + x + ")";
								g.model->refresh();
							}
							if (Selectable("Square Root (y=a*sqrt(x)+c)")) {
								g.model->type = ModelType::SQRT;
								g.model->expr_str = y + "=a*sqrt(" + x + ")+c";
								g.model->refresh();
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
							InputText("##model", &g.model->expr_str);
						}
						else if (g.model->type == EXPN || g.model->type == LOGN) {
							InputText("##model", &g.model->expr_str, ImGuiInputTextFlags_ReadOnly);
							Text("n = ");
							SameLine();
							InputDouble("##baseN", &g.model->n);
						}
						else {
							InputText("No blank", &g.model->expr_str, ImGuiInputTextFlags_ReadOnly);
						}
						TextColored(ImVec4(0.5, 0.5, 0.5, 1), "(?)");
						if (IsItemHovered()) {
							SetTooltip("Select a model from the options above. \n The computer will automaticly determine the values of a, c and b. \n Warning : \n - The experimental data musn't reverse the order of the model \n - Avoid dividing by 0 (ex: x/a) \n - Sketchy custom models will make the software crash \n - Any unfit model for your experimental data may cause errors \n - The constant e (2.7...) is not usable in custom models");
						}

						if (Button("Update model")) {
							if (g.model->expr_str.size() == 0) {
								model_text = "empty model";
							}
							else if (g.model->expr_str == g.xHeader->name + "=" + x) {
								model_text = "invalid model";

							}
							else if (g.model->type == AFFINE) {
								if (Regression::affine(g.xHeader->values, g.model->dataset->header->values, g.model->b, g.model->a)) {
									Model* m = g.model;
									m->values = {};
									for (int i = 0; i < g.xHeader->values.size(); i++) {
										m->values.push_back(m->value(g.xHeader->values[i]));
									}
									model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b);
								}
							}
							else if (g.model->type == LINEAR) {
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
							else if (g.model->type == LOG10) {
								g.model->n = 10;
								if (Regression::logarithmic(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->n)) {
									Model* m = g.model;
									m->values = {};
									for (int i = 0; i < g.xHeader->values.size(); i++) {
										m->values.push_back(m->value(g.xHeader->values[i]));
									}
									model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n";
								}
							}
							else if (g.model->type == LOG) {
								g.model->n = 1;
								if (Regression::logarithmic(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->n)) {
									Model* m = g.model;
									m->values = {};
									for (int i = 0; i < g.xHeader->values.size(); i++) {
										m->values.push_back(m->value(g.xHeader->values[i]));
									}
									model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n";
								}
							}
							else if (g.model->type == LOGN) {
								if (Regression::logarithmic(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->n)) {
									Model* m = g.model;
									m->values = {};
									for (int i = 0; i < g.xHeader->values.size(); i++) {
										m->values.push_back(m->value(g.xHeader->values[i]));
									}
									model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n";
								}
							}
							else if (g.model->type == EXP) {
								g.model->n = 2.71828;
								if (Regression::exponential(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->n)) {
									Model* m = g.model;
									m->values = {};
									for (int i = 0; i < g.xHeader->values.size(); i++) {
										m->values.push_back(m->value(g.xHeader->values[i]));
									}
									model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n";
								}
							}
							else if (g.model->type == EXPN) {
								if (Regression::exponential(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->n)) {
									Model* m = g.model;
									m->values = {};
									for (int i = 0; i < g.xHeader->values.size(); i++) {
										m->values.push_back(m->value(g.xHeader->values[i]));
									}
									model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n";
								}
							}
							else if (g.model->type == SQRT) {
								if (Regression::sqrt(g.xHeader->values, g.model->dataset->header->values, g.model->a, g.model->b, g.model->c)) {
									Model* m = g.model;
									m->values = {};
									for (int i = 0; i < g.xHeader->values.size(); i++) {
										m->values.push_back(m->value(g.xHeader->values[i]));
									}
									model_text = "a = " + std::to_string(g.model->a) + "\n" + "b = " + std::to_string(g.model->b) + "\n" + "b = " + std::to_string(g.model->c) + "\n";
								}
							}
							else {

							}
							Model* m = g.model;
							if (m->values.size() > 0) {
								m->u = 0;
								for (int i = 0; i < m->values.size(); i++) {
									m->u += std::abs((m->values[i] - m->dataset->header->values[i]) / m->dataset->header->values[i]);
								}
								m->u /= m->values.size();
								m->u *= 100;
							}
							model_text += "\nMean-relative error: " + std::to_string(m->u) + "%";
						}
						TextUnformatted(model_text.c_str());

						EndChild();
					}
					SameLine();
					if (ImPlot::BeginPlot("Graph", ImVec2(-1, -1))) {
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
							ImPlot::SetNextLineStyle(ImPlot::GetColormapColor(g.lines.size() + 1, this->settings->get_int("graphs_cmap")));
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
									double x = g.limits.Min().x + (idx / 100.0f) * abs(g.limits.Max().x - g.limits.Min().x);
									return ImPlotPoint(x, g.model->value(x));
									}, (void*)&g, 101);
							}
						}
						ImPlot::EndPlot();
					}
					EndChild();
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