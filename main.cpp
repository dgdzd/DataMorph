// DataMorph.cpp : définit le point d'entrée de l'application.
//

#include <iostream>
#include <App.h>
#include <GLFW/glfw3.h>
#include <gui/MainWindow.h>
#include <gui/UpdateWindow.h>
#include <Utils.h>
#include <matheval.h>

int main()
{
	DataMorph* app = DataMorph::getInstance();
	app->setIcon("resources\\icon.png");
	if (app->initialize() != 0) {
		std::cout << "Error while initializing the app.\n";
		return -1;
	}

	// Test matheval
	char expression[] = "2*x + 3";
	auto f = evaluator_create(expression);
	std::cout << evaluator_evaluate_x(f, 1) << std::endl;

	app->addLayer(new MainWindow());
	app->addLayer(new UpdateWindow());

	while (!app->shouldClose) {
		app->update();
	}

	app->terminate();
	return 0;
}