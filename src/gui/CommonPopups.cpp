#include <gui/CommonPopups.h>

#include <gui/MainWindow.h>
#include <imgui_stdlib.h>
#include <implot.h>
#include <exprtk.hpp>


NewVarPopup* NewVarPopup::inst = nullptr;
EditVarPopup* EditVarPopup::inst = nullptr;
NewGraphPopup* NewGraphPopup::inst = nullptr;
ResolveEquation* ResolveEquation::inst = nullptr;

NewVarPopup::NewVarPopup(MainWindow* parent) {
	this->parent = parent;
	this->name = "Add a column";
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	this->newSymbol = new char[32] {""};
	this->newUnit = new char[32] {""};
	this->expression = new char[32] {""};
	this->derivate = { 0, 0 };
	this->linespace = { 0.0f, 0.0f };
	this->integral = {0, 0};
	this->args = {};
	Project* pr = parent->state->openProject;
}

void NewVarPopup::onRender() {
	Project* pr = parent->state->openProject;
	int tab = -1;
	SetWindowSize(ImVec2(900, 500));

	if (BeginTabBar("NewVar")) {
		if (BeginTabItem("Dummy")) {
			tab = 0;
			Text("Symbol");
			InputText("##Symbol", newSymbol, 32);
			Text("Unit (optional)");
			InputText("##Unit", newUnit, 32);
			EndTabItem();
		}
		if (BeginTabItem("With expression")) {
			tab = 1;
			Text("Symbol");
			InputText("##Symbol", newSymbol, 32);
			Text("Unit (optional)");
			InputText("##Unit", newUnit, 32);
			Separator();

			Text("Input an expression : ");
			Text("%s :", newSymbol[0] == '\0' ? "(Symbol needed)" : (std::string(newSymbol) + "[i]"));
			SameLine();
			InputText("No blank", expression, 32);
			EndTabItem();
		}
		if (BeginTabItem("Derivate")) {
			tab = 2;
			Text("Symbol");
			InputText("##Symbol", newSymbol, 32);
			Text("Unit (optional)");
			InputText("##Unit", newUnit, 32);
			Separator();

			Text("Input a derivative :");
			Text("d");
			SameLine();
			if (BeginCombo("##3", pr->symbols[derivate.first].c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->symbols.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(pr->symbols[i].c_str(), is_selected)) {
						selected = i;
						derivate.first = i;
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}

			Separator();

			Text("dx");
			SameLine();
			if (BeginCombo("##4", pr->symbols[derivate.second].c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->symbols.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(pr->symbols[i].c_str(), is_selected)) {
						selected = i;
						derivate.second = i;
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}
			EndTabItem();
		}
		if (BeginTabItem("Linespace")) {
			tab = 3;
			Text("Symbol");
			InputText("##Symbol", newSymbol, 32);
			Text("Unit (optional)");
			InputText("##Unit", newUnit, 32);
			Separator();

			Text("Input a linespace :");
			Text("from : ");
			SameLine();
			InputFloat("##from", &linespace.first);
			Text("step : ");
			SameLine();
			InputFloat("##to", &linespace.second);
			EndTabItem();
		}
		if (BeginTabItem("Integral")) {
			tab = 4;
			Text("Symbol");
			InputText("##Symbol", newSymbol, 32);
			Text("Unit (optional)");
			InputText("##Unit", newUnit, 32);
			Separator();

			Text("Integral of : ");
			SameLine();
			if (BeginCombo("##5", pr->symbols[integral.second].c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->symbols.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(pr->symbols[i].c_str(), is_selected)) {
						selected = i;
						integral.second = i;
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}
			Text(" d");
			SameLine();
			if (BeginCombo("##6", pr->symbols[integral.first].c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->symbols.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(pr->symbols[i].c_str(), is_selected)) {
						selected = i;
						integral.first = i;
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}

			EndTabItem();
		}
		EndTabBar();
	}

	bool disabled = tab == 1 && expression[0] == '\0';
	BeginDisabled(disabled);
	{
		if (Button("Add")) {
			if (std::find(pr->symbols.begin(), pr->symbols.end(), newSymbol) != pr->symbols.end()) {
				parent->state->popups["Error###alreadyExists"] = true;
				parent->state->popups[name] = false;
				return;
			}

			ExpressionSpecs* specs = new ExpressionSpecs;
			switch (tab) {
			case 1:
				specs = new ExpressionSpecs(FORMULA, nullptr, nullptr);
				break;

			case 2:
				specs = new ExpressionSpecs(DERIVATIVE, &pr->headers[pr->ids[derivate.second]], &pr->headers[pr->ids[derivate.first]]);
				break;

			case 3:
				specs = new ExpressionSpecs(LINESPACE, nullptr, nullptr);
				this->args = { linespace.first, linespace.second };
				break;

			case 4:
				specs = new ExpressionSpecs(INTEGRAL, &pr->headers[pr->ids[integral.first]], &pr->headers[pr->ids[integral.second]]);
				break;

			default:
				break;
			}
			parent->state->popups[name] = false;
			Header* newHeader = new Header(pr, newSymbol, newUnit, {}, tab == 1 ? expression : "", specs, this->args);
			pr->ids.push_back(newHeader->id);
			pr->symbols.push_back(newSymbol);
			pr->units.push_back(newUnit);
			pr->addColumn(newHeader);
			CloseCurrentPopup();
			removeInstance();
		}
	}
	EndDisabled();
	SameLine();
	if (Button("Cancel")) {
		parent->state->popups[name] = false;
		CloseCurrentPopup();
		removeInstance();
	}
	EndPopup();
}

NewVarPopup* NewVarPopup::getInstance(MainWindow* mw) {
	if (!inst) {
		inst = new NewVarPopup(mw);
	}
	return inst;
}

void NewVarPopup::removeInstance() {
	delete inst;
	inst = nullptr;
}



EditVarPopup::EditVarPopup(MainWindow* parent, unsigned int id) : expression(nullptr, "") {
	this->parent = parent;
	this->name = "Edit variable";
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	this->pr = parent->state->openProject;
	Header* h = &pr->headers[id];
	this->id = id;
	this->newSymbol = h->name;
	this->newUnit = h->unit;
	if (h->expression) {
		this->expression = *h->expression;
	}
	if (expression.args.size() < 2) {
		expression.args = { 0.0, 0.0 };
	}
	if (!expression.specs.header_dx) {
		expression.specs.header_dx = &pr->headers[pr->ids[0]];
		expression.specs.header_dy = &pr->headers[pr->ids[0]];
	}
}

void EditVarPopup::onRender() {
	int tab = -1;
	SetWindowSize(ImVec2(900, 500));

	if (BeginTabBar("NewVar")) {
		if (BeginTabItem("Dummy")) {
			tab = 0;
			Text("Symbol");
			InputText("##Symbol", &newSymbol);
			Text("Unit (optional)");
			InputText("##Unit", &newUnit);
			EndTabItem();
		}
		if (BeginTabItem("With expression")) {
			tab = 1;
			Text("Symbol");
			InputText("##Symbol", &newSymbol);
			Text("Unit (optional)");
			InputText("##Unit", &newUnit);
			Separator();

			Text("Input an expression : ");
			Text("%s :", newSymbol[0] == '\0' ? "(Symbol needed)" : (std::string(newSymbol) + "[i]"));
			SameLine();
			InputText("No blank", &expression.expression);
			EndTabItem();
		}
		if (BeginTabItem("Derivate")) {
			tab = 2;
			Text("Symbol");
			InputText("##Symbol", &newSymbol);
			Text("Unit (optional)");
			InputText("##Unit", &newUnit);
			Separator();

			Text("Input a derivative :");
			Text("d");
			SameLine();
			if (BeginCombo("##3", expression.specs.header_dy->name.c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->ids.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(pr->symbols[i].c_str(), is_selected)) {
						selected = i;
						expression.specs.header_dy = &pr->headers[pr->ids[i]];
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}

			Separator();

			Text("dx");
			SameLine();
			if (BeginCombo("##4", pr->symbols[0].c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->symbols.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(expression.specs.header_dx->name.c_str(), is_selected)) {
						selected = i;
						expression.specs.header_dx = &pr->headers[pr->ids[i]];
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}
			EndTabItem();
		}
		if (BeginTabItem("Linespace")) {
			tab = 3;
			Text("Symbol");
			InputText("##Symbol", &newSymbol);
			Text("Unit (optional)");
			InputText("##Unit", &newUnit);
			Separator();

			Text("Input a linespace :");
			Text("from : ");
			SameLine();
			InputDouble("##from", &expression.args[0]);
			Text("step : ");
			SameLine();
			InputDouble("##to", &expression.args[1]);
			EndTabItem();
		}
		if (BeginTabItem("Integral")) {
			tab = 4;
			Text("Symbol");
			InputText("##Symbol", &newSymbol);
			Text("Unit (optional)");
			InputText("##Unit", &newUnit);
			Separator();

			Text("Integral of : ");
			SameLine();
			if (BeginCombo("##5", pr->symbols[0].c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->symbols.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(pr->symbols[i].c_str(), is_selected)) {
						selected = i;
						expression.specs.header_dy = &pr->headers[pr->ids[i]];
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}
			Text(" d");
			SameLine();
			if (BeginCombo("##6", pr->symbols[0].c_str())) {
				int selected = 0;
				for (int i = 0; i < pr->symbols.size(); i++) {
					bool is_selected = selected == i;
					if (Selectable(pr->symbols[i].c_str(), is_selected)) {
						selected = i;
						expression.specs.header_dx = &pr->headers[pr->ids[i]];
					}
					if (is_selected) {
						SetItemDefaultFocus();
					}
				}
				EndCombo();
			}

			EndTabItem();
		}
		EndTabBar();
	}

	bool disabled = tab == 1 && expression.expression[0] == '\0';
	BeginDisabled(disabled);
	{
		if (Button("Modify")) {
			std::vector<double> args;
			switch (tab) {
			case 1:
				expression.specs = ExpressionSpecs(FORMULA, nullptr, nullptr);
				break;

			case 2:
				expression.specs = ExpressionSpecs(DERIVATIVE, expression.specs.header_dx, expression.specs.header_dy);
				break;

			case 3:
				expression.specs = ExpressionSpecs(LINESPACE, nullptr, nullptr);
				args = expression.args;
				break;

			case 4:
				expression.specs = ExpressionSpecs(INTEGRAL, expression.specs.header_dx, expression.specs.header_dy);
				break;

			default:
				break;
			}

			Header* h = &pr->headers[id];
			auto i = std::find(pr->ids.begin(), pr->ids.end(), this->id);
			int d = std::distance(pr->ids.begin(), i);
			pr->symbols[d] = newSymbol;
			pr->units[d] = newUnit;
			std::vector<double> values = h->values;
			pr->headers[this->id] = Header(pr, newSymbol, newUnit, values, tab == 1 ? expression.expression.c_str() : "", &expression.specs, args);
			pr->headers[this->id].id = this->id;
			pr->headers[this->id].expression->parent = &pr->headers[this->id];
			pr->headers[this->id].expression->addVars();
			pr->headers[this->id].expression->compileExpression();
			pr->headers[this->id].expression->updateValues();
			parent->state->popups[name] = false;
			CloseCurrentPopup();
			removeInstance();
		}
	}
	EndDisabled();
	SameLine();
	if (Button("Cancel")) {
		parent->state->popups[name] = false;
		CloseCurrentPopup();
		removeInstance();
	}
	EndPopup();
}

EditVarPopup* EditVarPopup::getInstance(MainWindow* mw, unsigned int id) {
	if (!inst) {
		inst = new EditVarPopup(mw, id);
	}
	return inst;
}

bool EditVarPopup::hasInstance() {
	return inst;
}

void EditVarPopup::removeInstance() {
	delete inst;
	inst = nullptr;
}



NewGraphPopup::NewGraphPopup(MainWindow* parent) {
	this->parent = parent;
	this->name = "Manage graphs";
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	this->pr = parent->state->openProject;
	this->settings = Settings::instance;
}

void NewGraphPopup::onRender() {
	Project* pr = parent->state->openProject;
	SetWindowSize(ImVec2(450, 700));

	if (pr->graphs.size() == 0) {
		TextWrapped("There is currently no graph.");
		if (Button("Create one")) {
			Graph g("New graph 1", &pr->headers[pr->ids[0]], { Line(&pr->headers[pr->ids[0]]) }, 0, 0);
			g.lines[0].color = new ImVec4(ImPlot::GetColormapColor(0, this->settings->get_int("graphs_cmap")));
			g.lines[0].marker = this->settings->get_int("default_marker");
			pr->graphs.push_back(g);
		}
	}
	else if (BeginTabBar("graphs", ImGuiTabBarFlags_AutoSelectNewTabs)) {
		for (int i = 0; i < pr->graphs.size(); i++) {
			Graph& graph = pr->graphs[i];
			if (BeginTabItem(("Graph #" + std::to_string(i + 1)).c_str())) {
				Text("Name");
				InputText("##graphName", &graph.name, 32);
				Dummy(ImVec2(0.0f, 10.0f));
				if (BeginCombo("X axis", graph.xHeader->name.c_str())) {
					for (unsigned int id : pr->ids) {
						if (Selectable(pr->headers[id].name.c_str())) {
							graph.xHeader = &pr->headers[id];
						}
					}
					EndCombo();
				}
				Dummy(ImVec2(0.0f, 10.0f));
				Separator();
				Dummy(ImVec2(0.0f, 10.0f));
				Text("Lines");
				Text("Selected colormap : %s", ImPlotColormapToString(this->settings->get_int("graphs_cmap")));
				for (int j = 0; j < graph.lines.size(); j++) {
					if (TreeNode(("Line #" + std::to_string(j + 1)).c_str())) {
						Line& line = graph.lines[j];
						if (BeginCombo("Data to plot", line.header->name.c_str())) {
							for (unsigned int id : pr->ids) {
								if (Selectable(pr->headers[id].name.c_str())) {
									line.header = &pr->headers[id];
								}
							}
							EndCombo();
						}
						Checkbox("Scatter", &line.scatter);
						ColorEdit4("Line color", &line.color->x);
						if (BeginCombo("Marker", ImPlotMarkerToString(line.marker))) {
							for (int i = -1; i < ImPlotMarker_COUNT; i++) {
								if (Selectable(ImPlotMarkerToString(i))) {
									line.marker = i;
								}
							}
							EndCombo();
						}
						if (graph.lines.size() > 1) {
							PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
							PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
							PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.2f, 0.2f, 1.0f));
							if (Button(("Remove###" + std::to_string(j)).c_str())) {
								graph.lines.erase(graph.lines.begin() + j);
							}
							PopStyleColor();
							PopStyleColor();
							PopStyleColor();
						}
						TreePop();
					}
				}
				Dummy(ImVec2(0.0f, 10.0f));
				if (Button("+")) {
					Line l(&pr->headers[pr->ids[0]]);
					l.color = new ImVec4(ImPlot::GetColormapColor(graph.lines.size()+1, this->settings->get_int("graphs_cmap")));
					l.marker = this->settings->get_int("default_marker");
					graph.lines.push_back(l);
				}
				if (pr->graphs.size() > 0) {
					Dummy(ImVec2(0.0f, 15.0f));
					PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
					PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
					PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.2f, 0.2f, 1.0f));
					if (Button("Remove this graph")) {
						pr->graphs.erase(pr->graphs.begin() + i);
					}
					PopStyleColor();
					PopStyleColor();
					PopStyleColor();
				}
				EndTabItem();
			}
		}
		if (TabItemButton("+", ImGuiTabItemFlags_Trailing)) {
			Line l(&pr->headers[pr->ids[0]]);
			l.color = new ImVec4(ImPlot::GetColormapColor(0, this->settings->get_int("graphs_cmap")));
			l.marker = this->settings->get_int("default_marker");
			Graph g(("New graph " + std::to_string(pr->graphs.size() + 1)), &pr->headers[pr->ids[0]], { l }, 0, 0);
			pr->graphs.push_back(g);
		}
		EndTabBar();
	}
	Dummy(ImVec2(0.0f, 15.0f));
	if (Button("OK")) {
		parent->state->popups[name] = false;
		CloseCurrentPopup();
		removeInstance();
	}

	EndPopup();
}

NewGraphPopup* NewGraphPopup::getInstance(MainWindow* mw) {
	if (!inst) {
		inst = new NewGraphPopup(mw);
	}
	return inst;
}

void NewGraphPopup::removeInstance() {
	delete inst;
	inst = nullptr;
}


ResolveEquation::ResolveEquation(MainWindow* parent) {
	this->parent = parent;
	this->name = "Resolve Equation";
	this->p_open = true;
	this->showCloseButton = true;
	this->style = ImGui::GetStyle();
	this->wflags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	this->pr = parent->state->openProject;
	this->equation = "";
	this->result = "";
	this->error = "";
	this->from = 0.0;
	this->to = 100.0;
}

void ResolveEquation::onRender() {
	pr = parent->state->openProject;
	SetWindowSize(ImVec2(900, 500));

	Text("Search from : ");
	SameLine();
	InputDouble("##from", &from);
	Text("to : ");
	SameLine();
	InputDouble("##to", &to);

	Text("Equation : ");
	InputText("##left", &equation);
	BeginDisabled(equation.find("=") == std::string::npos);
	{
		if (Button("Resolve")) {
			if (from < to) {
				int separator_i = equation.find("=");
				std::string l = equation.substr(0, separator_i);
				std::string r = equation.substr(separator_i + 1);
				std::vector<double> x_s = resolveEqu(l, r, this->error);

				if (x_s.size() > 0) {
					result = "S = {";
					for (int i = 0; i < x_s.size(); i++) {
						result += std::to_string(x_s[i]) + (i == x_s.size()-1 ? "" : " ; ");
					}
					result += "}";
				}
				else {
					result = "S = {}";
				}
			}
		}
		EndDisabled();
	}
	if (error.empty()) {
		TextWrapped(this->result.c_str());
	}
	else {
		TextWrapped(this->error.c_str());
	}

	Dummy(ImVec2(0.0, 20.0));
	if (Button("Done")) {
		CloseCurrentPopup();
		parent->state->popups[name] = false;
		removeInstance();
	}
	EndPopup();
}

std::vector<double> ResolveEquation::resolveEqu(std::string l_expression, std::string r_expression, std::string& error, int max_iter) {
	bool invalid = false;
	bool swap = false;
	bool sign = 0;
	error = "";
	double x = from;
	std::vector<double> x_s = {};
	double precision_0 = 10e5;
	double precision = precision_0;

	typedef exprtk::symbol_table<double> symbol_table_t;
	typedef exprtk::expression<double>   expression_t;
	typedef exprtk::parser<double>       parser_t;

	symbol_table_t symbol_table;
	symbol_table.add_variable("x", x);
	symbol_table.add_constants();

	expression_t l_e;
	expression_t r_e;
	l_e.register_symbol_table(symbol_table);
	r_e.register_symbol_table(symbol_table);

	parser_t parser;
	if (!parser.compile(l_expression, l_e)) {
		error = "The left member of the equation has an invalid syntax.";
		return {};
	}
	if (!parser.compile(r_expression, r_e)) {
		error = "The right member of the equation has an invalid syntax.";
		return {};
	}

	double ivl = l_e.value();
	double ivr = r_e.value();

	if (ivl - ivr > 0) {
		swap = true;
		sign = 1;
	}
	else if (ivl - ivr == 0) {
		while (l_e.value() - r_e.value() == 0 && x == to) {
			x += 1 / precision_0;
		}
		if (x == to) {
			return {};
		}
		if (l_e.value() - r_e.value() > 0) {
			swap = true;
			sign = 1;
		}
	}

	bool exit = false;
	while (!exit) {
		while (precision > 0.00001) {
			precision /= 10;
			double to_add = precision / precision_0;
			double lval = l_e.value();
			double rval = r_e.value();
			if (!swap) {
				for (lval = l_e.value(), rval = r_e.value(); lval < rval; x += to_add, lval = l_e.value(), rval = r_e.value()) {
					if (x > to) {
						exit = true;
						break;
					}
				}
				if (lval != lval || rval != rval) {
					while (lval != lval || rval != rval) {
						x += to_add;
						lval = l_e.value();
						rval = r_e.value();
					}
					x += to_add;
				}
				precision /= 10;
				to_add /= 10;
				for (lval = l_e.value(), rval = r_e.value(); lval > rval; x -= to_add, lval = l_e.value(), rval = r_e.value()) {
					// Nothing
				}
				if (lval != lval || rval != rval) {
					while (lval != lval || rval != rval) {
						x -= to_add;
						lval = l_e.value();
						rval = r_e.value();
					}
					x -= to_add;
				}
				if (exit) {
					if (x < to && x > from) {
						exit = false;
					}
					else {
						return x_s;
					}
				}
			}
			else {
				for (lval = l_e.value(), rval = r_e.value(); lval > rval; x += to_add, lval = l_e.value(), rval = r_e.value()) {
					if (x > to) {
						exit = true;
						break;
					}
				}
				if (lval != lval || rval != rval) {
					while (lval != lval || rval != rval) {
						x += to_add;
						lval = l_e.value();
						rval = r_e.value();
					}
					x += to_add;
				}
				precision /= 10;
				to_add /= 10;
				for (lval = l_e.value(), rval = r_e.value(); lval < rval; x -= to_add, lval = l_e.value(), rval = r_e.value()) {
					// Nothing
				}
				if (lval != lval || rval != rval) {
					while (lval != lval || rval != rval) {
						x -= to_add;
						lval = l_e.value();
						rval = r_e.value();
					}
					x -= to_add;
				}
				if (x < to && x > from) {
					exit = false;
				}
				else {
					return x_s;
				}
			}
		}
		if (std::find(x_s.begin(), x_s.end(), x) != x_s.end()) {
			return x_s;
		}
		x_s.push_back(x);
		x += 1 / precision_0;
		swap = !swap;
		precision = precision_0;
	}
	return x_s;
}

ResolveEquation* ResolveEquation::getInstance(MainWindow* mw) {
	if (!inst) {
		inst = new ResolveEquation(mw);
	}
	return inst;
}

void ResolveEquation::removeInstance() {
	delete inst;
	inst = nullptr;
}
