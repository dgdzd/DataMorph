#ifndef DM_GRAPH_H
#define DM_GRAPH_H

#include <core/Header.h>
#include <implot.h>
#include <core/Model.h>
#include <vector>
#include <string>


struct Line {
	Header* header;
	bool scatter;
	ImVec4* color;
	ImPlotMarker marker;

	Line();
	Line(Header* header);
	Line(Header* header, bool scatter, ImVec4* color, ImPlotMarker marker);

	bool operator==(Line& other);
};

struct Graph {
	int width;
	int height;
	Header* xHeader;
	Model* model;
	std::vector<Line> lines;
	std::string name;
	double a;
	double b;
	double c;

	Graph(std::string name, Header* xHeader, std::vector<Line> lines, int width, int height);
};

#endif