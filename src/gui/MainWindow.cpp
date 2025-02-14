#include <gui/MainWindow.h>

#include <core/Expression.h>
#include <FontManager.h>
#include <gui/NewProjectWindow.h>
#include <gui/GraphWindow.h>
#include <core/Graph.h>
#include <imgui/imgui_stdlib.h>
#include <iostream>
#include <functional>
#include <Utils.h>
#include <vector>

using namespace ImGui;

// We define this class locally because it is only used in this file
class NewVarPopup : public Window {
	static NewVarPopup* inst;
	MainWindow* parent;
	char* newSymbol;
	char* newUnit;
	char* expression;
	std::pair<int, int> derivate;
	std::pair<float, float> linespace;
	std::vector<int> integral;
	std::vector<float> args;

public:
	NewVarPopup(MainWindow* parent) {
		this->parent = parent;
		this->name = "Add a column";
		this->p_open = true;
		this->showCloseButton = true;
		this->style = ImGui::GetStyle();
		this->wflags = ImGuiWindowFlags_NoCollapse;
		this->newSymbol = new char[32] {""};
		this->newUnit = new char[32] {""};
		this->expression = new char[32] {""};
		this->derivate = {0, 0};
		this->linespace = {0.0f, 0.0f};
		this->integral = {0, 1, 0, 0};
		this->args = {};
		Project* pr = parent->state->openProject;
	}
	void onRender() override {
		Project* pr = parent->state->openProject;
		int tab = -1;
		if (BeginTabBar("NewVar")) {
			if (BeginTabItem("Dummy")) {
				tab = 0;
				Text("Symbol");
				InputText("##Symbol", newSymbol, 32);
				Text("Unit (optional)");
				InputText("##Unit", newUnit, 32);
				EndTabItem();
			}
			if (BeginTabItem("With expression")) {
				tab = 1;
				Text("Symbol");
				InputText("##Symbol", newSymbol, 32);
				Text("Unit (optional)");
				InputText("##Unit", newUnit, 32);
				Separator();

				Text("Input an expression : ");
				Text("%s :", newSymbol[0] == '\0' ? "(Symbol needed)" : (std::string(newSymbol)+"[i]"));
				SameLine();
				InputText("No blank", expression, 32);
				EndTabItem();
			}
			if (BeginTabItem("Derivate")) {
				tab = 2;
				Text("Symbol");
				InputText("##Symbol", newSymbol, 32);
				Text("Unit (optional)");
				InputText("##Unit", newUnit, 32);
				Separator();

				Text("Input a derivative :");
				Text("d");
				SameLine();
				if (BeginCombo("##3", pr->symbols[derivate.first].c_str())) {
					int selected = 0;
					for (int i = 0; i < pr->symbols.size(); i++) {
						bool is_selected = selected == i;
						if (Selectable(pr->symbols[i].c_str(), is_selected)) {
							selected = i;
							derivate.first = i;
						}
						if (is_selected) {
							SetItemDefaultFocus();
						}
					}
					EndCombo();
				}

				Separator();

				Text("dx");
				SameLine();
				if (BeginCombo("##4", pr->symbols[derivate.second].c_str())) {
					int selected = 0;
					for (int i = 0; i < pr->symbols.size(); i++) {
						bool is_selected = selected == i;
						if (Selectable(pr->symbols[i].c_str(), is_selected)) {
							selected = i;
							derivate.second = i;
						}
						if (is_selected) {
							SetItemDefaultFocus();
						}
					}
					EndCombo();
				}
				EndTabItem();
			}
			if (BeginTabItem("Linespace")) {
				tab = 3;
				Text("Symbol");
				InputText("##Symbol", newSymbol, 32);
				Text("Unit (optional)");
				InputText("##Unit", newUnit, 32);
				Separator();

				Text("Input a linespace :");
				Text("from : ");
				SameLine();
				InputFloat("##from", &linespace.first);
				Text("step : ");
				SameLine();
				InputFloat("##to", &linespace.second);
				EndTabItem();
			}
			if (BeginTabItem("Integral")) {
				tab = 4;
				Text("Symbol");
				InputText("##Symbol", newSymbol, 32);
				Text("Unit (optional)");
				InputText("##Unit", newUnit, 32);
				Separator();

				Text("Input an integral :");
				Text("from : ");
				SameLine();
				InputInt("##from2", &integral[0]);
				Text("to : ");
				SameLine();
				InputInt("##to2", &integral[1]);
				Text("integral of : ");
				SameLine();
				if (BeginCombo("##5", pr->symbols[integral[2]].c_str())) {
					int selected = 0;
					for (int i = 0; i < pr->symbols.size(); i++) {
						bool is_selected = selected == i;
						if (Selectable(pr->symbols[i].c_str(), is_selected)) {
							selected = i;
							integral[2] = i;
						}
						if (is_selected) {
							SetItemDefaultFocus();
						}
					}
					EndCombo();
				}
				SameLine();
				Text(" d");
				SameLine();
				if (BeginCombo("##6", pr->symbols[integral[3]].c_str())) {
					int selected = 0;
					for (int i = 0; i < pr->symbols.size(); i++) {
						bool is_selected = selected == i;
						if (Selectable(pr->symbols[i].c_str(), is_selected)) {
							selected = i;
							integral[3] = i;
						}
						if (is_selected) {
							SetItemDefaultFocus();
						}
					}
					EndCombo();
				}

				EndTabItem();
			}
			EndTabBar();
		}

		bool disabled = tab == 1 && expression[0] == '\0';
		BeginDisabled(disabled); 
		{
			if (Button("Add")) {
				if (std::find(pr->symbols.begin(), pr->symbols.end(), newSymbol) != pr->symbols.end()) {
					parent->state->popups["Error###alreadyExists"] = true;
					parent->state->popups[name] = false;
					return;
				}

				ExpressionSpecs* specs = new ExpressionSpecs;
				if (tab == 2) {
					specs = new ExpressionSpecs(DERIVATIVE, &pr->headers[pr->symbols[derivate.second]], &pr->headers[pr->symbols[derivate.first]]);
				}
				else if (tab == 1) {
					specs = new ExpressionSpecs(FORMULA, nullptr, nullptr);
				}
				else if (tab == 3) {
					specs = new ExpressionSpecs(LINESPACE, nullptr, nullptr);
					this->args = {linespace.first, linespace.second};
				}
				else if (tab == 4) {
					specs = new ExpressionSpecs(INTEGRAL, &pr->headers[pr->symbols[derivate.second]], &pr->headers[pr->symbols[derivate.first]]);
				}
				parent->state->popups[name] = false;
				Header* newHeader = new Header(pr, newSymbol, newUnit, {}, tab == 1 ? expression : "", specs, this->args);
				pr->addColumn(newHeader);
				pr->symbols.push_back(newSymbol);
				pr->units.push_back(newUnit);
				CloseCurrentPopup();
			}
		}
		EndDisabled();
		SameLine();
		if (Button("Cancel")) {
			parent->state->popups[name] = false;
			CloseCurrentPopup();
		}
		EndPopup();
	}

