#ifndef DM_APP_H
#define DM_APP_H

#include <core/gui/Window.h>
#include <core/Settings.h>
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

class DataMorph {
	static DataMorph* inst;
	Settings* settings;
	FT_Library ftlib;
	ImGuiIO* io;
	ImFont* baseFont;
	std::vector<Window*> layers;
	std::vector<Window*> m_toAdd;

	DataMorph();

	int init_libs();

public:
	std::string name;
	std::string version;
	bool shouldClose;
	ImGuiStyle defaultStyle;
	GLFWwindow* window;

	static DataMorph* getInstance();
	static double getTime();
	int initialize();
	Window* getLayer(int id);
	void update();
	void terminate();
	void addLayer(Window* layer);
	void setIcon(const char* path);
};

#endif
