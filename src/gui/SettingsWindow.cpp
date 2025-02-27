#include <gui/SettingsWindow.h>

#include <App.h>
#include <core/imgui_extension.h>
#include <core/Settings.h>
#include <FontManager.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <iostream>
#include <nfd.hpp>
#include <Utils.h>

using namespace ImGui;

SettingsWindow::SettingsWindow() : local_settings(local_settings) {
	this->name = "Settings";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoSavedSettings;
	this->settings = Settings::instance;
	this->local_settings = Settings(*this->settings);
	this->applied = true;
}

void SettingsWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void SettingsWindow::onDetach() {

}

void SettingsWindow::onPreRender() {
	PushFont(this->font23);
}

void SettingsWindow::onPostRender() {
	PopFont();
}

void SettingsWindow::onRender() {
	FontManager& fm = *FontManager::getInstance();
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);
	SetWindowSize(ImVec2(1200.0f, 500.0f));

	if (BeginTabBar("stats_tabs")) {
		if (BeginTabItem("General")) {
			if (BeginChild("Styles", ImVec2(0, GetContentRegionAvail().y - 35), ImGuiChildFlags_Border)) {
				HeaderText("General", "font64b", 0.5f);
				Text("General options.");
				Dummy(ImVec2(0, 15));
				Separator();
				Dummy(ImVec2(0, 15));

				EndChild();
			}
			EndTabItem();
		}
		if (BeginTabItem("Styles")) {
			if (BeginChild("Styles", ImVec2(0, GetContentRegionAvail().y - 35), ImGuiChildFlags_Border)) {
				HeaderText("Styles", "font64b", 0.5f);
				Text("All options related to DataMorph's appearance is here.");
				Dummy(ImVec2(0, 15));
				Separator();
				Dummy(ImVec2(0, 15));

				HeaderText("Main theme", "font64b", 0.4f);
				TextWrapped("Choose between a dark or light theme.\nNote : you should restart DataMorph in order for the changes to apply.");
				if (BeginCombo("##Main theme", this->local_settings.get_string("theme").c_str())) {
					if (Selectable("Dark")) {
						local_settings.set_string("theme", "Dark");
						this->applied = false;
					}
					if (Selectable("Light")) {
						local_settings.set_string("theme", "Light");
						this->applied = false;
					}
					EndCombo();
				}
				Dummy(ImVec2(0, 10));
				Separator();
				Dummy(ImVec2(0, 10));
				HeaderText("Graph plots colormap", "font64b", 0.4f);
				Text("Choose a colormap to use for plot lines' default colors.");
				int cmap = local_settings.get_int("graphs_cmap");
				if (BeginCombo("##Lines color map", ImPlotColormapToString(cmap))) {
					for (int i = 0; i < 16; i++) {
						if (Selectable(ImPlotColormapToString(i))) {
							local_settings.set_int("graphs_cmap", i);
							this->applied = false;
						}
					}
					EndCombo();
				}
				if (BeginChild("##Colors", ImVec2(0, 40), ImGuiWindowFlags_HorizontalScrollbar, ImGuiWindowFlags_NoResize)) {
					for (int i = 0; i < ImPlot::GetColormapSize(); i++) {
						ColorButton(("Color #" + std::to_string(i)).c_str(), ImPlot::GetColormapColor(i, cmap));
						SameLine();
					}
					EndChild();
				}
				EndChild();
			}
			EndTabItem();
		}
		if (BeginTabItem("Python")) {
			if (BeginChild("Python", ImVec2(0, GetContentRegionAvail().y - 35), ImGuiChildFlags_Border)) {
				HeaderText("Python", "font64b", 0.5f);
				Text("Manage all settings related to Python here.");
				Dummy(ImVec2(0, 15));
				Separator();
				Dummy(ImVec2(0, 15));

				HeaderText("Python interpreter path", "font64b", 0.4f);
				TextWrapped("Enter path to a Python interpreter.");
				if (InputText("##Python interpreter path", &local_settings.get_string("python_exec_path"))) {
					this->applied = false;
				}
				SameLine();
				if (Button("Browse...")) {
					NFD::UniquePath outPath;
					outPath;

					nfdfilteritem_t filterItem[1] = { {"Executable", "exe"} };

					nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1, "C:\\Users");
					if (result == NFD_OKAY) {
						local_settings.set_string("Python executable path", outPath.get());
						this->applied = false;
					}
					else if (result == NFD_CANCEL) {
						std::cout << "User pressed cancel." << std::endl;
					}
					else {
						std::cout << "Error: " << NFD::GetError() << std::endl;
					}
				}
				Dummy(ImVec2(0, 10));
				Separator();
				Dummy(ImVec2(0, 10));
				HeaderText("Keywords highlight color", "font64b", 0.4f);
				TextWrapped("Choose a color to highlight Python keywords");
				ColorEdit3("Keywords color", &local_settings.get_vec("py_keyw_highlight_c").x);
				if (BeginChild("##Highlighexample", ImVec2(0, 65), ImGuiChildFlags_Border)) {
					ImVec4 col = local_settings.get_vec("py_keyw_highlight_c");
					PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
					PushFont(fm.getFont("code23"));
					TextColored(col, "for"); SameLine(); Text(" i "); SameLine(); TextColored(col, "in "); SameLine(); Text("range(3):");
					Text("    print(i)");

					PopFont();
					PopStyleVar();
				}
				EndChild();
				Dummy(ImVec2(0, 10));
				Separator();
				Dummy(ImVec2(0, 10));
				HeaderText("Functions definition", "font64b", 0.4f);
				TextWrapped("Choose a color to highlight Python function definitions.");
				ColorEdit3("Function definition color", &local_settings.get_vec("py_func_def_c").x);
				if (BeginChild("##Funcdefexample", ImVec2(0, 65), ImGuiChildFlags_Border)) {
					ImVec4 col = local_settings.get_vec("py_func_def_c");
					PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
					PushFont(fm.getFont("code23"));
					TextColored(col, "def"); SameLine(); Text(" hello_world(int i): ");
					Text("    print(i)");

					PopFont();
					PopStyleVar();
				}
				EndChild();
				Dummy(ImVec2(0, 10));
				Separator();
				Dummy(ImVec2(0, 10));
				HeaderText("Comments", "font64b", 0.4f);
				TextWrapped("Choose a color to highlight Python comments.");
				ColorEdit3("Comment color", &local_settings.get_vec("py_comment_c").x);
				if (BeginChild("##commentexample", ImVec2(0, 40), ImGuiChildFlags_Border)) {
					ImVec4 col = local_settings.get_vec("py_comment_c");
					PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
					PushFont(fm.getFont("code23"));
					TextColored(col, "# This is a comment.");

					PopFont();
					PopStyleVar();
				}
				EndChild();
				Dummy(ImVec2(0, 10));
				Separator();
				Dummy(ImVec2(0, 10));
				HeaderText("Strings", "font64b", 0.4f);
				TextWrapped("Choose a color to highlight Python strings.");
				ColorEdit3("String color", &local_settings.get_vec("py_str_c").x);
				if (BeginChild("##strexample", ImVec2(0, 40), ImGuiChildFlags_Border)) {
					ImVec4 col = local_settings.get_vec("py_str_c");
					PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
					PushFont(fm.getFont("code23"));
					TextColored(col, "\"This is a comment.\"");

					PopFont();
					PopStyleVar();
				}
				EndChild();
			}
			EndChild();
			EndTabItem();
		}
		EndTabBar();
		if (Button("Ok")) {
			this->p_open = false;
		}
		SameLine();
		BeginDisabled(this->applied);
		{
			if (Button("Apply")) {
				*Settings::instance = local_settings;
				Settings::instance->write_options();
				this->applied = true;

				if (this->settings->get_string("theme") == "Dark") {
					ImGui::StyleColorsDark();
				}
				else {
					ImGui::StyleColorsLight();
				}
			}
		}
		EndDisabled();
	}
}

void SettingsWindow::message(std::string header, ...) {
}