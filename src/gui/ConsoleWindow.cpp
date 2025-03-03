#include <gui/ConsoleWindow.h>

#include <FontManager.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <iostream>

using namespace ImGui;

ConsoleWindow* ConsoleWindow::current = nullptr;

ConsoleWindow::ConsoleWindow() {
	this->name = "Console";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->mainConsole = new Console("Main", std::cout);

	ConsoleWindow::current = this;
}

void ConsoleWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
	mainConsole->use();
}

void ConsoleWindow::onDetach() {
	mainConsole->unuse();
	ConsoleWindow::current = nullptr;
}

void ConsoleWindow::onPreRender() {
	PushFont(this->font23);
}

void ConsoleWindow::onPostRender() {
	PopFont();
}

void ConsoleWindow::onRender() {
	FontManager& fm = *FontManager::getInstance();
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);

	SetWindowSize(ImVec2(800.0f, 550.0f));

	PushClipRect(titlebar.Min, titlebar.Max, false); // Start editing titlebar area

	PopClipRect(); // Stop editing titlebar area

	if (BeginTabBar("Consoles", ImGuiTabBarFlags_Reorderable)) {
		if (BeginTabItem("Main console")) {
			this->content = this->mainConsole->redirect_stream.str();
			PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			PushFont(fm.getFont("code23"));
			InputTextMultiline("##consoleout", &content, GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
			PopFont();
			PopStyleColor();
			EndTabItem();
		}
		EndTabBar();
	}
}

void ConsoleWindow::message(std::string header, ...) {
	// TODO
}