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
		double precision = 1.0;
		bool same = false;
		bool delta_loss = true;
		std::vector<double> y = {};

		auto mean = [](std::vector<double> y_true, std::vector<double> y_pred) {
			double error_squared = 0;
			for (int i = 0; i < y_true.size(); i++) {
				error_squared = (y_true[i] - y_pred[i]) * (y_true[i] - y_pred[i]);
			}
			error_squared = error_squared / y_true.size();
			return error_squared;
			};

		if (yModel.find(x_str) != std::string::npos) {
			if (yModel.find("a") != std::string::npos) {
				y = {};
				for (int i = 0; i < xs.size(); i++) {
					x = xs[i];
					y.push_back(m_e.value());
				}
				loss0 = mean(ys, y);
				a++;
				y = {};
				for (int i = 0; i < xs.size(); i++) {
					x = xs[i];
					y.push_back(m_e.value());
				}
				loss1 = mean(ys, y);
				a--;
				if (loss1 > loss0) {
					delta_loss = false;
				}
				while (precision > 0.00001 && not same) {
					if (delta_loss) {
						while (delta_loss && !same) {
							y = {};
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								y.push_back(m_e.value());
							}
							loss0 = mean(ys, y);
							a += precision;
							y = {};
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								y.push_back(m_e.value());
							}
							loss1 = mean(ys, y);

							delta_loss = loss1 < loss0;
							if (loss1 == 0) {
								same = true;
							}
						}
					}
					else {
						while (!delta_loss && !same) {
							y = {};
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								y.push_back(m_e.value());
							}
							loss0 = mean(ys, y);
							a -= precision;
							y = {};
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								y.push_back(m_e.value());
							}
							loss1 = mean(ys, y);

							delta_loss = loss1 < loss0;
							if (loss1 == 0) {
								same = true;
							}
						}
					}
					precision /= 10;
				}
			}
		}
		return true;
	}

	std::vector<double> gaussianMethod(std::vector<std::vector<double>>A, std::vector<double>B) {
		int n = A.size();

		for (int i = 0; i < n; i++) {
			int maxRow = i;
			for (int j = i + 1; j < n; j++) {
				if (abs(A[j][i]) > abs(A[maxRow][i])) {
					maxRow = j;
				}
			}

			std::swap(A[i], A[maxRow]);
			std::swap(B[i], B[maxRow]);

			//pivot eliomition
			for (int j = i + 1; j < n; j++) {
				double factor = A[j][i] / A[i][i];
				for (int k = i; k < n; k++) {
					A[j][k] -= factor * A[i][k];
				}
				B[j] -= factor * B[i];
			}
		}

		//substitution
		std::vector<double> X(n);
		for (int i = n - 1; i >= 0; i--) {
			X[i] = B[i] / A[i][i];
			for (int j = i - 1; j >= 0; j--) {
				B[j] -= A[j][i] * X[i];
			}
		}
		return X;
	}

	bool quadratic(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 3) {
			return false;
		}

		std::vector<std::vector<double>> A = {
			{xs[0] * xs[0], xs[0], 1},
			{xs[1] * xs[1], xs[1], 1},
			{xs[2] * xs[2], xs[2], 1}
		};
		std::vector<double> B = { ys[0], ys[1], ys[2] };

		std::vector<double> X = gaussianMethod(A, B);

		a = X[0];
		b = X[1];
		c = X[2];

		return true;
	}

	bool cubic(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c, double& d) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 4) {
			return false;
		}

		std::vector<std::vector<double>> A = {
			{xs[0] * xs[0] * xs[0], xs[0] * xs[0], xs[0], 1},
			{xs[1] * xs[1] * xs[1], xs[1] * xs[1], xs[1], 1},
			{xs[2] * xs[2] * xs[2], xs[2] * xs[2], xs[2], 1},
			{xs[3] * xs[3] * xs[3], xs[3] * xs[3], xs[3], 1},
		};
		std::vector<double> B = { ys[0], ys[1], ys[2], ys[3]};

		std::vector<double> X = gaussianMethod(A, B);

		a = X[0];
		b = X[1];
		c = X[2];
		d = X[3];

		return true;
	}

	bool sinusoidal(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 4) {
			return false;
		}

		auto f = [](double xi, double ai, double bi, double ci) {
			return ai * std::sin(bi * xi + ci);
			};
		a = std::max({ std::max(ys), std::abs(std::min(ys)) });

		std::vector<double> y = {};
		for (int i = 0; i < ys.size(); i++) {
			y.push_back(std::asin(ys[i]/a));
		}

		b = 0;
		for (int i = 0; i < xs.size() - 1; i++) {
			b += (y[i + 1] - y[i]) / (xs[i + 1] - xs[i]);
		}
		b /= xs.size() - 1;

		c = 0;
		for (int i = 0; i < xs.size(); i++) {
			c += y[i] - b * xs[i];
		}
		c /= xs.size() - 1;

		return true;
	}
}