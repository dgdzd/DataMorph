#include <gui/NewProjectWindow.h>

#include <App.h>
#include <FontManager.h>
#include <Utils.h>
#include <iostream>

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
	this->inputs = { new char[100] {""}, new char[100] {""} };
	this->symbols = std::vector<std::string>();
	this->units = std::vector<std::string>();
	this->graphics = 0;
	this->project_name;
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

	if (BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {
		if (BeginTabItem("Manual data entry")) {
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
					auto found_it = std::find(symbols.begin(), symbols.end(), std::string(this->inputs[0]));
					if (found_it == symbols.end()) {
						std::string magnitude = std::string(this->inputs[0]);
						std::string unit = std::string(this->inputs[1]);
						magnitude = std::trim(magnitude);
						unit = std::trim(unit);
						this->symbols.push_back(magnitude);
						this->units.push_back(unit);
					}
					else {
						int i = std::distance(symbols.begin(), found_it);
						units[i] = std::string(this->inputs[1]);
					}
				}
			}
			EndDisabled();
			

			if (BeginChild("##units", ImVec2(0, 100), ImGuiChildFlags_Borders)) {
				for (int i = 0; i < symbols.size(); i++) {
					std::string magnitude = symbols[i];
					std::string unit = units[i];
					Text("%s : %s", magnitude, unit == "" ? "(No unit)" : unit);
					SameLine();

					PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
					PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
					PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.2f, 0.2f, 1.0f));
					if (Button(("Remove##" + std::to_string(i)).c_str())) {
						this->symbols.erase(this->symbols.begin() + i);
						this->units.erase(this->units.begin() + i);
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

		if (BeginTabItem("From video (.avi)")) {
			Text("idk");
			EndTabItem();
		}

		if (BeginTabItem("From sound (.wav)")) {
			Text("idk");
			EndTabItem();
		}

		if (BeginTabItem("Fluid Simulation")) {
			Text("not yet !");
			EndTabItem();
		}

		EndTabBar();

		Text("Enter Project Name");
		InputText("32 characters max", this->project_name, 32, ImGuiInputTextFlags_AlwaysOverwrite);

		BeginDisabled(this->project_name[0] == '\0' || this->symbols.empty());
		{
			if (Button("Create Project")) {
				this->p_open = false;
				DataMorph::getInstance()->getLayer(0)->message("new_project_t1", std::string(this->project_name), symbols, units);
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