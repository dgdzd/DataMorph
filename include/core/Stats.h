#ifndef DM_STATS_H
#define DM_STATS_H

#include <core/Header.h>
#include <implot.h>
#include <vector>
#include <string>

struct Bar {
	Header* header;
	ImVec4* color;

	Bar() {
		this->header = nullptr;
		this->scatter = false;
		this->color = new ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	Bar(Header* header) {
		this->header = header;
		this->scatter = false;
		this->color = new ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	Bar(Header* header, ImVec4* color) {
		this->header = header;
		this->color = color;
	}

	bool operator==(Line& other) {
		return this->header == other.header && this->color == other.color;
	}
};

struct Stats {
	int width;
	int height;
	Header* xHeader;
	std::Bar bar;
	std::string name;

	Stats(std::string name, Header* xHeader, std::Bar bar, int width, int height) {
		this->name = name;
		this->xHeader = xHeader;
		this->bar = bar;
		this->width = width;
		this->height = height;
	}
};

#endif
