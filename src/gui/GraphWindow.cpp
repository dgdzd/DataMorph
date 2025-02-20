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
					InputText("Model", g.model, 64);
					if (Button("Update model")) {
						//to do
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

							// Uncertainty
							double uncertainty = standard_deviation / sqrt(h->values.size());

							PushFont(FontManager::getInstance()->getFont("math23"));
							Text("average(%s) = %.5f%s", h->name, average, h->unit);
							Text("σ(%s) = %.5f%s", h->name, standard_deviation, h->unit);
							Text("u(%s) = %.5f%s", h->name, uncertainty, h->unit);
							PopFont();
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