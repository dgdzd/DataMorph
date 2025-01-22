#ifndef DM_LAYER_H
#define DM_LAYER_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <string>

class Frame {
public:
	std::string name;
	bool p_open;
	ImGuiStyle style;
	ImGuiWindowFlags wflags;

	virtual void onAttach() = 0;
	virtual void onDetach() = 0;
	virtual void onPreRender() = 0;
	virtual void onPostRender() = 0;
	virtual void onRender() = 0;
};

#endif