#ifndef DM_FLUIDWINDOW_H
#define DM_FLUIDWINDOW_H

#include <App.h>
#include <core/Framebuffer.h>
#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <core/Project.h>
#include <core/Shader.h>
#include <GLFW/glfw3.h>
#include <imgui_internal.h>
#include <string>
#include <utility>
#include <vector>

class FluidWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Framebuffer* fb;
	Shader* shader;

public:
	static FluidWindow* current;
	//truc de opengl
	const GLuint WIDTH = 800;
	const GLint HEIGHT = 800;

	GLFWwindow* window;

	FluidWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
	void frame();
};
#endif
