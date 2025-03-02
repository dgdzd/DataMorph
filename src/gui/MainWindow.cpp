#include <gui/MainWindow.h>

#include <core/Expression.h>
#include <core/Graph.h>
#include <core/Stats.h>
#include <FontManager.h>
#include <functional>
#include <gui/CommonPopups.h>
#include <gui/ConsoleWindow.h>
#include <gui/NewProjectWindow.h>
#include <gui/GraphWindow.h>
#include <gui/PythonWindow.h>
#include <gui/FluidWindow.h>
#include <gui/SettingsWindow.h>
#include <gui/StatsWindow.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <nfd.hpp>
#include <Utils.h>
#include <vector>

using namespace ImGui;


MainWindow::MainWindow() {
	this->name = "DataMorph";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
	this->settings = Settings::instance;
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
			BeginDisabled(!pr);
			{
				if (BeginMenu("Export to...")) {
					if (MenuItem("CSV")) {
						NFD::UniquePath outPath;
						outPath;

						nfdresult_t result = NFD::SaveDialog(outPath, nullptr, 0, "C:\\Users", (this->state->openProject->name + ".csv").c_str());
						if (result == NFD_OKAY) {
							std::string path = outPath.get();
							std::string filename = path.substr(path.find_last_of("\\") + 1);
							std::string file_extension = filename.substr(filename.find_last_of(".") + 1);
							if (file_extension != "csv") {
								path += ".csv";
							}
							std::ofstream file(path);
							if (file.is_open()) {
								for (int i = 0; i < pr->symbols.size(); i++) {
									file << pr->symbols[i] << (i == pr->ids.size() - 1 ? "" : ";");
								}
								file << "\n";
								for (int i = 0; i < pr->headers[pr->ids[0]].values.size(); i++) {
									for (int j = 0; j < pr->ids.size(); j++) {
										unsigned int id = pr->ids[j];
										file << pr->headers[id].values[i] << (j == pr->ids.size() - 1 ? "" : ";");
									}
									file << "\n";
								}
								file.close();
							}
						}
					}
					ImGui::EndMenu();
				}
			}
			EndDisabled();
			Separator();
			if (MenuItem("Settings")) {
				DataMorph::getInstance()->addLayer(new SettingsWindow());
			}
			Separator();
			if (MenuItem("Exit")) {
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
					for (int i = 0; i < pr->symbols.size(); i++) {
						std::string symbol = pr->symbols[i];
						if (MenuItem(symbol.c_str())) {
							pr->removeColumn(pr->ids[i]);
						}
					}
					ImGui::EndMenu();
				}
				if (BeginMenu("Add Row")) {
					if (MenuItem("Add 1 Row")) { 
						pr->addRow(); 
					}
					if (MenuItem("Add 10 Rows")) {
						for (int i = 0; i < 10; i++) {
							pr->addRow();
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (BeginMenu("Graphs & Stats")) {
				if (MenuItem("Manage graphs...")) {
					state->popups["Manage graphs"] = true;
				}
				if (MenuItem("View graphs")) {
					DataMorph::getInstance()->addLayer(new GraphWindow(pr));
				}
				ImGui::EndMenu();
			}
		}
		if (BeginMenu("Tools")) {
			if (MenuItem("Resolve an Equation")) {
				state->popups["Resolve Equation"] = true;
			}
			if (MenuItem("Open Python IDE")) {
				DataMorph::getInstance()->addLayer(new PythonWindow(pr));
			}
			if (MenuItem("Start Fluid Simulation")) {
				FluidWindow* fluid_window = new FluidWindow();
				if (fluid_window->innit()) {
					fluid_window->renderLoop();
				}

			}
			ImGui::EndMenu();
		}
		if (BeginMenu("View")) {
			if (MenuItem("Show console")) {
				DataMorph::getInstance()->addLayer(new ConsoleWindow());
			}
			ImGui::EndMenu();
		}
		if (BeginMenu("Help")) {
			if (MenuItem("About")) {}
			ImGui::EndMenu();
		}
		EndMenuBar();
	}

	if (pr) {
		for (auto& [key, value] : state->popups) {
			if (value) {
				OpenPopup(key.c_str());
			}
		}
	}

	NewVarPopup* nvp = NewVarPopup::getInstance(this);
	if (BeginPopupModal(nvp->name.c_str(), NULL, nvp->wflags)) {
		nvp->onRender();
	}
	if (EditVarPopup::hasInstance()) {
		EditVarPopup* evp = EditVarPopup::getInstance(this, 0);
		if (BeginPopupModal(evp->name.c_str(), NULL, evp->wflags)) {
			evp->onRender();
		}
	}
	NewGraphPopup* ngp = NewGraphPopup::getInstance(this);
	if (BeginPopupModal(ngp->name.c_str(), NULL, ngp->wflags)) {
		ngp->onRender();
	}
	ResolveEquation* re = ResolveEquation::getInstance(this);
	if (BeginPopupModal(re->name.c_str(), NULL, re->wflags)) {
		re->onRender();
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
		if (BeginTable("##table", pr->ids.size() + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX)) {
			for (int i = 0; i < pr->ids.size(); i++) {
				std::string header = pr->symbols[i];
				if (pr->units[i] != "") {
					header += " (in " + pr->units[i] + ")";
				}
				TableSetupColumn(header.c_str(), ImGuiTableColumnFlags_WidthFixed, 150.0f);
			}
			TableSetupColumn("##del", ImGuiTableColumnFlags_WidthFixed, 20.0f);

			TableNextRow();
			for (int i = 0; i < TableGetColumnCount() - 1; i++) {
				if (!TableSetColumnIndex(i)) {
					continue;
				}
				std::string symbol = pr->symbols[i];
				std::string unit = pr->units[i];
				unsigned int id = pr->ids[i];
				Header* header = &pr->headers[id];
				TableHeader((symbol + (unit.empty() ? "" : " (in " + unit + ")")).c_str());

				if (BeginPopupContextItem()) {
					Text((symbol + " (in " + unit + ")").c_str());
					Separator();
					if (MenuItem("Edit...")) {
						EditVarPopup::getInstance(this, id);
						this->state->popups["Edit variable"] = true;
					}
					BeginDisabled(pr->headers.size() < 2);
					{
						if (MenuItem("Remove this column")) {
							pr->removeColumn(id);
						}
					}
					EndDisabled();
					PushItemFlag(ImGuiItemFlags_AutoClosePopups, false);
					if (MenuItem("Locked values", NULL, header->lockedValues)) {
						header->lockedValues = !header->lockedValues;
					}
					PopItemFlag();
					EndPopup();
				}
			}
			if (TableSetColumnIndex(TableGetColumnCount() - 1)) {
				TableHeader("##del");
			}

			for (int i = 0; i < pr->headers[pr->ids[0]].values.size(); i++) {
				TableNextRow();
				for (int j = 0; j < pr->ids.size(); j++) {
					unsigned int id = pr->ids[j];
					Header& header = pr->headers[id];
					std::string symbol = header.name;
					std::string unit = header.unit;
					double& val = header.values[i];
					TableNextColumn();
					SetNextItemWidth(150);
					if (header.expression && header.lockedValues) {
						header.expression->updateValues();
					}

					bool f = header.lockedValues;
					PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					if (f) {
						InputDouble(("##val:" + symbol + ":" + std::to_string(i)).c_str(), &val, 0.0, 0.0, "%.6f", ImGuiInputTextFlags_ReadOnly);
						// CA MARCHE PAS CA CRASH JE SAIS PAS PK
						if (BeginPopupContextItem(("##rctx:" + symbol + ":" + std::to_string(i)).c_str())) {
							Text((symbol + " (in " + unit + ")").c_str());
							Separator();
						}
						// ça, ça marche par contre
						if (IsItemHovered()) {
							SetTooltip("This value is locked.");
						}

					}
					else {
						InputDouble(("##val:" + symbol + ":" + std::to_string(i)).c_str(), &val, 0.0, 0.0, "%.6f");
						// CA MARCHE PAS CETTE MERDZE
						if (IsItemDeactivatedAfterEdit()) {
							if (j == pr->ids.size() - 1 && i == header.values.size() - 1) { // If last cell
								std::cout << "End reached\n";
								pr->addRow(); // Add a new row
							}
						}
					}
					PopStyleColor();
				}

				TableNextColumn();
				SetNextItemWidth(20);
				PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
				PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
				PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.2f, 0.2f, 1.0f));
				if (Button(("X##" + std::to_string(i)).c_str())) {
					pr->removeRow(i);
				}
				PopStyleColor();
				PopStyleColor();
				PopStyleColor();
			}
			EndTable();
		}

		if (Button("+", ImVec2(30, 30))) {
			pr->addRow();
		}
		SameLine();
		if (Button("-", ImVec2(30, 30))) {
			if (pr->headers[pr->ids[0]].values.size() > 1) {
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
			Header h = Header(project, project->symbols[i], project->units[i], { 0.0 });
			project->ids.push_back(h.id);
			project->headers[h.id] = h;
		}
		
		this->state->openProject = project;
		std::cout << "Project created : " << name << std::endl;
	}
	else if (header == "new_project_t2") {
		std::string name = va_arg(args, std::string);
		Project* project = new Project(name, "");
		project->symbols = va_arg(args, std::vector<std::string>);
		project->units = va_arg(args, std::vector<std::string>);
		std::vector<std::vector<double>> values = va_arg(args, std::vector<std::vector<double>>);
		for (int i = 0; i < project->symbols.size(); i++) {
			Header h = Header(project, project->symbols[i], project->units[i], values[i]);
			project->ids.push_back(h.id);
			project->headers[h.id] = h;
		}
		this->state->openProject = project;
		std::cout << "Project created : " << name << std::endl;
	}
	else {
		throw std::invalid_argument("Invalid header : " + header);
	}
}