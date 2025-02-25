#ifndef DM_UTILS_H
#define DM_UTILS_H

#include <core/Model.h>
#include <functional>
#include <implot.h>
#include <string>
#include <vector>

namespace std {
	extern string trim_trailing(const string& str, const char c = ' ');
	extern string trim_leading(const string& str, const char c = ' ');
	extern string trim(const string& str, const char c = ' ');
	extern std::vector<std::string> split(const std::string& str, const char c = ' ', int limit = 0);
	extern std::string format_num(double val, std::string prefix);

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
	extern T sum(const std::vector<T>& v, std::function<T(T val, int i)> func = [](T val, int i) { return val; }) {
		T s = 0;
		for (int i = 0; i < v.size(); i++) {
			s += func(v[i], i);
		}
		return s;
	}

	template <typename T>
	extern T mean(const std::vector<T>& v) {
		return sum(v) / v.size();
	}
}

extern const char* ImPlotMarkerToString(ImPlotMarker marker);
extern const char* ImPlotColormapToString(ImPlotColormap colormap);
extern ImPlotColormap StringToImPlotColormap(const std::string& str);
extern std::string ModelTypeToString(ModelType type);

#endif