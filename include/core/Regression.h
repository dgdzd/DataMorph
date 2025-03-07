#ifndef DM_REGRESSION_H
#define DM_REGRESSION_H

#include <string>
#include <vector>

namespace Regression {
	//done
	extern bool linear(const std::vector<double>& x, const std::vector<double>& y, double& w1);
	extern bool affine(const std::vector<double>& x, const std::vector<double>& y, double& w0, double& w1);
	extern bool custom(const std::vector<double>& xs, const std::vector<double>& ys, std::string model, double& a, double& b, double& c, std::string x_str);
	extern bool quadratic(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c);
	extern bool cubic(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c, double& d);
	extern bool sinusoidal(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c);
	extern bool logarithmic(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b, double& n);
	extern bool exponential(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b, double& n);
	extern bool sqrt(const std::vector<double>& xs, const std::vector<double>& ys, double& a, double& b, double& c);
	extern bool inverse(const std::vector<double>& xs, const std::vector<double>& ys, double& a);
	extern bool lituus(const std::vector<double>& xs, const std::vector<double>& ys, double& a);
	extern bool cochleoid(const std::vector<double>& xs, const std::vector<double>& ys, double& a);
	//to do
	extern double logistic(const std::vector<double>& x, const std::vector<double>& y);
}

#endif