#ifndef DM_REGRESSION_H
#define DM_REGRESSION_H

#include <string>
#include <vector>

namespace Regression {
	//done
	extern bool linear(const std::vector<double>& x, const std::vector<double>& y, double& w1);
	extern bool affine(const std::vector<double>& x, const std::vector<double>& y, double& w0, double& w1);
	extern bool custom(const std::vector<double>& xs, const std::vector<double>& ys, std::string model, double& a, double& b, double& c, std::string x_str);
	extern bool polynomial(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b, double& c, bool a_sign);
	//to do
	extern double exponential(const std::vector<double>& x, const std::vector<double>& y);
	extern double logarithmic(const std::vector<double>& x, const std::vector<double>& y);
	extern double power(const std::vector<double>& x, const std::vector<double>& y);
	extern double logistic(const std::vector<double>& x, const std::vector<double>& y);
	extern double sinusoidal(const std::vector<double>& x, const std::vector<double>& y);
}

#endif