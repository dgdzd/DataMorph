#ifndef DM_APP_H
#define DM_APP_H

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>
#include <core/gui/Frame.h>

class DataMorph {
	static DataMorph* inst;
	FT_Library ftlib;
	ImGuiIO* io;
	ImFont* baseFont;
	std::vector<Frame*> layers;

	DataMorph();

	int init_libs();

public:
	std::string name;
	std::string version;
	bool shouldClose;
	GLFWwindow* window;

	static DataMorph* getInstance();
	int initialize();
	void update();
	void terminate();
	void addLayer(Frame* layer);
};

#endif