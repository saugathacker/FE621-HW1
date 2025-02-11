#include "BlackScholes.h"
#include <iostream>
#include <functional>

using namespace std;

double bisection_method(BlackScholes &bs, double market_price)
{
    // using bisection method to find implied vol
    double a = 0.001;
    double b = 3.0;
    double c = (a + b) / 2;
    double epsilon = 1e-06;
    double tol = 10;
    while (std::abs(tol) > 0.000001)
    {
        tol = bs(c) - market_price;
        if (tol < 0)
        {
            a = c;
        }
        else
        {
            b = c;
        }
        c = (a + b) / 2;
    }

    return c;
}

double trapezoidal_rule(function<double(double)> f, double a, double b)
{

    int n = 1000000;
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));

    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += f(x);
    }

    return h * sum;
}

double simpsons_rule(function<double(double)> f, double a, double b)
{
    // requires even intervals
    int n = 1000000;
    double h = (b - a) / n;
    double sum = f(a) + f(b);

    // Accumulate Simpson's weighted sum
    for (int i = 1; i < n; i++)
    {
        double x = a + i * h; // Compute current x value

        // Apply Simpson's weighting: 4 for odd indices, 2 for even indices
        sum += (i % 2 == 0) ? 2 * f(x) : 4 * f(x);
    }

    // Finalize the integral value with the Simpson’s Rule formula
    return (h / 3) * sum;
}

int main()
{

    double spot = 129.84;
    double strike = 130;
    double rate = 0.04;
    double div = 0.03;
    double time = 5.0 / 252.0;
    double vol = 0.4;
    PayoffType payoff = PayoffType::Call;

    // iterate over each dates
    // iterate all strike for one of the many expiration dates
    // iterate over all calls and then put for that strike
    // just change the time, strike, and payoff for the same object and pass the object to bisection method
    // store the atm call and put iv
    // average the in the money ivs and average the out of the money ivs

    // BlackScholes bs{strike, spot, time, rate, payoff, div};

    // std::cout << bs(vol) << std::endl;

    // double market = 3.43;
    // // using bisection method to find implied vol
    // double c = bisection_method(bs, market);

    // std::cout
    //     << "Implied vol = " << c << std::endl;
    // std::cout << "With calculated IV call = " << bs(c) << std::endl;
    // std::cout << "Market call = " << market << std::endl;

    // trapezoidal rule
    auto real_valued_func = [](double x)
    {
        if (x == 0.0)
        {
            return 1.0;
        }
        else
        {
            return (sin(x) / x);
        }
    };

    // bounds for large interval
    double a_plus = pow(10, 6);
    double a_minus = -a_plus;

    cout << "trapezoidal rule integral approx: " << trapezoidal_rule(real_valued_func, a_minus, a_plus) << endl;
    cout << "simpsons rule integral approx: " << simpsons_rule(real_valued_func, a_minus, a_plus) << endl;
    return 0;
}