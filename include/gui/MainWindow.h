#ifndef DM_MAINWINDOW_H
#define DM_MAINWINDOW_H

#include <exprtk.hpp>
#include <core/gui/Window.h>
#include <core/imgui_extension.h>
#include <App.h>
#include <imgui_internal.h>
#include <string>
#include <map>
#include <iostream>

using namespace ImGui;

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>   expression_t;
typedef exprtk::parser<double>       parser_t;

struct Header {
	std::string name;
	std::string unit;
	const char* expression;
	std::vector<double> values;

	Header() {
		this->name = "";
		this->unit = "";
		this->expression = "";
		this->values = {};
	}

	Header(std::string name, std::string unit, std::vector<double> values, const char* expression = "") {
		this->name = name;
		this->unit = unit;
		this->expression = expression;
		this->values = values;
	}
};

struct Project {
	std::string name;
	std::string path;
	std::map<std::string, Header> headers;
	std::vector<std::string> symbols;
	std::vector<std::string> units;
	std::map<std::string, std::vector<double>> values;

	Project(std::string name, std::string path) {
		this->name = name;
		this->path = path;
		this->units = {};
		this->symbols = {};
		this->values = {};
	}

	void initValues() {
		for (int i = 0; i < this->symbols.size(); i++) {
			this->values[this->symbols[i]] = { 0.0 };
			this->headers[this->symbols[i]].values = std::vector<double>(1, 0.0);
		}
		
	}

	void addRow() {
		for (int i = 0; i < this->symbols.size(); i++) {
			this->headers[this->symbols[i]].values.push_back(0.0);
		}
	}

	void removeRow() {
		for (int i = 0; i < this->symbols.size(); i++) {
			this->headers[this->symbols[i]].values.pop_back();
		}
	}

	void addColumn(Header* header) {
		const char* expr = header->expression;
		if (expr[0] == '\0') {
			this->headers[header->name] = *header;
			this->headers[header->name].values = std::vector<double>(this->headers[this->symbols[0]].values.size(), 0.0);
		}
		else {
			symbol_table_t symbol_table;
			expression_t expression;
			parser_t parser;
			std::map<std::string, double> symbol_map;
			std::string name = header->name;
			this->headers[name] = *header;

			for (int i = 0; i < this->symbols.size(); i++) {
				std::string symbol = this->symbols[i];
				symbol_map[symbol] = this->headers[symbol].values[0];
				symbol_table.add_variable(symbol, symbol_map[symbol]);
			}

			double i = 0;
			symbol_table.add_variable("i", i);
			expression.register_symbol_table(symbol_table);
			parser.compile(expr, expression);
			
			for (int i = 0; i < this->headers[this->symbols[0]].values.size(); i++) {
				for (int j = 0; j < this->symbols.size(); j++) {
					std::string symbol = this->symbols[j];
					symbol_map[symbol] = this->headers[symbol].values[i];
				}
				this->headers[name].values.push_back(expression.value());
			}
		}
	}

	void removeColumn(std::string name) {
		for (int i = 0; i < this->values.size(); i++) {
			this->headers.erase(name);
		}
	}

	/*void resize_values(int rows, int columns) {
		for (int i; i < values.size(); i++) {
			for (int j; j < values[i].size(); j++) {
				this->values[i][j] = this->values[i][j];
			}
		}
		this->values.resize(columns);
		
		for (int i; i < rows; i++) {
			this->values.resize(rows);
		}
	}*/
};

struct State {
	bool hasOpenProject;
	Project* openProject;
	std::map<std::string, bool> popups;

	State() {
		this->openProject = nullptr;
	}
};

class MainWindow : public Window {
	ImFont* font20;
	ImFont* font23;
	ImFont* font64;
public:
	State* state;

	MainWindow();

	void onAttach() override;
	void onDetach() override;
	void onPreRender() override;
	void onPostRender() override;
	void onRender() override;
	void message(std::string header, ...) override;
};

#endif