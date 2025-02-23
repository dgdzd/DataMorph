#include <core/Regression.h>

#include <cmath>
#include <iostream>
#include <Utils.h>
#include <vector>
#include <exprtk.hpp>

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

	/*double exponential(const std::vector<double>& x, const std::vector<double>& y) {
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
	}*/

	bool custom(const std::vector<double>& xs, const std::vector<double>& ys, std::string model, double& a, double& b, double& c, std::string x_str) {
		if (xs.size() != ys.size()) {
			return false;
		}
		typedef exprtk::symbol_table<double> symbol_table_t;
		typedef exprtk::expression<double>   expression_t;
		typedef exprtk::parser<double>       parser_t;

		a = 1.0;
		b = 1.0;
		c = 1.0;
		double x = xs[0];

		symbol_table_t symbol_table;
		symbol_table.add_variable("a", a);
		symbol_table.add_variable("b", b);
		symbol_table.add_variable("c", c);
		symbol_table.add_variable(x_str, x);
		symbol_table.add_constants();

		expression_t m_e;
		m_e.register_symbol_table(symbol_table);

		parser_t parser;
		int separator_i = std::string(model).find("=");
		std::string xheader = std::string(model).substr(0, separator_i);
		std::string yModel = std::string(model).substr(separator_i + 1);
		parser.compile(yModel, m_e);

		double loss0 = 0.0;
		double loss1 = 0.0;
		int sign = 1;
		double precision = 1.0;
		bool same = false;
		double delta_loss = 0.0;

		if (yModel.find(x_str) != std::string::npos) {
			if (yModel.find("a") != std::string::npos) {
				for (int j = 0; j < ys.size(); j++) {
					x = xs[j];
					loss0 += std::abs(m_e.value() - ys[j]);
				}
				a++;
				for (int j = 0; j < ys.size(); j++) {
					x = xs[j];
					loss1 += std::abs(m_e.value() - ys[j]);
				}
				a--;
				if (loss1 - loss0 > 0) {
					sign = -1;
				}
				while (precision > 0.00001 && not same) {
					std::cout << sign << std::endl;
					if (sign == 1) {
						delta_loss = -1;
						while (delta_loss < 0) {
							loss0 = 0.0;
							for (int j = 0; j < ys.size(); j++) {
								x = xs[j];
								loss0 += std::abs(m_e.value() - ys[j]);
							}
							a += precision;
							loss1 = 0.0;
							for (int j = 0; j < ys.size(); j++) {
								x = xs[j];
								loss1 += std::abs(m_e.value() - ys[j]);
							}
							delta_loss = loss1 - loss0;
							if (delta_loss == 0) {
								same = true;
							}
							std::cout << a << std::endl;
							std::cout << loss0 << std::endl;
							std::cout << loss1 << std::endl;
						}
						sign = -1;
					}
					else {
						delta_loss = 1;
						while (delta_loss > 0) {
							loss0 = 0.0;
							for (int j = 0; j < ys.size(); j++) {
								x = xs[j];
								loss0 += std::abs(m_e.value() - ys[j]);
							}
							a -= precision;
							loss1 = 0.0;
							for (int j = 0; j < ys.size(); j++) {
								x = xs[j];
								loss1 += std::abs(m_e.value() - ys[j]);
							}
							delta_loss = loss1 - loss0;
							if (delta_loss == 0) {
								same = true;
							}
						}
						sign = 1;
					}
					precision /= 10;
				}
			}
		}
		return true;
	}
}