	static NewVarPopup* getInstance(MainWindow* mw) {
		if (!inst) {
			inst = new NewVarPopup(mw);
		}
		return inst;
	}
};

NewVarPopup* NewVarPopup::inst = nullptr;

class NewGraphPopup : public Window {
	static NewGraphPopup* inst;
	MainWindow* parent;
	Project* pr;

public:
	NewGraphPopup(MainWindow* parent) {
		this->parent = parent;
		this->name = "Manage graphs";
		this->p_open = true;
		this->showCloseButton = true;
		this->style = ImGui::GetStyle();
		this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
		this->pr = parent->state->openProject;
	}
	void onRender() override {
		Project* pr = parent->state->openProject;
		SetWindowSize(ImVec2(450, 700));

		if (pr->graphs.size() == 0) {
			TextWrapped("There is currently no graph.");
			if (Button("Create one")) {
				Graph g("New graph 1", &pr->headers[pr->symbols[0]], { Line(&pr->headers[pr->symbols[0]]) }, 0, 0);
				pr->graphs.push_back(g);
			}
		}
		else if (BeginTabBar("graphs")) {
			for (int i = 0; i < pr->graphs.size(); i++) {
				Graph& graph = pr->graphs[i];
				if (BeginTabItem(("Graph #" + std::to_string(i+1)).c_str())) {
					Text("Name");
					InputText("##graphName", &graph.name, 32);
					Dummy(ImVec2(0.0f, 10.0f));
					if (BeginCombo("X axis", graph.xHeader->name.c_str())) {
						for (std::string symbol : pr->symbols) {
							if (Selectable(symbol.c_str())) {
								graph.xHeader = &pr->headers[symbol];
							}
						}
						EndCombo();
					}
					Dummy(ImVec2(0.0f, 10.0f));
					Separator();
					Dummy(ImVec2(0.0f, 10.0f));
					Text("Lines");
					for (int j = 0; j < graph.lines.size(); j++) {
						if (TreeNodeEx(("Line #" + std::to_string(j+1)).c_str())) {
							Line& line = graph.lines[j];
							if (BeginCombo("Data to plot", line.header->name.c_str())) {
								for (std::string symbol : pr->symbols) {
									if (Selectable(symbol.c_str())) {
										line.header = &pr->headers[symbol];
									}
								}
								EndCombo();
							}
							Checkbox("Scatter", &line.scatter);
							ColorEdit4("Line color", &line.color->x);
							if (BeginCombo("Marker", ImPlotMarkerToString(line.marker))) {
								for (int i = -1; i < ImPlotMarker_COUNT; i++) {
									if (Selectable(ImPlotMarkerToString(i))) {
										line.marker = i;
									}
								}
								EndCombo();
							}
							if (graph.lines.size() > 1) {
								PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
								PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
								PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.2f, 0.2f, 1.0f));
								if (Button(("Remove###" + std::to_string(j)).c_str())) {
									graph.lines.erase(graph.lines.begin() + j);
								}
								PopStyleColor();
								PopStyleColor();
								PopStyleColor();
							}
							TreePop();
						}
					}
					Dummy(ImVec2(0.0f, 10.0f));
					if (Button("+")) {
						Line l(&pr->headers[pr->symbols[0]]);
						graph.lines.push_back(l);
					}
					EndTabItem();
				}
			}
			if (TabItemButton("+", ImGuiTabItemFlags_Trailing)) {
				Graph g(("New graph " + std::to_string(pr->graphs.size() + 1)), &pr->headers[pr->symbols[0]], { Line(&pr->headers[pr->symbols[0]]) }, 0, 0);
				pr->graphs.push_back(g);
			}
			EndTabBar();
		}
		Dummy(ImVec2(0.0f, 15.0f));
		if (Button("OK")) {
			parent->state->popups[name] = false;
			CloseCurrentPopup();
		}

		EndPopup();
	}
	static NewGraphPopup* getInstance(MainWindow* mw) {
		if (!inst) {
			inst = new NewGraphPopup(mw);
		}
		return inst;
	}
};

