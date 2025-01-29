#include <gui/NewProjectWindow.h>
#include <FontManager.h>

using namespace ImGui;

NewProjectWindow::NewProjectWindow() {
	this->name = "Create a new project";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
}

void NewProjectWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void NewProjectWindow::onDetach() {
	DataMorph::getInstance()->shouldClose = true;
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
			Text("idk");
			EndTabItem();
		}
		if (BeginTabItem("From video (.avi)")) {
			Text("i still dk");
			EndTabItem();
		}
		EndTabBar();
	}
}