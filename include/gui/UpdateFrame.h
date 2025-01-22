#include <core/gui/Frame.h>
#include <core/imgui_extension.h>
#include <imgui_internal.h>
#include <string>
#include <Constants.h>

using namespace ImGui;

class UpdateFrame : public Frame {
	ImFont* font20;
	ImFont* font26;
	ImFont* font64;
	State* state;

public:
	UpdateFrame() {
		this->name = "What's new";
		this->font20 = ImGui::GetIO().Fonts->AddFontFromFileTTF("..\\resources\\fonts\\Segoe UI.ttf", 20.0f);
		this->font26 = ImGui::GetIO().Fonts->AddFontFromFileTTF("..\\resources\\fonts\\Segoe UI.ttf", 26.0f);
		this->font64 = ImGui::GetIO().Fonts->AddFontFromFileTTF("..\\resources\\fonts\\Segoe UI.ttf", 64.0f);
		this->p_open = true;
		this->style = ImGui::GetStyle();
		this->wflags = ImGuiWindowFlags_NoCollapse;
	}

	void onAttach() override {
		// TODO
	}

	void onDetach() override {
		// TODO
	}

	void onPreRender() override {
		PushFont(this->font26);
	}

	void onPostRender() override {
		PopFont();
	}

	void onRender() override {
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
};