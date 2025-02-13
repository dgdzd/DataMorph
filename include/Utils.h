#ifndef DM_UTILS_H
#define DM_UTILS_H

#include <string>
#include <implot.h>

namespace std {
	extern string trim_trailing(const string& str, const char c = ' ');
	extern string trim_leading(const string& str, const char c = ' ');
	extern string trim(const string& str, const char c = ' ');
}

extern const char* ImPlotMarkerToString(ImPlotMarker marker);

#endif