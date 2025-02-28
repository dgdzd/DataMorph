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

std::vector<std::string> std::split(const std::string& str, const char c, int limit) {
	std::vector<std::string> parts;
	std::string part = "";
	int count = 0;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == c && (limit == 0 || count < limit)) {
			parts.push_back(part);
			part = "";
			count++;
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

const char* ImPlotColormapToString(ImPlotColormap colormap) {
	switch (colormap) {
	case ImPlotColormap_Deep:
		return "Seaborn Deep";
	case ImPlotColormap_Dark:
		return "Dark";
	case ImPlotColormap_Pastel:
		return "Pastel";
	case ImPlotColormap_Paired:
		return "Paired";
	case ImPlotColormap_Viridis:
		return "Viridis";
	case ImPlotColormap_Plasma:
		return "Plasma";
	case ImPlotColormap_Hot:
		return "Hot";
	case ImPlotColormap_Cool:
		return "Cool";
	case ImPlotColormap_Pink:
		return "Pink";
	case ImPlotColormap_Jet:
		return "Jet";
	case ImPlotColormap_Twilight:
		return "Twilight";
	case ImPlotColormap_RdBu:
		return "Red/Blue";
	case ImPlotColormap_BrBG:
		return "Brown/Blue-green";
	case ImPlotColormap_PiYG:
		return "Pink/Yellow-green";
	case ImPlotColormap_Spectral:
		return "Spectral";
	case ImPlotColormap_Greys:
		return "Greys";
	default:
		return "Dark";
	}
}

ImPlotColormap StringToImPlotColormap(const std::string& str) {
	if (str == "Seaborn Deep") {
		return ImPlotColormap_Deep;
	}
	else if (str == "Dark") {
		return ImPlotColormap_Dark;
	}
	else if (str == "Pastel") {
		return ImPlotColormap_Pastel;
	}
	else if (str == "Paired") {
		return ImPlotColormap_Paired;
	}
	else if (str == "Viridis") {
		return ImPlotColormap_Viridis;
	}
	else if (str == "Plasma") {
		return ImPlotColormap_Plasma;
	}
	else if (str == "Hot") {
		return ImPlotColormap_Hot;
	}
	else if (str == "Cool") {
		return ImPlotColormap_Cool;
	}
	else if (str == "Pink") {
		return ImPlotColormap_Pink;
	}
	else if (str == "Jet") {
		return ImPlotColormap_Jet;
	}
	else if (str == "Twilight") {
		return ImPlotColormap_Twilight;
	}
	else if (str == "Red/Blue") {
		return ImPlotColormap_RdBu;
	}
	else if (str == "Brown/Blue-green") {
		return ImPlotColormap_BrBG;
	}
	else if (str == "Pink/Yellow-green") {
		return ImPlotColormap_PiYG;
	}
	else if (str == "Spectral") {
		return ImPlotColormap_Spectral;
	}
	else if (str == "Greys") {
		return ImPlotColormap_Greys;
	}
	else {
		return -1;
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
	case ModelType::SINUS:
		return "Sinus";
	case ModelType::LOG10:
		return "Logarithmic Base 10";
	case ModelType::LOGN:
		return "Logarithmic Base n";
	case ModelType::LOG:
		return "Natural Logarithmic";
	case ModelType::EXPN:
		return "Exponential Base n";
	case ModelType::EXP:
		return "Exponential";
	case ModelType::SQRT:
		return "Square Root";
	case ModelType::OTHER:
		return "Other";
	default:
		return "None";
	}
}