#include <app.h>
#include <iostream>

DataMorph* DataMorph::inst;

DataMorph::DataMorph() {
	this->name = "DataMorph";
	this->version = "0.0.1";
	this->inst = nullptr;
	this->shouldClose = false;
	this->ftlib = nullptr;
	this->window = nullptr;
}

DataMorph* DataMorph::getInstance() {
	if (!inst) {
		inst = new DataMorph();
	}

	return inst;
}

int DataMorph::initialize() {
	int code = init_libs();
	if (code != 0) {
		return code;
	}

	this->window = glfwCreateWindow(800, 500, this->name.c_str(), NULL, NULL);

	if (!this->window) {
		std::cout << "Failed to initialize window\n";
		return -1;
	}

	glfwMakeContextCurrent(this->window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD.";
		return 0;
	}

	/* Initializes the viewport */
	glViewport(0, 0, 800, 500);

	return 0;
}

int DataMorph::init_libs() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW.\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, false);
	glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_SAMPLES, 4);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	if (FT_Init_FreeType(&ftlib)) {
		std::cout << "Failed to initialize Freetype.";
		return -1;
	}

	return 0;
}

void DataMorph::update() {
	this->shouldClose = glfwWindowShouldClose(this->window);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void DataMorph::terminate() {
	glfwTerminate();
}