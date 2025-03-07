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

		if (yModel.find(x_str) != std::string::npos) {
			if (yModel.find("a") != std::string::npos) {
				loss0 = 0;
				for (int i = 0; i < xs.size(); i++) {
					x = xs[i];
					loss0 += std::abs(m_e.value() - ys[i]);
				}
				a++;
				loss1 = 0;
				for (int i = 0; i < xs.size(); i++) {
					x = xs[i];
					loss1 += std::abs(m_e.value() - ys[i]);
				}
				a--;
				delta_loss = loss1 < loss0;

				while (precision > 0.00001 && not same) {
					if (delta_loss) {
						while (delta_loss && !same) {
							loss0 = 0;
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								loss0 += std::abs(m_e.value() - ys[i]);
							}
							a += precision;
							loss1 = 0;
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								loss1 += std::abs(m_e.value() - ys[i]);
							}

							delta_loss = loss1 < loss0;
							same = loss1 == 0;
						}
					}
					else {
						while (!delta_loss && !same) {
							loss0 = 0;
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								loss0 += std::abs(m_e.value() - ys[i]);
							}
							a -= precision;
							loss1 = 0;
							for (int i = 0; i < xs.size(); i++) {
								x = xs[i];
								loss1 += std::abs(m_e.value() - ys[i]);
							}

							delta_loss = loss1 < loss0;
							same = loss1 == 0;
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

		a = (std::max(ys) - std::min(ys)) / 2;
		if (a < 0) {
			return false;
		}
		if (a == 0) {
			b = 1;
			c = 0;
			return true;
		}

		std::vector<double> peak_xs = {};
		for (int i = 1; i < xs.size() - 1; ++i) {
			if (ys[i] > ys[i - 1] && ys[i] > ys[i + 1]) {
				peak_xs.push_back(xs[i]);
			}
		}
		if (peak_xs.size() < 2) {
			return false;
		}

		double av_period = 0.0;
		for (int i = 1; i < peak_xs.size(); ++i) {
			av_period += (peak_xs[i] - peak_xs[i - 1]);
		}
		av_period /= (peak_xs.size() - 1);
		b = (2 * 3.14159) / av_period;

		auto f = [](double x, double a, double b, double c) {
			return a * std::sin(b * x + c);
			};

		double precision = 0.1;
		double loss0 = 0.0;
		double loss1 = 0.0;
		double x = xs[0];
		for (int i = 0; i < xs.size(); i++) {
			x = xs[i];
			loss0 += std::abs(f(x, a, b, c) - ys[i]);
		}
		c++;
		for (int i = 0; i < xs.size(); i++) {
			x = xs[i];
			loss1 += std::abs(f(x, a, b, c) - ys[i]);
		}
		c--;
		bool same = loss0 == 0;
		bool sign = loss1 < loss0;
		while (precision > 0.00001 && !same) {
			if (sign) {
				while (sign && !same) {
					loss0 = 0;
					for (int i = 0; i < xs.size(); i++) {
						x = xs[i];
						loss0 += std::abs(f(x, a, b, c) - ys[i]);
					}
					c += precision;
					loss1 = 0;
					for (int i = 0; i < xs.size(); i++) {
						x = xs[i];
						loss1 += std::abs(f(x, a, b, c) - ys[i]);
					}
					same = loss1 == 0;
					sign = loss1 < loss0;
				}
				sign = not sign;
			}
			else {
				while (!sign && !same) {
					loss0 = 0;
					for (int i = 0; i < xs.size(); i++) {
						x = xs[i];
						loss0 += std::abs(f(x, a, b, c) - ys[i]);
					}
					c -= precision;
					loss1 = 0;
					for (int i = 0; i < xs.size(); i++) {
						x = xs[i];
						loss1 += std::abs(f(x, a, b, c) - ys[i]);
					}
					same = loss1 == 0;
					sign = loss1 < loss0;
				}
				sign = not sign;
			}
			precision /= 10;
		}

		return true;
	}

	bool logarithmic10(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}

		double X = 0;
		int n = xs.size();
		for (int i = 0; i < n; i++) {
			X += std::log10(xs[i]);
		}
		X /= n;

		double Y = 0;
		for (int i = 0; i < n; i++) {
			Y += ys[i];
		}
		Y /= n;

		double a_up = 0;
		for (int i = 0; i < n; i++) {
			a_up += (std::log10(xs[i]) - X) * (ys[i] - Y);
		}
		
		double a_down = 0;
		for (int i = 0; i < n; i++) {
			a_down += std::pow((std::log10(xs[i]) - X), 2);
		}

		a = a_up / a_down;

		b = Y - a * X;

		return true;
	}

	bool logarithmic(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& base_n) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}
		if (base_n <= 0) {
			return false;
		}

		auto log = [](double x, double n) {
			return std::log(x)/std::log(n);	
			};

		double X = 0;
		int n = xs.size();
		for (int i = 0; i < n; i++) {
			X += log(xs[i], base_n);
		}
		X /= n;

		double Y = 0;
		for (int i = 0; i < n; i++) {
			Y += ys[i];
		}
		Y /= n;

		double a_up = 0;
		for (int i = 0; i < n; i++) {
			a_up += (log(xs[i], base_n) - X) * (ys[i] - Y);
		}

		double a_down = 0;
		for (int i = 0; i < n; i++) {
			a_down += std::pow((log(xs[i], base_n) - X), 2);
		}

		a = a_up / a_down;

		b = Y - a * X;

		return true;
	}

	bool exponential(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& n) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}

		auto log = [](double x, double n) {
			return std::log(x) / std::log(n);
			};

		std::vector<double> Y = {};
		for (int i = 0; i < xs.size(); i++) {
			Y.push_back(log(ys[i], n));
		}


		double xm = std::mean(xs);
		double ym = std::mean(Y);
		double sum_x2 = std::sum<double>(xs, [](double val, int i) { return val * val; });
		double sum_xy = std::sum<double>(xs, [&Y](double val, int i) { return val * Y[i]; });
		double quadratic_variation = std::sum<double>(xs, [&Y, &xm, &ym](double val, int i) { return (val - xm) * (Y[i] - ym); });
		b = quadratic_variation / std::sum<double>(xs, [&xm](double val, int i) { return pow(val - xm, 2); });
		a = ym - b * xm;
		a = std::pow(n, a);

		return true;
	}

	bool sqrt(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}

		std::vector<double> X = {};
		for (int i = 0; i < xs.size(); i++) {
			X.push_back(std::sqrt(xs[i]));
		}

		double xm = std::mean(X);
		double ym = std::mean(ys);
		double sum_x2 = std::sum<double>(X, [](double val, int i) { return val * val; });
		double sum_xy = std::sum<double>(X, [&ys](double val, int i) { return val * ys[i]; });
		double quadratic_variation = std::sum<double>(X, [&ys, &xm, &ym](double val, int i) { return (val - xm) * (ys[i] - ym); });
		b = quadratic_variation / std::sum<double>(X, [&xm](double val, int i) { return pow(val - xm, 2); });
		a = ym - b * xm;

		return true;
	}
	bool inverse(const std::vector<double>& xs, const std::vector<double>& ys, double& a) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}

		std::vector<double> as = {};
		for (int i = 0; i < xs.size(); i++) {
			as.push_back(xs[i] * ys[i]);
		}
		a = std::sum(as) / as.size();

		return true;
	}

	bool lituus(const std::vector<double>& xs, const std::vector<double>& ys, double& a) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}

		std::vector<double> as = {};
		for (int i = 0; i < xs.size(); i++) {
			as.push_back(ys[i] * std::sqrt(xs[i]));
		}
		a = std::sum(as) / as.size();

		return true;
	}

	bool cochleoid(const std::vector<double>& xs, const std::vector<double>& ys, double& a) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}

		std::vector<double> as = {};
		for (int i = 0; i < xs.size(); i++) {
			as.push_back(ys[i] * xs[i] / std::sin(xs[i]));
		}
		a = std::sum(as) / as.size();

		return true;
	}

	bool rhodonea(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b) {
		if (xs.size() != ys.size()) {
			return false;
		}
		if (xs.size() < 2) {
			return false;
		}

		auto f = [](double r1, double r2, double& b, double t1, double t2, double precision, bool& order) {
			double y0 = (r1 * std::sin(b * t2)) / (r2 * t1);
			b += precision * (2 * order - 1);
			double y1 = (r1 * std::sin(b * t2)) / (r2 * t1);
			order = y1 > y0;
			};

		a = 1;
		bool order = true;
		f(ys[0], ys[1], b, xs[0], xs[1], 0.1, order);
		while ((order and order) or (!order and !order)) {
			f(ys[0], ys[1], b, xs[0], xs[1], 0.1, order);
		}

		return true;
	}
}