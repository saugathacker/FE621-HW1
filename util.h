#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <numbers>
#include <functional>
#include <string>

class BlackScholes;

// Normal CDF function (declaration)
double norm_cdf(double x);

// Normal PDF function
double norm_pdf(double x);

double bisection_method(BlackScholes &bs, double market_price, bool debug = false);

double newton_method(BlackScholes &bs, double market_price);

double secant_method(BlackScholes &bs, double market_price);

double delta_finite_difference(BlackScholes &bs, double vol);

double gamma_finite_difference(BlackScholes &bs, double vol);

double vega_finite_difference(BlackScholes &bs, double vol);

double trapezoidal_rule(std::function<double(double)>, double a, double b, int N);

double simpsons_rule(std::function<double(double)> f, double a, double b, int N);

double truncation_error(std::string method,
                        std::function<double(double)> f, double a, double b, int N);

int convergence_iterations(std::string method,
                           std::function<double(double)> f, double a, double b, double epsilon = 1e-4);

double double_trapezoidal_rule(std::function<double(double, double)> f,
                               double a, double b, int dx,
                               double c, double d, int dy);

#endif // UTIL_H
