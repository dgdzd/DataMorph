#ifndef DM_CONSOLEWINDOW_H
#define DM_CONSOLEWINDOW_H

#include <core/Console.h>
#include <core/gui/Window.h>
#include <vector>

class ConsoleWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
	Console* mainConsole;
	std::vector<Console> consoles;
	std::string content;

public:
	ConsoleWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif