#ifndef DM_PROJECT_H
#define DM_PROJECT_H

#include <core/Header.h>
#include <core/Graph.h>
#include <core/Stats.h>
#include <map>
#include <string>
#include <vector>

struct Project {
	std::string name;
	std::string path;
	std::map<std::string, Header> headers;
	std::vector<std::string> symbols;
	std::vector<std::string> units;
	std::map<std::string, std::vector<double>> values;
	std::vector<Graph> graphs;
	std::vector<Stats> stats;

	Project(std::string name, std::string path);

	void initValues();
	void addRow();
	void removeRow();
	void addColumn(Header* header);
	void removeColumn(std::string name);
};

#endif