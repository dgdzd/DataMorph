#ifndef DM_REGRESSION_H
#define DM_REGRESSION_H

#include <string>
#include <vector>

namespace Regression {
	extern bool linear(const std::vector<double>& x, const std::vector<double>& y, &w1);
	extern bool affine(const std::vector<double>& x, const std::vector<double>& y, &w0, &w1);
	extern double exponential(const std::vector<double>& x, const std::vector<double>& y);
	extern double logarithmic(const std::vector<double>& x, const std::vector<double>& y);
	extern double power(const std::vector<double>& x, const std::vector<double>& y);
	extern double polynomial(const std::vector<double>& x, const std::vector<double>& y, int degree);
	extern double logistic(const std::vector<double>& x, const std::vector<double>& y);
	extern double sinusoidal(const std::vector<double>& x, const std::vector<double>& y);
}

#endif