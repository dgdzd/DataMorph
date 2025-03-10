#include <gui/SettingsWindow.h>

#include <core/Settings.h>
#include <FontManager.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <iostream>
#include <local/I18n.h>
#include <nfd.hpp>
#include <Utils.h>

using namespace ImGui;

SettingsWindow* SettingsWindow::current = nullptr;

SettingsWindow::SettingsWindow() : local_settings(local_settings), i18n(DataMorph::i18n) {
	this->name = "Settings";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoSavedSettings;
	this->settings = DataMorph::settings;
	this->local_settings = Settings(*this->settings);
	this->applied = true;

	SettingsWindow::current = this;
}

void SettingsWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void SettingsWindow::onDetach() {
	SettingsWindow::current = nullptr;
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
	SetWindowSize(ImVec2(1200.0f, 500.0f), ImGuiCond_FirstUseEver);

	if (BeginTabBar("settings")) {
		if (BeginTabItem(i18n.t("general", "general"))) {
			if (BeginChild(i18n.t("general"), ImVec2(0, GetContentRegionAvail().y - 37), ImGuiChildFlags_Border)) {
				HeaderText(i18n.t("general"), "font64b", 0.5f);
				Text(i18n.t("general.desc"));
				Dummy(ImVec2(0, 15));
				Separator();
				Dummy(ImVec2(0, 15));

				HeaderText(i18n.t("general.lang"), "font64b", 0.4f);
				Text(i18n.t("general.lang.desc"));
				int& lang = local_settings.get_int("lang");
				if (BeginCombo("##Lines color map", LangToString(lang).c_str())) {
					for (int i = 0; i < LANG_COUNT; i++) {
						if (Selectable(LangToString(i).c_str(), lang == i)) {
							lang = i;
							this->applied = false;
						}
					}
					EndCombo();
				}
				Dummy(ImVec2(0, 10));

				HeaderText(i18n.t("general.autocheck_updates"), "font64b", 0.4f);
				TextWrapped(i18n.t("general.autocheck_updates.desc"));
				if (Checkbox(i18n.t("general.autocheck_updates"), &local_settings.get_bool("autocheck_updates"))) {
					this->applied = false;
				}

				EndChild();
			}
			EndTabItem();
		}
		if (BeginTabItem(i18n.t("styles", "styles"))) {
			if (BeginChild(i18n.t("styles"), ImVec2(0, GetContentRegionAvail().y - 37), ImGuiChildFlags_Border)) {
				HeaderText(i18n.t("styles"), "font64b", 0.5f);
				Text(i18n.t("styles.desc"));
				Dummy(ImVec2(0, 15));
				Separator();
				Dummy(ImVec2(0, 15));

				HeaderText(i18n.t("styles.main_theme"), "font64b", 0.4f);
				TextWrapped(i18n.t("styles.main_theme.desc"));
				std::string& theme = this->local_settings.get_string("theme");
				if (BeginCombo("##Main theme", i18n.t("styles."+theme))) {
					if (Selectable(i18n.t("styles.dark"), theme == "dark")) {
						theme = "dark";
						this->applied = false;
					}
					if (Selectable(i18n.t("styles.light"), theme == "light")) {
						theme = "light";
						this->applied = false;
					}
					EndCombo();
				}
				Dummy(ImVec2(0, 10));
				Separator();
				Dummy(ImVec2(0, 10));
				HeaderText(i18n.t("styles.graphs_plots_cmap"), "font64b", 0.4f);
				Text(i18n.t("styles.graphs_plots_cmap.desc"));
				int& cmap = local_settings.get_int("graphs_cmap");
				if (BeginCombo("##Lines color map", ImPlotColormapToString(cmap))) {
					for (int i = 0; i < 16; i++) {
						if (Selectable(ImPlotColormapToString(i), cmap == i)) {
							cmap = i;
							this->applied = false;
						}
					}
					EndCombo();
				}
				if (BeginChild("##Colors", ImVec2(0, 40), 0, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize)) {
					for (int i = 0; i < ImPlot::GetColormapSize(); i++) {
						ColorButton(("Color #" + std::to_string(i+1)).c_str(), ImPlot::GetColormapColor(i, cmap));
						SameLine();
					}
					EndChild();
				}
				Dummy(ImVec2(0, 10));
				HeaderText(i18n.t("styles.default_plots_marker"), "font64b", 0.4f);
				Text(i18n.t("styles.default_plots_marker.desc"));
				int& mker = local_settings.get_int("default_marker");
				if (BeginCombo("##Default marker", ImPlotMarkerToString(mker))) {
					for (int i = -1; i < ImPlotMarker_COUNT; i++) {
						if (Selectable(ImPlotMarkerToString(i), mker == i)) {
							mker = i;
							this->applied = false;
						}
					}
					EndCombo();
				}

				EndChild();
			}
			EndTabItem();
		}
		if (BeginTabItem(i18n.t("python", "python"))) {
			if (BeginChild(i18n.t("python"), ImVec2(0, GetContentRegionAvail().y - 37), ImGuiChildFlags_Border)) {
				HeaderText(i18n.t("python"), "font64b", 0.5f);
				Text(i18n.t("python.desc"));
				Dummy(ImVec2(0, 15));
				Separator();
				Dummy(ImVec2(0, 15));

				HeaderText(i18n.t("python.interp_path"), "font64b", 0.4f);
				TextWrapped(i18n.t("python.interp_path.desc"));
				if (InputText("##Python interpreter path", &local_settings.get_string("python_exec_path"))) {
					this->applied = false;
				}
				SameLine();
				if (Button(i18n.t("browse"))) {
					NFD::UniquePath outPath;
					outPath;

					nfdfilteritem_t filterItem[1] = { {"Executable", "exe"} };

					nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1, "C:\\Users");
					if (result == NFD_OKAY) {
						local_settings.set_string("python_exec_path", outPath.get());
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
				*DataMorph::settings = local_settings;
				DataMorph::settings->write_options();
				this->applied = true;

				if (this->settings->get_string("theme") == "dark") {
					ImGui::StyleColorsDark();
					DataMorph::getInstance()->defaultStyle = ImGui::GetStyle();
				}
				else {
					ImGui::StyleColorsLight();
					DataMorph::getInstance()->defaultStyle = ImGui::GetStyle();
				}
				i18n = I18n(this->settings->get_int("lang"));
			}
		}
		EndDisabled();
	}
}

void SettingsWindow::message(std::string header, ...) {
}