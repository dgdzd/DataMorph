#include <App.h>

#include <FontManager.h>
#include <implot.h>
#include <iostream>
#include <nfd.hpp>
#include <random>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>

DataMorph* DataMorph::inst;
Settings* DataMorph::settings;
I18n DataMorph::i18n = I18n();

DataMorph::DataMorph() {
	this->name = "DataMorph";
	this->version = "0.0.1";
	this->inst = nullptr;
	this->shouldClose = false;
	this->ftlib = nullptr;
	this->window = nullptr;
	this->settings = nullptr;
	srand(10e5 * getTime());
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

	this->window = glfwCreateWindow(1, 1, this->name.c_str(), NULL, NULL);

	if (!this->window) {
		std::cout << "Failed to initialize window\n";
		return -1;
	}

	glfwMakeContextCurrent(this->window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD.\n";
		return 0;
	}

	IMGUI_CHECKVERSION();

	if (!ImGui::CreateContext()) {
		std::cout << "Failed to initialize ImGui.\n";
		return -1;
	}

	if (!ImPlot::CreateContext()) {
		std::cout << "Failed to initialize ImPlot.\n";
		return -1;
	}

	this->io = &ImGui::GetIO();

	this->io->ConfigWindowsMoveFromTitleBarOnly = true;
	this->io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	this->io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	this->baseFont = this->io->Fonts->AddFontFromFileTTF("resources\\fonts\\Segoe UI.ttf", 16.0f, 0, this->io->Fonts->GetGlyphRangesGreek());

	FontManager* fm = FontManager::getInstance();
	fm->loadFontFromPath("resources\\fonts\\Segoe UI.ttf", 20, "font20");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI.ttf", 23, "font23");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI.ttf", 64, "font64");

	fm->loadFontFromPath("resources\\fonts\\Segoe UI b.ttf", 20, "font20b");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI b.ttf", 23, "font23b");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI b.ttf", 64, "font64b");

	fm->loadFontFromPath("resources\\fonts\\Segoe UI i.ttf", 20, "font20i");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI i.ttf", 23, "font23i");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI i.ttf", 64, "font64i");

	fm->loadFontFromPath("resources\\fonts\\Segoe UI bi.ttf", 20, "font20bi");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI bi.ttf", 23, "font23bi");
	fm->loadFontFromPath("resources\\fonts\\Segoe UI bi.ttf", 64, "font64bi");

	fm->loadFontFromPath("resources\\fonts\\JB Mono.ttf", 20, "code20");
	fm->loadFontFromPath("resources\\fonts\\JB Mono.ttf", 23, "code23");
	fm->loadFontFromPath("resources\\fonts\\JB Mono.ttf", 64, "code64");

	fm->loadFontFromPath("resources\\fonts\\JB Mono b.ttf", 20, "code20b");
	fm->loadFontFromPath("resources\\fonts\\JB Mono b.ttf", 23, "code23b");
	fm->loadFontFromPath("resources\\fonts\\JB Mono b.ttf", 64, "code64b");

	fm->loadFontFromPath("resources\\fonts\\JB Mono i.ttf", 20, "code20i");
	fm->loadFontFromPath("resources\\fonts\\JB Mono i.ttf", 23, "code23i");
	fm->loadFontFromPath("resources\\fonts\\JB Mono i.ttf", 64, "code64i");

	fm->loadFontFromPath("resources\\fonts\\JB Mono bi.ttf", 20, "code20bi");
	fm->loadFontFromPath("resources\\fonts\\JB Mono bi.ttf", 23, "code23bi");
	fm->loadFontFromPath("resources\\fonts\\JB Mono bi.ttf", 64, "code64bi");

	fm->loadFontFromPath("resources\\fonts\\Times New Roman.ttf", 23, "math23");

	this->io->Fonts->Build();

	this->settings = new Settings("options.opt");
	Settings::instance = this->settings;
	i18n = I18n(this->settings->get_int("lang"));

	if (this->settings->get_string("theme") == "dark") {
		ImGui::StyleColorsDark();
	}
	else {
		ImGui::StyleColorsLight();
	}

	this->defaultStyle = ImGui::GetStyle();
	defaultStyle.FrameRounding = 3.0f;
	defaultStyle.FramePadding = ImVec2(5.0f, 5.0f);
	defaultStyle.WindowRounding = 10.0f;
	defaultStyle.ChildRounding = 3.0f;
	defaultStyle.TabRounding = 3.0f;

	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

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
	glfwWindowHint(GLFW_DECORATED, false);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
	glfwWindowHint(GLFW_SAMPLES, 4);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	if (FT_Init_FreeType(&ftlib)) {
		std::cout << "Failed to initialize Freetype.\n";
		return -1;
	}

	NFD::Guard guard;

	return 0;
}

Window* DataMorph::getLayer(int id) {
	return this->layers[id];
}

void DataMorph::update() {
	this->shouldClose = glfwWindowShouldClose(this->window);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Adding new windows
	for (Window* layer : this->m_toAdd) {
		this->layers.push_back(layer);
		layer->onAttach();
	}
	this->m_toAdd.clear();

	// GUI Rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (Window* layer : this->layers) {
		ImGuiStyle baseStyle = ImGui::GetStyle();
		*(&ImGui::GetStyle()) = this->defaultStyle;
		if (!layer->p_open) {
			this->layers.erase(std::remove(this->layers.begin(), this->layers.end(), layer), this->layers.end());
			layer->onDetach();
			continue;
		}
		layer->onPreRender();
		if (ImGui::Begin(layer->name.c_str(), layer->showCloseButton ? &layer->p_open : (bool*)0, layer->wflags)) {
			layer->onRender();
		}
		ImGui::End();
		layer->onPostRender();
		*(&ImGui::GetStyle()) = baseStyle;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (this->io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(this->window);
	}

	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

void DataMorph::terminate() {
	Settings::instance->write_options();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
	glfwTerminate();
}

void DataMorph::addLayer(Window* layer) {
	this->m_toAdd.push_back(layer);
}

void DataMorph::setIcon(const char* path) {
	GLFWimage icon;
	icon.pixels = stbi_load(path, &icon.width, &icon.height, 0, 4);
	glfwSetWindowIcon(this->window, 1, &icon);
	stbi_image_free(icon.pixels);
}

double DataMorph::getTime() {
	return glfwGetTime();
}
