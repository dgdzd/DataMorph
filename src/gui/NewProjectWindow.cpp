#include <gui/NewProjectWindow.h>

#include <App.h>
#include <core/FileHandler.h>
#include <core/imgui_extension.h>
#include <FontManager.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <nfd.hpp>
#include <utility>
#include <Utils.h>

using namespace ImGui;

NewProjectWindow::NewProjectWindow() {
	this->name = "Create a new project";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
	this->settings = Settings::instance;
	this->inputs = { new char[100] {""}, new char[100] {""} };
	this->symbols = { {}, {} };
	this->units = { {}, {} };
	this->graphics = 0;
}

void NewProjectWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void NewProjectWindow::onDetach() {
	
}

void NewProjectWindow::onPreRender() {
	PushFont(this->font23);
}

void NewProjectWindow::onPostRender() {
	PopFont();
}

void NewProjectWindow::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);

	SetWindowSize(ImVec2(900.0f, 500.0f));
	SetWindowFocus();

	int tab = -1;
	if (BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {
		if (BeginTabItem("Manual data entry")) {
			tab = 0;

			SetCurrentFont(this->font64);
			SetWindowFontScale(0.4f);
			Text("Enter name and unit :");

			SetCurrentFont(this->font23);
			SetWindowFontScale(1.0f);

			InputText("Symbol", this->inputs[0], 64);
			InputText("Unit (optional)", this->inputs[1], 64);
			BeginDisabled(this->inputs[0][0] == '\0');
			{
				if (Button("Add/Modify")) {
					auto found_it = std::find(symbols[tab].begin(), symbols[tab].end(), std::string(this->inputs[0]));
					if (found_it == symbols[tab].end()) {
						std::string magnitude = std::string(this->inputs[0]);
						std::string unit = std::string(this->inputs[1]);
						magnitude = std::trim(magnitude);
						unit = std::trim(unit);
						this->symbols[tab].push_back(magnitude);
						this->units[tab].push_back(unit);
					}
					else {
						int i = std::distance(symbols[tab].begin(), found_it);
						units[tab][i] = std::string(this->inputs[1]);
					}
				}
			}
			EndDisabled();
			

			if (BeginChild("##units", ImVec2(0, 100), ImGuiChildFlags_Borders)) {
				for (int i = 0; i < symbols[tab].size(); i++) {
					std::string magnitude = symbols[tab][i];
					std::string unit = units[tab][i];
					Text("%s : %s", magnitude, unit == "" ? "(No unit)" : unit);
					SameLine();

					PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
					PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
					PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.2f, 0.2f, 1.0f));
					if (Button(("Remove##" + std::to_string(i)).c_str())) {
						this->symbols[tab].erase(this->symbols[tab].begin() + i);
						this->units[tab].erase(this->units[tab].begin() + i);
					}
					PopStyleColor();
					PopStyleColor();
					PopStyleColor();
				}
				EndChild();
			}

			TextWrapped("Note : You will be able to modify or add more symbols and units even after the project creation. "
						"Since then, each variable will be assigned a column in a table.");


			EndTabItem();
		}

		if (BeginTabItem("Import")) {
			tab = 1;
			Text("Import a data file (.csv, .txt, .dat)");
			if (Button("Browse")) {
				NFD::UniquePath outPath;
				outPath;

				nfdfilteritem_t filterItem[1] = { {"Data files", "csv,dat,txt"} };

				nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1, "C:\\Users");
				if (result == NFD_OKAY) {
					this->path = outPath.get();
					if (!FileHandler::read_csv_headers(this->path, this->symbols[tab], this->filesize, this->filename)) {
						std::cout << "Failed to open file." << std::endl;
					}
					if (!FileHandler::read_csv_values(this->path, this->values, this->filesize, this->filename)) {
						std::cout << "Failed to open file." << std::endl;
					}
					this->units[tab] = std::vector<std::string>(this->symbols[tab].size(), "");
				}
				else if (result == NFD_CANCEL) {
					std::cout << "User pressed cancel." << std::endl;
				}
				else {
					std::cout << "Error: " << NFD::GetError() << std::endl;
				}
			}

			if (this->symbols[tab].empty()) {
				TextWrapped("Note : The first line of the file must contain the headers of the columns. "
					"Each column will be assigned a variable in the project. "
					"Each row will be assigned a line in the table.");
			}
			else {
				Text("Selected data file : %s", this->filename.c_str());
				Text("File size : %s", std::format_num(this->filesize, "b"));

				if (BeginChild("Imported headers", ImVec2(0, 130), ImGuiChildFlags_Border)) {
					for (int i = 0; i < symbols[tab].size(); i++) {
						std::string& symbol = symbols[tab][i];
						InputText(("##input_" + std::to_string(i)).c_str(), &symbol);
					}
					EndChild();
				}
			}
			EndTabItem();
		}

		if (BeginTabItem("From video (.avi)")) {
			tab = 2;
			Text("idk");
			EndTabItem();
		}

		if (BeginTabItem("From sound (.wav)")) {
			tab = 3;
			Text("idk");
			EndTabItem();
		}

		EndTabBar();

		Text("Enter Project Name");
		InputText("32 characters max", this->project_name, 32, ImGuiInputTextFlags_AlwaysOverwrite);

		BeginDisabled(this->project_name[0] == '\0' || this->symbols.empty());
		{
			if (Button("Create Project")) {
				this->p_open = false;
				std::string m_header = "";
				switch (tab) {
				case 0:
					m_header = "new_project_t1";
					DataMorph::getInstance()->getLayer(0)->message(m_header, std::string(this->project_name), symbols[tab], units[tab]);
					break;
				case 1:
					m_header = "new_project_t2";
					DataMorph::getInstance()->getLayer(0)->message(m_header, std::string(this->project_name), symbols[tab], units[tab], values);
					break;
				case 2:
					m_header = "new_project_t3";
					break;
				case 3:
					m_header = "new_project_t4";
					break;
				default:
					break;
				}
			}
		}
		EndDisabled();
	}
}

void NewProjectWindow::message(std::string header, ...) {
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