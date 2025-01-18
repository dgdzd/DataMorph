// DataMorph.cpp : définit le point d'entrée de l'application.
//

#include <iostream>
#include <app.h>
#include <GLFW/glfw3.h>

using namespace std;

int main()
{
	DataMorph* app = DataMorph::getInstance();
	if (app->initialize() != 0) {
		cout << "Error while initializing the app.\n";
		return -1;
	}

	while (!app->shouldClose) {
		app->update();
	}

	app->terminate();
	return 0;
}