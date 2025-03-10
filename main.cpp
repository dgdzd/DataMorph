﻿// DataMorph.cpp : définit le point d'entrée de l'application.
//
#include <iostream>
#include <App.h>
#include <GLFW/glfw3.h>
#include <gui/MainWindow.h>
#include <gui/UpdateWindow.h>
#include <Utils.h>

int main()
{
	DataMorph* app = DataMorph::getInstance();
	app->setIcon("resources\\icon.ico");
	if (app->initialize() != 0) {
		std::cout << "Error while initializing the app.\n";
		return -1;
	}

	app->addLayer(new MainWindow());
	app->addLayer(new UpdateWin());

	while (!app->shouldClose) {
		app->update();
	}

	app->terminate();
	return 0;
}