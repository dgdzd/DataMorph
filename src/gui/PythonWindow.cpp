# include <gui/PythonWindow.h>

# include <App.h>
# include <FontManager.h>
# include <imgui_stdlib.h>
# include <iostream>
# include <Utils.h>


using namespace ImGui;

PythonWindow::PythonWindow(Project* project) {
	this->project = project;
	this->name = "Python IDE";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoSavedSettings;
	this->editor_text = "# New python file\n\nprint(\"Hello World!\")";
	this->settings = Settings::instance;
}

void PythonWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void PythonWindow::onDetach() {

}

void PythonWindow::onPreRender() {
	PushFont(this->font23);
}

void PythonWindow::onPostRender() {
	PopFont();
}

void PythonWindow::onRender() {
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
		if (BeginMenu("Python")) {
			if (MenuItem("Reset Kernel")) {

			}
			if (MenuItem("Execute")) {

			}
			ImGui::EndMenu();
		}
		EndMenuBar();
	}
	PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	InputTextMultiline("##editor", &editor_text, window->Size);
	PopStyleColor();


}

void PythonWindow::message(std::string header, ...) {
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