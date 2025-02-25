#include <gui/SettingsWindow.h>

#include <App.h>
#include <core/Settings.h>
#include <FontManager.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <iostream>
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
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);
	SetWindowSize(ImVec2(1200.0f, 500.0f));

	if (BeginTabBar("stats_tabs")) {
		if (BeginTabItem("General")) {
			Text("General opts");
			EndTabItem();
		}
		if (BeginTabItem("Styles")) {
			if (BeginCombo("Main theme", this->local_settings.get_option("Theme").c_str())) {
				if (Selectable("Dark")) {
					local_settings.set_option("Theme", "Dark");
					this->applied = false;
				}
				if (Selectable("Light")) {
					local_settings.set_option("Theme", "Light");
					this->applied = false;
				}
				EndCombo();
			}
			if (BeginCombo("Lines color map", ImPlotColormapToString(std::stoi(local_settings.options_data["Graphs colormap"])))) {
				for (int i = 0; i < 16; i++) {
					if (Selectable(ImPlotColormapToString(i))) {
						local_settings.set_option("Graphs colormap", std::to_string(i));
						this->applied = false;
					}
				}
				EndCombo();
			}
			EndTabItem();
		}
		if (BeginTabItem("Python")) {
			if (InputText("Python executable path", &local_settings.options_data["Python executable path"])) {
				this->applied = false;
			}
			EndTabItem();
		}
		EndTabBar();
		Separator();
		if (Button("Ok")) {
			this->p_open = false;
		}
		SameLine();
		BeginDisabled(this->applied);
		{
			if (Button("Apply")) {
				Settings::instance->options_data = local_settings.options_data;
				Settings::instance->write_options();
				this->applied = true;

				if (this->settings->get_option("Theme") == "Dark") {
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