#ifndef DM_GRAPH_H
#define DM_GRAPH_H

#include <core/Header.h>
#include <list>
#include <string>

struct Graph {
	int width;
	int height;
	Header* xHeader;
	Header* yHeader;
	std::string name;

	Graph(std::string name, Header* xHeader, Header* yHeader, int width, int height) {
		this->name = name;
		this->xHeader = xHeader;
		this->yHeader = yHeader;
		this->width = width;
		this->height = height;
	}
};

#endif