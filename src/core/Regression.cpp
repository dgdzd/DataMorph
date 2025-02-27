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
		std::string yModel = "";
		if (model.find("=") != std::string::npos) {
			int separator_i = model.find("=");
			yModel = model.substr(separator_i + 1);
		}
		else {
			yModel = model;
		}
		parser.compile(yModel, m_e);

		double loss0 = 0.0;
		double loss1 = 0.0;
		int sign = 1;
		double precision = 1.0;
		bool same = false;
		double delta_loss = 0.0;

		if (yModel.find(x_str) != std::string::npos) {
			if (yModel.find("a") != std::string::npos) {
				x = xs[0];
				double y_0 = m_e.value();
				for (int j = 0; j < ys.size(); j++) {
					x = xs[j];
					loss0 += std::abs(m_e.value() - ys[j]);
				}
				a++;
				x = xs[0];
				double y_1 = m_e.value();
				for (int j = 0; j < ys.size(); j++) {
					x = xs[j];
					loss1 += std::abs(m_e.value() - ys[j]);
				}
				a--;
				if (loss1 - loss0 > 0) {
					sign *= -1;
				}
				while (precision > 0.00001 && not same) {
					if (sign == 1) {
						delta_loss = -1;
						while (delta_loss < 0 && !same) {
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
							if (loss1 == 0) {
								same = true;
							}
						}
						sign *= -1;
					}
					else {
						delta_loss = 1;
						while (delta_loss > 0 && !same) {
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
							if (loss1 == 0) {
								same = true;
							}
						}
						sign *= -1;
					}
					precision /= 10;
				}
			}
		}
		return true;
	}

	bool polynomial(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 3) {
			return false;
		}
		
		//given ax^2+bx+c with 3 points : 
		//equ1 : c = ax0^2 + bx0 - y0
		//equ2 : ax1^2+bx1+c = y1
		//equ3 : ax2^2+bx2+c = y2
		//equ4 : ax1^2 + bx1 + (ax0^2 + bx0 - y0) = y1 -->to calculate b
		//equ5 : ax2^2 + bx2 + (ax0^2 + bx0 - y0) = y2
		//equ6 : ax1^2 + bx1 + (ax0^2 + bx0 - y0) - (ax2^2 + bx2 + (ax0^2 + bx0 - y0)) = y1 - y2 -->to calculate a

		typedef exprtk::symbol_table<double> symbol_table_t;
		typedef exprtk::expression<double>   expression_t;
		typedef exprtk::parser<double>       parser_t;

		double x0 = xs[0];
		double x1 = xs[1];
		double x2 = xs[2];
		double y0 = ys[0];
		double y1 = ys[1];
		double y2 = ys[2];

		a = 1.0;
		b = 1.0;
		c = 1.0;

		symbol_table_t symbol_table;
		symbol_table.add_variable("a", a);
		symbol_table.add_variable("b", b);
		symbol_table.add_variable("c", c);
		symbol_table.add_variable("x0", x0);
		symbol_table.add_variable("x1", x1);
		symbol_table.add_variable("x2", x2);
		symbol_table.add_variable("y0", y0);
		symbol_table.add_variable("y1", y1);
		symbol_table.add_variable("y2", y2);
		symbol_table.add_constants();

		expression_t e_4;
		expression_t e_6;
		e_4.register_symbol_table(symbol_table);
		e_6.register_symbol_table(symbol_table);

		parser_t parser;
		std::string equ4_l = "a*x1^2 + b*x1 + (a*x0^2 + b*x0 - y0)";
		std::string equ6_l = "a*x1^2 + b*x1 + (a*x0^2 + b*x0 - y0) - (a*x2^2 + b*x2 + (a*x0^2 + b*x0 - y0))";
		parser.compile(equ4_l, e_4);
		parser.compile(equ6_l, e_6);

		double precision = 1.0;
		bool same = false;
		if (e_6.value() == y1 - y2) {
			same = true;
		}
		while (precision > 0.00001 && !same) {
			if (e_6.value() < y1 - y2) {
				while (e_6.value() < y1 - y2) {
					a++;
				}
			}
			else {
				while (e_6.value() > y1 - y2) {
					a--;
				}
			}
			precision /= 10;
			if (e_6.value() == y1 - y2) {
				same = true;
			}
		}

		return true;
	}
}

	/*
		auto f = [](double x_i, double a, double b, double c) {
			return a * x_i * x_i + b * x_i + c;
		};
	
		a = std::double_t(a_sign) * 2 - 1;
		b = 1;
		c = 1;

		double precision = 1.0;
		bool sign = 1;
		double loss0 = 0.0;
		double loss1 = 0.0;
		bool delta_loss = false;

		for (int i = 0; i < y.size(); i++) {
			loss0 += std::abs(f(x[i], a, b, c) - y[i]);
		}
		a++;
		for (int i = 0; i < y.size(); i++) {
			loss1 += std::abs(f(x[i], a, b, c) - y[i]);
		}
		a--;
		delta_loss = loss0 > loss1;

		while (precision > 0.00001) {
			if (delta_loss) {
				while (delta_loss) {
					for (int i = 0; i < y.size(); i++) {
						loss0 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					a -= precision;
					for (int i = 0; i < y.size(); i++) {
						loss1 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					delta_loss = loss0 > loss1;
				}
			}
			else {
				while (!delta_loss) {
					for (int i = 0; i < y.size(); i++) {
						loss0 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					a -= precision;
					for (int i = 0; i < y.size(); i++) {
						loss1 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					delta_loss = loss0 > loss1;
				}
			}
			precision /= 10;
		}

		for (int i = 0; i < y.size(); i++) {
			loss0 += std::abs(f(x[i], a, b, c) - y[i]);
		}
		b++;
		for (int i = 0; i < y.size(); i++) {
			loss1 += std::abs(f(x[i], a, b, c) - y[i]);
		}
		b--;
		delta_loss = loss0 > loss1;

		while (precision > 0.00001) {
			if (delta_loss) {
				while (delta_loss) {
					for (int i = 0; i < y.size(); i++) {
						loss0 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					b -= precision;
					for (int i = 0; i < y.size(); i++) {
						loss1 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					delta_loss = loss0 > loss1;
				}
			}
			else {
				while (!delta_loss) {
					for (int i = 0; i < y.size(); i++) {
						loss0 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					b -= precision;
					for (int i = 0; i < y.size(); i++) {
						loss1 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					delta_loss = loss0 > loss1;
				}
			}
			precision /= 10;
		}

		for (int i = 0; i < y.size(); i++) {
			loss0 += std::abs(f(x[i], a, b, c) - y[i]);
		}
		c++;
		for (int i = 0; i < y.size(); i++) {
			loss1 += std::abs(f(x[i], a, b, c) - y[i]);
		}
		c--;
		delta_loss = loss0 > loss1;

		while (precision > 0.00001) {
			if (delta_loss) {
				while (delta_loss) {
					for (int i = 0; i < y.size(); i++) {
						loss0 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					c -= precision;
					for (int i = 0; i < y.size(); i++) {
						loss1 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					delta_loss = loss0 > loss1;
				}
			}
			else {
				while (!delta_loss) {
					for (int i = 0; i < y.size(); i++) {
						loss0 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					c -= precision;
					for (int i = 0; i < y.size(); i++) {
						loss1 += std::abs(f(x[i], a, b, c) - y[i]);
					}
					delta_loss = loss0 > loss1;
				}
			}
			precision /= 10;
		}
	*/