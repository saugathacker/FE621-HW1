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

// Bisection Method
double bisection_method(BlackScholes &bs, double market_price, bool debug = false);

// Newton's Method
double newton_method(BlackScholes &bs, double market_price);

// Secant Method
double secant_method(BlackScholes &bs, double market_price);

// Calculate Delta using Finite Difference
double delta_finite_difference(BlackScholes &bs, double vol);

// Calculate Gamma using Finite Difference
double gamma_finite_difference(BlackScholes &bs, double vol);

// Calculate Vega using Finite Difference
double vega_finite_difference(BlackScholes &bs, double vol);

// Trapezoidal Rule
double trapezoidal_rule(std::function<double(double)>, double a, double b, int N);

// Simpson's Rule
double simpsons_rule(std::function<double(double)> f, double a, double b, int N);

// Truncation Error
double truncation_error(std::string method,
                        std::function<double(double)> f, double a, double b, int N);

// Convergence Test
int convergence_iterations(std::string method,
                           std::function<double(double)> f, double a, double b, double epsilon = 1e-4);

// double Trapezoidal Rule
double double_trapezoidal_rule(std::function<double(double, double)> f,
                               double a, double b, int dx,
                               double c, double d, int dy);

#endif // UTIL_H
