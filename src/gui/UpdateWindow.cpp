#include <gui/UpdateWindow.h>
#include <FontManager.h>
#include <iostream>

using namespace ImGui;

UpdateWin::UpdateWin() {
	this->name = "What's new";
	this->font20 = nullptr;
	this->font23 = nullptr;
	this->font64 = nullptr;
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoDocking;
}

void UpdateWin::onAttach() {
	FontManager* fm = FontManager::getInstance();
	this->font20 = fm->getFont("font20");
	this->font23 = fm->getFont("font23");
	this->font64 = fm->getFont("font64");
}

void UpdateWin::onDetach() {
	// TODO
}

void UpdateWin::onPreRender() {
	PushFont(this->font23);
}

void UpdateWin::onPostRender() {
	PopFont();
}

void UpdateWin::onRender() {
	const ImGuiWindow* window = GetCurrentWindow();
	const ImRect titlebar = window->TitleBarRect();
	SetWindowFontScale(1.0f);

	SetWindowSize(ImVec2(400.0f, 800.0f), ImGuiCond_FirstUseEver);

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

void UpdateWin::message(std::string header, ...) {
	// TODO
}