#ifndef DM_UTILS_H
#define DM_UTILS_H

#include <string>
#include <implot.h>
#include <vector>

namespace std {
	extern string trim_trailing(const string& str, const char c = ' ');
	extern string trim_leading(const string& str, const char c = ' ');
	extern string trim(const string& str, const char c = ' ');

	template <typename T>
	extern T min(const std::vector<T>& v) {
		T m = v[0];
		for (int i = 1; i < v.size(); i++) {
			if (v[i] < m) {
				m = v[i];
			}
		}
		return m;
	}

	template <typename T>
	extern T max(const std::vector<T>& v) {
		T m = v[0];
		for (int i = 1; i < v.size(); i++) {
			if (v[i] > m) {
				m = v[i];
			}
		}
		return m;
	}

	template <typename T>
	extern T sum(const std::vector<T>& v) {
		T s = 0;
		for (int i = 0; i < v.size(); i++) {
			s += v[i];
		}
		return s;
	}
}

extern const char* ImPlotMarkerToString(ImPlotMarker marker);

#endif