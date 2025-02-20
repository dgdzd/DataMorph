#ifndef DM_GRAPH_H
#define DM_GRAPH_H

#include <core/Header.h>
#include <implot.h>
#include <vector>
#include <string>



struct Line {
	Header* header;
	bool scatter;
	ImVec4* color;
	ImPlotMarker marker;

	Line() {
		this->header = nullptr;
		this->scatter = false;
		this->color = new ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		this->marker = ImPlotMarker_Square;
	}

	Line(Header* header) {
		this->header = header;
		this->scatter = false;
		this->color = new ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		this->marker = ImPlotMarker_Square;
	}

	Line(Header* header, bool scatter, ImVec4* color, ImPlotMarker marker) {
		this->header = header;
		this->scatter = scatter;
		this->color = color;
		this->marker = marker;
	}

	bool operator==(Line& other) {
		return this->header == other.header && this->scatter == other.scatter && this->color == other.color && this->marker == other.marker;
	}
};

struct Graph {
	int width;
	int height;
	Header* xHeader;
	std::vector<Line> lines;
	std::string name;
	char* model;
	double a;
	double b;
	double c;

	Graph(std::string name, Header* xHeader, std::vector<Line> lines, int width, int height) {
		this->name = name;
		this->xHeader = xHeader;
		this->lines = lines;
		this->width = width;
		this->height = height;
		this->model = new char[64] {};
		this->a = 0.0;
		this->b = 0.0;
		this->c = 0.0;

		typedef exprtk::symbol_table<double> symbol_table_t;
		typedef exprtk::expression<double>   expression_t;
		typedef exprtk::parser<double>       parser_t;

		symbol_table_t symbol_table;
		symbol_table.add_variable("a", a);
		symbol_table.add_variable("b", b);
		symbol_table.add_variable("c", c);
		for (int i = 0; i < xHeader->parent->symbols.size(); i++) {
			std::string symbol = xHeader->parent->symbols[i];
			symbol_table.add_variable(symbol, xHeader->parent->headers[symbol].values[0]);
		}

		symbol_table.add_constants();

		expression_t m_e;
		m_e.register_symbol_table(symbol_table);

		parser_t parser;
		parser.compile(std::string(this->model), m_e);
	}
};

#endif