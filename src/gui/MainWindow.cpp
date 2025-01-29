#include <gui/MainWindow.h>
#include <gui/NewProjectWindow.h>
#include <FontManager.h>
#include <iostream>

using namespace ImGui;

MainWindow::MainWindow() {
	this->name = "DataMorph";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
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
	SetWindowFontScale(1.0f);

	SetWindowSize(ImVec2(1200.0f, 700.0f));

	if (BeginMenuBar()) {
		if (BeginMenu("File")) {
			if (MenuItem("New project", "Ctrl+N")) {
				DataMorph::getInstance()->addLayer(new NewProjectWindow());
			}
			if (MenuItem("Open project", "Ctrl+O")) {
				this->state->hasOpenProject = true;
				this->state->openProject = new Project();
				this->state->openProject->name = "Open project";
			}
			if (MenuItem("Save project", "Ctrl+S")) {
				if (this->state->hasOpenProject) {
					this->state->openProject->name = "Saved project";
				}
			}
			if (MenuItem("Close project", "Ctrl+Maj+X")) {
				this->state->hasOpenProject = false;
				this->state->openProject = nullptr;
			}
			EndMenu();
		}
		if (BeginMenu("Edit")) {
			if (MenuItem("Undo", "Ctrl+Z")) {}
			if (MenuItem("Redo", "Ctrl+Y")) {}
			Separator();
			if (MenuItem("Cut", "Ctrl+X")) {}
			if (MenuItem("Copy", "Ctrl+C")) {}
			if (MenuItem("Paste", "Ctrl+V")) {}
			EndMenu();
		}
		if (BeginMenu("View")) {
			if (MenuItem("Show log")) {}
			if (MenuItem("Show console")) {}
			EndMenu();
		}
		if (BeginMenu("Help")) {
			if (MenuItem("About")) {}
			EndMenu();
		}
		EndMenuBar();
	}

	if (!this->state->hasOpenProject) {
		this->name = "DataMorph - Inactive";
	}
	else {
		this->name = "DataMorph - " + this->state->openProject->name;
	}

	PushClipRect(titlebar.Min, titlebar.Max, false); // Start editing titlebar area

	PopClipRect(); // Stop editing titlebar area

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