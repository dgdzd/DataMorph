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

std::vector<std::string> std::split(const std::string& str, const char c) {
	std::vector<std::string> parts;
	std::string part = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == c) {
			parts.push_back(part);
			part = "";
		}
		else {
			part += str[i];
		}
	}
	parts.push_back(part);
	return parts;
}

std::string std::format_num(double val, std::string prefix) {
	if (val > 10e2) {
		std::string strval = std::to_string(val / 10e2);
		return strval.substr(0, strval.find(".") + 3) + " k" + prefix;
	}
	else if (val > 10e5) {
		std::string strval = std::to_string(val / 10e5);
		return strval.substr(0, strval.find(".") + 3) + " M" + prefix;
	}
	else if (val > 10e8) {
		std::string strval = std::to_string(val / 10e8);
		return strval.substr(0, strval.find(".") + 3) + " G" + prefix;
	}
	else if (val > 10e11) {
		std::string strval = std::to_string(val / 10e11);
		return strval.substr(0, strval.find(".") + 3) + " T" + prefix;
	}
	else {
		return std::to_string((int)val) + " " +prefix;
	}
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

std::string ModelTypeToString(ModelType type) {
	switch (type) {
	case ModelType::LINEAR:
		return "Linear";
	case ModelType::AFFINE:
		return "Affine";
	case ModelType::CUSTOM:
		return "Custom";
	case ModelType::QUADRATIC:
		return "Quadratic";
	case ModelType::CUBIC:
		return "Cubic";
	case ModelType::EXPONENTIAL:
		return "Exponential";
	case ModelType::LOGARITHMIC:
		return "Logarithmic";
	case ModelType::POWER:
		return "Power";
	case ModelType::OTHER:
		return "Other";
	default:
		return "None";
	}
}