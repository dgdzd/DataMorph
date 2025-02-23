#include <core/Graph.h>

#include <core/Project.h>

Line::Line() {
	this->header = nullptr;
	this->scatter = false;
	this->color = new ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	this->marker = ImPlotMarker_Square;
}

Line::Line(Header* header) {
	this->header = header;
	this->scatter = false;
	this->color = new ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	this->marker = ImPlotMarker_Square;
}

Line::Line(Header* header, bool scatter, ImVec4* color, ImPlotMarker marker) {
	this->header = header;
	this->scatter = scatter;
	this->color = color;
	this->marker = marker;
}

bool Line::operator==(Line& other) {
	return this->header == other.header && this->scatter == other.scatter && this->color == other.color && this->marker == other.marker;
}

Graph::Graph(std::string name, Header* xHeader, std::vector<Line> lines, int width, int height) {
	 this->name = name;
	 this->xHeader = xHeader;
	 this->lines = lines;
	 this->width = width;
	 this->height = height;
	 this->model = new Model("", {}, {});
	}