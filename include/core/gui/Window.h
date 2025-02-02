#ifndef DM_WINDOW_H
#define DM_WINDOW_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <string>

class Window {
public:
	std::string name;
	bool showCloseButton;
	bool p_open;
	ImGuiStyle style;
	ImGuiWindowFlags wflags;

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
	virtual void onPreRender() = 0;
	virtual void onPostRender() = 0;
	virtual void onRender() = 0;
	virtual void message(std::string header, ...) = 0;
};

#endif