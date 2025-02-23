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
}