NewGraphPopup* NewGraphPopup::inst = nullptr;


MainWindow::MainWindow() {
	this->name = "DataMorph";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
	this->state = new State();
}

void MainWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void MainWindow::onDetach() {
	DataMorph::getInstance()->shouldClose = true;
}

void MainWindow::onPreRender() {
	PushFont(this->font23);
}

void MainWindow::onPostRender() {
	PopFont();
}

void MainWindow::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	Project* pr = this->state->openProject;
	State* state = this->state;
	SetWindowFontScale(1.0f);

	SetWindowSize(ImVec2(1200.0f, 700.0f));

	if (BeginMenuBar()) {
		if (BeginMenu("File")) {
			if (MenuItem("New project...", "Ctrl+N")) {
				DataMorph::getInstance()->addLayer(new NewProjectWindow());
			}
			if (MenuItem("Open project...", "Ctrl+O")) {
			}
			if (MenuItem("Save project", "Ctrl+S")) {
			}
			if (MenuItem("Close project", "Ctrl+Maj+X")) {
				this->p_open = false;
			}
			ImGui::EndMenu();
		}
		if (BeginMenu("Edit")) {
			if (MenuItem("Undo", "Ctrl+Z")) {}
			if (MenuItem("Redo", "Ctrl+Y")) {}
			Separator();
			if (MenuItem("Cut", "Ctrl+X")) {}
			if (MenuItem("Copy", "Ctrl+C")) {}
			if (MenuItem("Paste", "Ctrl+V")) {}
			ImGui::EndMenu();
		}
		if (pr) {
			if (BeginMenu("Table")) {
				if (MenuItem("Add Variable...")) {
					state->popups["Add a column"] = true;
				}
				if (BeginMenu("Delete Variable")) {
					for (std::string symbol : pr->symbols) {
						if (MenuItem(symbol.c_str())) {
							pr->removeColumn(symbol);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (BeginMenu("Graphs")) {
				if (MenuItem("Manage graphs...")) {
					state->popups["Manage graphs"] = true;
				}
				if (MenuItem("View graphs")) {
					DataMorph::getInstance()->addLayer(new GraphWindow(pr));
				}
				ImGui::EndMenu();
			}
		}
		if (BeginMenu("View")) {
			if (MenuItem("Show log")) {}
			if (MenuItem("Show console")) {}
			ImGui::EndMenu();
		}
		if (BeginMenu("Help")) {
			if (MenuItem("About")) {}
			ImGui::EndMenu();
		}
		EndMenuBar();
	}

	// The context menu (on right click)
	if (BeginPopupContextWindow()) {
		if (MenuItem("Add row")) {
			pr->addRow();
		}
		if (MenuItem("Remove row")) {
			if (pr->values.size() > 0) {
				pr->removeRow();
			}
		}
		EndPopup();
	}

	if (pr) {
		for (auto& [key, value] : state->popups) {
			if (value) {
				OpenPopup(key.c_str());
			}
		}
	}

	NewVarPopup* nvp = NewVarPopup::getInstance(this);
	if (BeginPopupModal(nvp->name.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		nvp->onRender();
	}
	NewGraphPopup* ngp = NewGraphPopup::getInstance(this);
	if (BeginPopupModal(ngp->name.c_str(), NULL, ngp->wflags)) {
		ngp->onRender();
	}

	if (BeginPopupModal("Error###alreadyExists", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
		Text("This symbol already exists.");
		if (Button("Ok")) {
			state->popups["Error###alreadyExists"] = false;
			CloseCurrentPopup();
		}
		EndPopup();
	}

	if (!this->state->openProject) {
		this->name = "DataMorph - Inactive";
	}
	else {
		this->name = "DataMorph - " + this->state->openProject->name;
	}
	this->name += "###MainWindow"; // Pour définir un ID constant "MainWindow" pour la fenêtre

	if (!this->state->openProject) {
		PushFont(this->font64);
		SetWindowFontScale(0.5f);
		CenteredText("Welcome to DataMorph");
		PopFont();

		Dummy(ImVec2(0.0f, 50.0f));

		PushFont(this->font20);
		SetWindowFontScale(1.0f);
		Text("Version 0.0.1");
		PopFont();
	}
	else {
		Project* pr = this->state->openProject;
		if (BeginTable("##table", pr->symbols.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit, ImVec2(160.0f * pr->symbols.size(), 0.0f))) {
			for (int i = 0; i < pr->symbols.size(); i++) {
				std::string header = pr->symbols[i];
				if (pr->units[i] != "") {
					header += " (in " + pr->units[i] + ")";
				}
				TableSetupColumn(header.c_str(), ImGuiTableColumnFlags_WidthFixed, 150.0f);
			}
			TableHeadersRow();
			for (int i = 0; i < pr->headers[pr->symbols[0]].values.size(); i++) {
				TableNextRow();
				for (std::string symbol : pr->symbols) {
					Header& header = pr->headers[symbol];
					double& val = header.values[i];
					TableNextColumn();
					SetNextItemWidth(150);
					if (header.expression) {
						header.expression->updateValues();
					}

					bool f = header.expression && header.expression->specs.type != DUMMY;
					if (f) {
						Text("%s", std::to_string(val).c_str());
					}
					else {
						PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
						InputDouble(("##val:" + symbol + ":" + std::to_string(i)).c_str(), &val, 0.0, 0.0, "%.6f", ImGuiInputTextFlags_AlwaysOverwrite);
						PopStyleColor();
					}
				}
			}
			EndTable();
		}

		if (Button("+")) {
			pr->addRow();
		}
		SameLine();
		if (Button("-")) {
			if (pr->headers[pr->symbols[0]].values.size() > 0) {
				pr->removeRow();
			}
		}
	}
}

void MainWindow::message(std::string header, ...) {
	va_list args;
	va_start(args, header);

	if (header == "new_project_t1") {
		std::string name = va_arg(args, std::string);
		Project* project = new Project(name, "");
		project->symbols = va_arg(args, std::vector<std::string>);
		project->units = va_arg(args, std::vector<std::string>);
		for (int i = 0; i < project->symbols.size(); i++) {
			project->headers[project->symbols[i]] = Header(project, project->symbols[i], project->units[i], {});
		}
		project->initValues();
		
		this->state->openProject = project;
		std::cout << "Project created : " << name << std::endl;
	}
	else {
		throw std::invalid_argument("Invalid header : " + header);
	}
}