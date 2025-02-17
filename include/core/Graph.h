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

	Graph(std::string name, Header* xHeader, std::vector<Line> lines, int width, int height) {
		this->name = name;
		this->xHeader = xHeader;
		this->lines = lines;
		this->width = width;
		this->height = height;
		this->model = new char[64] {};
	}
};

#endif