#include <gui/GraphWindow.h>

#include <App.h>
#include <cmath>
#include <FontManager.h>
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
	this->statsVar = "";
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
	SetWindowSize(ImVec2(1200.0f, 500.0f));

	if (BeginTabBar("graphs_tabs")) {
		for (int i = 0; i < project->graphs.size(); i++) {
			Graph& g = project->graphs[i];
			if (BeginTabItem(g.name.c_str())) {
				{
					BeginChild("Infos", ImVec2(GetContentRegionAvail().x * 0.5f, 200), ImGuiChildFlags_Borders, ImGuiWindowFlags_MenuBar);
					if (BeginMenuBar()) {
						if (BeginMenu("Infos"))
						{
							ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
					}
					Text("Modelisation : ");
					if (BeginCombo("Choose a model", "None")) {
						std::string x = g.lines[0].header->name;
						if (Selectable("Linear")) {
							strcpy(g.model, (g.xHeader->name + "=a*" + x).c_str());
						}
						if (Selectable("Affine")) {
							strcpy(g.model, (g.xHeader->name + "=a*" + x + "+b").c_str());
						}
						if (Selectable("Polynomial")) {
							strcpy(g.model, (g.xHeader->name + "=a*" + x + "^2+b*" + x + "+c").c_str());
						}
						if (Selectable("Exponential")) {
							strcpy(g.model, (g.xHeader->name + "=a*2.71828^" + g.lines[0].header->name + "+b").c_str());
							//e = 2.71828
						}
						if (Selectable("Logarithmic")) {
							strcpy(g.model, (g.xHeader->name + "=a*log(" + g.lines[0].header->name + ")+b").c_str());
						}
						if (Selectable("Power")) {
							strcpy(g.model, (g.xHeader->name + "=a*" + g.lines[0].header->name + "^b+c").c_str());
						}
						if (Selectable("Sigmoid")) {
							strcpy(g.model, (g.xHeader->name + "=1/(1+2.71828^-" + g.lines[0].header->name + ")").c_str());
							//e = 2.71828
						}
						if (Selectable("Sine")) {
							strcpy(g.model, (g.xHeader->name + "=a*sin(b+" + g.lines[0].header->name + ")").c_str());
						}
						if (Selectable("Cosine")) {
							strcpy(g.model, (g.xHeader->name + "=a*cos(b+" + g.lines[0].header->name + ")").c_str());
						}
						EndCombo();
					}
					InputText("No blank", g.model, 64);
					if (Button("Update model")) {
						if (g.model == "") {

						}
						else if (g.model == g.xHeader->name+"="+g.lines[0].header->name) {

						}
						else {
							typedef exprtk::symbol_table<double> symbol_table_t;
							typedef exprtk::expression<double>   expression_t;
							typedef exprtk::parser<double>       parser_t;

							double a = 0.0;
							double b = 0.0;
							double c = 0.0;

							symbol_table_t symbol_table;
							symbol_table.add_variable("a", a);
							symbol_table.add_variable("b", b);
							symbol_table.add_variable("c", c);
							symbol_table.add_constants();
							for (int j = 0; j < project->values.size(); j++) {
								symbol_table.add_variable(project->symbols[j], project->values[project->symbols[j]][0]);
							}

							expression_t m_e;
							m_e.register_symbol_table(symbol_table);

							parser_t parser;
							int separator_i = std::string(g.model).find("=");
							std::string xheader = std::string(g.model).substr(0, separator_i);
							std::string yModel = std::string(g.model).substr(separator_i + 1);
							parser.compile(yModel, m_e);

							std::vector<double> x_models = {};
							for (int j = 0; j < g.lines[0].header->values.size(); j++) {
								x_models.push_back(g.xHeader->values[j]);
							}

							double loss0 = 0.0;
							double loss1 = 0.0;
							int sign = 1;
							double precision0 = 10e5;
							double precision = precision0;

							if (yModel.find("a") != std::string::npos) {
								for (int j = 0; j < x_models.size(); j++) {
									loss0 += std::abs(m_e.value() - x_models[j]);
								}
								a++;
								for (int j = 0; j < x_models.size(); j++) {
									loss1 += std::abs(m_e.value() - x_models[j]);
								}
								a--;
								if (loss1 - loss0 < 0) {
									sign = -1;
								}
								else {
									sign = 1;
								}
								while (precision > 0.00001) {
									if (sign == 1) {
										while (loss1 - loss0 > 0) {
											loss0 = loss1;
											a += 0.1;
											for (int j = 0; j < x_models.size(); j++) {
												loss1 += std::abs(m_e.value() - x_models[j]);
											}
										}
									}
									else {
										while (loss1 - loss0 > 0) {
											loss0 = loss1;
											a -= 0.1;
											for (int j = 0; j < x_models.size(); j++) {
												loss1 += std::abs(m_e.value() - x_models[j]);
											}
										}
									}

								}
							}
						}
					}

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
					ImPlot::EndPlot();
				}
				if (TreeNode("Statistics")) {
					if (BeginChild("##StatsChild", ImVec2(0, 0), ImGuiChildFlags_Borders)) {
						if (BeginCombo("Var", this->statsVar == "" ? "None" : project->headers[this->statsVar].name.c_str())) {
							if (Selectable("None")) {
								this->statsVar = "";
							}
							for (int i = 0; i < project->headers.size(); i++) {
								if (Selectable(project->headers[project->symbols[i]].name.c_str())) {
									this->statsVar = project->symbols[i];
								}
							}
							EndCombo();
						}
						if (!this->statsVar.empty()) {
							Header* h = &project->headers[this->statsVar];

							// Average
							double average = 0;
							for (auto v : h->values) {
								average += v;
							}
							average /= h->values.size();

							// Standard deviation
							double standard_deviation = 0;
							for (auto v : h->values) {
								standard_deviation += pow(average - v, 2);
							}
							standard_deviation /= h->values.size();
							standard_deviation = sqrt(standard_deviation);

							// Uncertainty
							double uncertainty = standard_deviation / sqrt(h->values.size());

							Text((const char*)u8"%s̅ = %.5f %s", h->name, average, h->unit);
							Text((const char*)u8"σ(%s) = %.5f %s", h->name, standard_deviation, h->unit);
							Text("u(%s) = %.5f %s", h->name, uncertainty, h->unit);
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