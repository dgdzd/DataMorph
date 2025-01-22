// DataMorph.cpp : définit le point d'entrée de l'application.
//

#include <iostream>
#include <App.h>
#include <GLFW/glfw3.h>
#include <gui/MainFrame.h>
#include <gui/UpdateFrame.h>

int main()
{
	DataMorph* app = DataMorph::getInstance();
	if (app->initialize() != 0) {
		std::cout << "Error while initializing the app.\n";
		return -1;
	}

	DataMorph::getInstance()->addLayer(new MainFrame());
	DataMorph::getInstance()->addLayer(new UpdateFrame());

	while (!app->shouldClose) {
		app->update();
	}

	app->terminate();
	return 0;
}