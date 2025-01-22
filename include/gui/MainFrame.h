#include <core/gui/Frame.h>
#include <core/imgui_extension.h>
#include <imgui_internal.h>
#include <string>

using namespace ImGui;

struct Project {
	std::string name;
	std::string path;

	Project() {
		this->name = "";
		this->path = "";
	}
};

struct State {
	bool hasOpenProject;
	Project* openProject;

	State() {
		this->hasOpenProject = false;
		this->openProject = nullptr;
	}
};

class MainFrame : public Frame {
	ImFont* font20;
	ImFont* font26;
	ImFont* font64;
	State* state;

public:
	MainFrame() {
		this->name = "DataMorph";
		this->font20 = ImGui::GetIO().Fonts->AddFontFromFileTTF("..\\resources\\fonts\\Segoe UI.ttf", 20.0f);
		this->font26 = ImGui::GetIO().Fonts->AddFontFromFileTTF("..\\resources\\fonts\\Segoe UI.ttf", 26.0f);
		this->font64 = ImGui::GetIO().Fonts->AddFontFromFileTTF("..\\resources\\fonts\\Segoe UI.ttf", 64.0f);
		this->p_open = true;
		this->style = ImGui::GetStyle();
		this->wflags = ImGuiWindowFlags_NoCollapse;
		this->state = new State();
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

		SetWindowSize(ImVec2(1200.0f, 700.0f));

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
};