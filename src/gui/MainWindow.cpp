#include <gui/MainWindow.h>
#include <gui/NewProjectWindow.h>
#include <FontManager.h>
#include <iostream>
#include <vector>

using namespace ImGui;


// We define this class locally because it is only used in this file
class NewVarPopup : public Window {
	static NewVarPopup* inst;
	MainWindow* parent;
	char* newSymbol;
	char* newUnit;
	char* expression;

public:
	NewVarPopup(MainWindow* parent) {
		this->parent = parent;
		this->name = "Add a column";
		this->p_open = true;
		this->showCloseButton = true;
		this->style = ImGui::GetStyle();
		this->wflags = ImGuiWindowFlags_NoCollapse;
		this->newSymbol = new char[32] {""};
		this->newUnit = new char[32] {""};
		this->expression = new char[32] {""};
	}
	void onRender() override {
		Project* pr = parent->state->openProject;
		if (BeginTabBar("NewVar")) {
			if (BeginTabItem("Name")) {
				Text("Symbol");
				InputText("##1", newSymbol, 32);
				Text("Unit");
				InputText("##2", newUnit, 32);
				EndTabItem();
			}
			if (BeginTabItem("Expression")) {
				Text("Input an Expression : ");
				Text("symbol [i] : ");
				SameLine();
				InputText("no blank", expression, 32);
				EndTabItem();
			}
			EndTabBar();
		}
		
		if (Button("Add")) {
			pr->symbols.push_back(newSymbol);
			pr->units.push_back(newUnit);
			pr->addColumn();
			parent->state->popups[name] = false;
			CloseCurrentPopup();
		}
		SameLine();
		if (Button("Cancel")) {
			parent->state->popups[name] = false;
			CloseCurrentPopup();
		}
		EndPopup();
	}

	static NewVarPopup* getInstance(MainWindow* mw) {
		if (!inst) {
			inst = new NewVarPopup(mw);
		}
		return inst;
	}
};

NewVarPopup* NewVarPopup::inst = nullptr;


MainWindow::MainWindow() {
	this->name = "DataMorph";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
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
	Project* pr = this->state->openProject;
	State* state = this->state;
	SetWindowFontScale(1.0f);

	SetWindowSize(ImVec2(1200.0f, 700.0f));

	if (BeginMenuBar()) {
		if (BeginMenu("File")) {
			if (MenuItem("New project", "Ctrl+N")) {
				DataMorph::getInstance()->addLayer(new NewProjectWindow());
			}
			if (MenuItem("Open project", "Ctrl+O")) {
			}
			if (MenuItem("Save project", "Ctrl+S")) {
			}
			if (MenuItem("Close project", "Ctrl+Maj+X")) {
				this->p_open = false;
			}
			EndMenu();
		}
		if (BeginMenu("Table")) {
			if (MenuItem("Add Variable")) {
				if (pr) {
					state->popups["Add a column"] = true;
				}
			}
			if (MenuItem("Delete Variable")) {
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

	// The context menu (on right click)
	if (BeginPopupContextWindow()) {
		if (MenuItem("Add row")) {
			pr->addRow();
		}
		if (MenuItem("Remove row")) {
			if (pr->values.size() > 0) {
				pr->removeRow();
			}
		}
		EndPopup();
	}

	if (pr) {
		for (auto& [key, value] : state->popups) {
			if (value) {
				OpenPopup(key.c_str());
			}
		}
	}

	NewVarPopup* nvp = NewVarPopup::getInstance(this);
	if (BeginPopupModal(nvp->name.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		nvp->onRender();
	}

	if (!this->state->openProject) {
		this->name = "DataMorph - Inactive";
	}
	else {
		this->name = "DataMorph - " + this->state->openProject->name;
	}
	this->name += "###MainWindow"; // Pour définir un ID constant "MainWindow" pour la fenêtre

	if (!this->state->openProject) {
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
	else {
		Project* pr = this->state->openProject;
		if (BeginTable("##table", pr->symbols.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit, ImVec2(110.0f*pr->symbols.size(), 0.0f))) {
			for (int i = 0; i < pr->symbols.size(); i++) {
				std::string header = pr->symbols[i];
				if (pr->units[i] != "") {
					header += " (in " + pr->units[i] + ")";
				}
				TableSetupColumn(header.c_str(), ImGuiTableColumnFlags_WidthFixed, 100.0f);
			}
			TableHeadersRow();
			for (int i = 0; i < pr->values.size(); i++) {
				TableNextRow();
				for (int j = 0; j < pr->symbols.size(); j++) {
					TableNextColumn();
					SetNextItemWidth(100);
					InputDouble(("##val" + std::to_string(i) + ":" + std::to_string(j)).c_str(), &pr->values[i][j], 0.0, 0.0, "%.6f", ImGuiInputTextFlags_AlwaysOverwrite);
				}
			}
			EndTable();
		}

		if (Button("+")) {
			pr->addRow();
		}
		SameLine();
		if (Button("-")) {
			if (pr->values.size() > 0) {
				pr->removeRow();
			}
		}
	}
}

void MainWindow::message(std::string header, ...) {
	va_list args;
	va_start(args, header);

	if (header == "new_project_t1") {
		std::string name = va_arg(args, std::string);
		Project* project = new Project(name, "");
		project->symbols = va_arg(args, std::vector<std::string>);
		project->units = va_arg(args, std::vector<std::string>);
		project->initValues();
		
		this->state->openProject = project;
		std::cout << "Project created : " << name << std::endl;
	}
	else {
		throw std::invalid_argument("Invalid header : " + header);
	}
}