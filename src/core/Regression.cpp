#include <core/Regression.h>

#include <cmath>
#include <iostream>
#include <Utils.h>
#include <vector>

namespace Regression {
	bool linear(const std::vector<double>& x, const std::vector<double>& y, double& w1) {
		if (x.size() != y.size()) {
			return false;
		}
		double sum_x2 = std::sum<double>(x, [](double val, int i) { return val * val; });
		double sum_xy = std::sum<double>(x, [&y](double val, int i) { return val * y[i]; });
		w1 = sum_xy / sum_x2;
		return true;
	}

	bool affine(const std::vector<double>& x, const std::vector<double>& y, double& w0, double& w1) {
		if (x.size() != y.size()) {
			return false;
		}
		double n = x.size();
		double xm = std::mean(x);
		double ym = std::mean(y);
		double sum_x2 = std::sum<double>(x, [](double val, int i) { return val * val; });
		double sum_xy = std::sum<double>(x, [&y](double val, int i) { return val * y[i]; });
		double quadratic_variation = std::sum<double>(x, [&y, &xm, &ym](double val, int i) { return (val - xm) * (y[i] - ym); });
		w1 = quadratic_variation / std::sum<double>(x, [&xm](double val, int i) { return pow(val - xm, 2); });
		w0 = ym - w1 * xm;
		return true;
	}

	double exponential(const std::vector<double>& x, const std::vector<double>& y) {
		if (x.size() != y.size()) {
			return 0.0;
		}
		double n = x.size();
		double sum_x = std::sum(x);
		double sum_y = std::sum(y);
		double sum_x2 = std::sum(x, [](double val) { return val * val; });
		double sum_xy = std::sum(x, [&y](double val, int i) { return val * y[i]; });
		double w1 = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
		double w0 = (sum_y - w1 * sum_x) / n;
	}

	double custom(std::string model, Project* project) {
		if (x.size() != y.size()) {
			return 0.0;
		}
		typedef exprtk::symbol_table<double> symbol_table_t;
		typedef exprtk::expression<double>   expression_t;
		typedef exprtk::parser<double>       parser_t;

		double a = 0.0;
		double b = 0.0;
		double c = 0.0;

		symbol_table_t symbol_table;
		symbol_table.add_variable("a", a);
		symbol_table.add_variable("b", b);
		symbol_table.add_variable("c", c);
		symbol_table.add_constants();
		for (int j = 0; j < project->symbol.size(); j++) {
			symbol_table.add_variable(project->symbols[j], project->headers[project->ids[j]].values[0]);
		}

		expression_t m_e;
		m_e.register_symbol_table(symbol_table);

		parser_t parser;
		int separator_i = std::string(g.model).find("=");
		std::string xheader = std::string(g.model).substr(0, separator_i);
		std::string yModel = std::string(g.model).substr(separator_i + 1);
		parser.compile(yModel, m_e);

		std::vector<double> x_models = {};
		for (int j = 0; j < g.lines[0].header->values.size(); j++) {
			x_models.push_back(g.xHeader->values[j]);
		}

		double loss0 = 0.0;
		double loss1 = 0.0;
		int sign = 1;
		double precision0 = 10e5;
		double precision = precision0;

		if (yModel.find("a") != std::string::npos) {
			for (int j = 0; j < x_models.size(); j++) {
				loss0 += std::abs(m_e.value() - x_models[j]);
			}
			a++;
			for (int j = 0; j < x_models.size(); j++) {
				loss1 += std::abs(m_e.value() - x_models[j]);
			}
			a--;
			if (loss1 - loss0 < 0) {
				sign = -1;
			}
			else {
				sign = 1;
			}
			while (precision > 0.00001) {
				if (sign == 1) {
					while (loss1 - loss0 > 0) {
						loss0 = 0.0;
						for (int j = 0; j < x_models.size(); j++) {
							loss0 += std::abs(m_e.value() - x_models[j]);
						}
						a += precision/precision0;
						loss1 = 0.0;
						for (int j = 0; j < x_models.size(); j++) {
							loss1 += std::abs(m_e.value() - x_models[j]);
						}
					}
					sign = -1;
				}
				else {
					while (loss1 - loss0 < 0) {
						loss0 = 0.0;
						for (int j = 0; j < x_models.size(); j++) {
							loss0 += std::abs(m_e.value() - x_models[j]);
						}
						a -= precision / precision0;
						loss1 = 0.0;
						for (int j = 0; j < x_models.size(); j++) {
							loss1 += std::abs(m_e.value() - x_models[j]);
						}
					}
					sign = 1;
				}

			}
		}
	}
}