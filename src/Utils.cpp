#include <Utils.h>

// String Utilities

std::string std::trim_trailing(const std::string& str, const char c) {
	if (str[0] == c) {
		return trim_trailing(str.substr(1), c);
	}
	else {
		return str;
	}
}

std::string std::trim_leading(const std::string& str, const char c) {
	if (str[str.size() - 1] == c) {
		return trim_leading(str.substr(0, str.size() - 2), c);
	}
	else {
		return str;
	}
}

std::string std::trim(const std::string& str, const char c) {
	return trim_leading(trim_trailing(str, c), c);
}

// Enum-to-string conversion

const char* ImPlotMarkerToString(ImPlotMarker marker) {
	switch (marker) {
	case ImPlotMarker_Circle:
		return "Circle";
	case ImPlotMarker_Square:
		return "Square";
	case ImPlotMarker_Diamond:
		return "Diamond";
	case ImPlotMarker_Up:
		return "Up";
	case ImPlotMarker_Down:
		return "Down";
	case ImPlotMarker_Left:
		return "Left";
	case ImPlotMarker_Right:
		return "Right";
	case ImPlotMarker_Cross:
		return "Cross";
	case ImPlotMarker_Plus:
		return "Plus";
	case ImPlotMarker_Asterisk:
		return "Asterisk";
	default:
		return "None";
	}
}