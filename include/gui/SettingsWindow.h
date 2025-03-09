#ifndef DM_SETTINGSWINDOW_H
#define DM_SETTINGSWINDOW_H

#include <App.h>
#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <imgui_internal.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace ImGui;

class SettingsWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Settings local_settings;
	bool applied;

public:
	static SettingsWindow* current;
	I18n& i18n;

	SettingsWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif