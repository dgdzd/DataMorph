#include <gui/UpdateWindow.h>
#include <FontManager.h>
#include <iostream>

using namespace ImGui;

UpdateWindow::UpdateWindow() {
	this->name = "What's new";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoDocking;
}

void UpdateWindow::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void UpdateWindow::onDetach() {
	// TODO
}

void UpdateWindow::onPreRender() {
	PushFont(this->font23);
}

void UpdateWindow::onPostRender() {
	PopFont();
}

void UpdateWindow::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);

	SetWindowSize(ImVec2(400.0f, 800.0f));

	PushClipRect(titlebar.Min, titlebar.Max, false); // Start editing titlebar area

	PopClipRect(); // Stop editing titlebar area

	if (BeginChild("##content", ImVec2(0.0f, 0.0f))) {
		PushFont(this->font64);
		SetWindowFontScale(0.3f);
		if (CollapsingHeader(("What's new in version v" + data_morph::version + ":").c_str())) {
			PopFont();

			PushFont(this->font20);
			SetWindowFontScale(1.0f);

			TextWrapped("Nothing.");
		}
		PopFont();
		PushFont(this->font20);
		SetWindowFontScale(1.0f);
		TextWrapped("Wow, did you really doom-scroll that far ?");
		PopFont();
	}
	EndChild();
}