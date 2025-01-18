#ifndef DM_APP_H
#define DM_APP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>

class DataMorph {
	static DataMorph* inst;
	FT_Library ftlib;

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
};

#endif