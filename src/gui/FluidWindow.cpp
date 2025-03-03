#include <gui/FluidWindow.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <App.h>
#include <FontManager.h>
#include <imgui_stdlib.h>
#include <Utils.h>

using namespace ImGui;

FluidWindow* FluidWindow::current = nullptr;

FluidWindow::FluidWindow() {
	this->name = "Python IDE";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoSavedSettings;
	this->settings = Settings::instance;

	FluidWindow::current = this;
}

void FluidWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void FluidWindow::onDetach() {
	FluidWindow::current = nullptr;
}

void FluidWindow::onPreRender() {
	PushFont(this->font23);
}

void FluidWindow::onPostRender() {
	PopFont();
}

void FluidWindow::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);
	SetWindowSize(ImVec2(1200.0f, 800.0f));

	if (BeginMenuBar()) {
		if (BeginMenu("File")) {
			if (MenuItem("Save", "Ctrl+S")) {}
			if (MenuItem("Load", "Ctrl+O")) {}
			if (MenuItem("Close")) {
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
		EndMenuBar();
	}
}

void FluidWindow::message(std::string header, ...) {
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