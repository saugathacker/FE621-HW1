#include "util.h"
#include "BlackScholes.h"

constexpr double h = 0.0001;

// Implementation of the Normal CDF function
double norm_cdf(double x)
{
    return (1.0 + std::erf(x / std::numbers::sqrt2)) / 2;
}

// Implementation of the Normal PDF function
double norm_pdf(double x)
{
    return std::exp(-0.5 * x * x) / std::sqrt(2 * std::numbers::pi);
}

double bisection_method(BlackScholes &bs, double market_price, bool debug)
{
    double a = 0.0001;
    double b = 3.0;
    double c = (a + b) / 2;
    double epsilon = 1e-06;
    double tol = bs(c) - market_price;
    int max_iter = 1000;
    int iter = 0;

    // **Check if the root is even bracketed**
    double fa = bs(a) - market_price;
    double fb = bs(b) - market_price;

    if (debug)
        std::cout << "bs(a) " << bs(a) << "bs(b) " << bs(b) << std::endl;

    if (fa * fb > 0)
    {
        if (debug)
            std::cout << "Warning: Root is not in range! Returning best estimate.\n";
        return std::min(std::max(epsilon, a), b);
    }

    if (debug)
        std::cout << "Starting Bisection Method...\n";

    while (std::abs(tol) > epsilon && iter < max_iter)
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
        iter++;

        // **Optional debug logging**
        if (debug && iter % 10 == 0)
        {
            std::cout << "Iteration " << iter << ": IV estimate = " << c << " | Tolerance = " << tol << std::endl;
        }
    }

    if (debug)
        std::cout << "Ending Bisection Method after " << iter << " iterations.\n";

    // **Ensure valid output within IV range**
    c = std::max(epsilon, std::min(c, b));

    return c;
}

double newton_method(BlackScholes &bs, double market_price)
{
    double sigma = 0.2; // Initial guess
    double epsilon = 1e-06;
    int max_iter = 100;

    for (int i = 0; i < max_iter; ++i)
    {
        double price = bs(sigma);
        double vega = bs.get_vega(sigma);

        if (std::abs(price - market_price) < epsilon || vega <= 0.0)
        {
            return sigma;
        }

        sigma -= (price - market_price) / vega;
    }

    return sigma; // Return the last computed sigma
}

double delta_finite_difference(BlackScholes &bs, double vol)
{
    double S = bs.get_spot();

    BlackScholes bs_plus(bs.get_strike(), S + h, bs.get_time_to_maturity(), bs.get_interest_rate(), bs.get_payoff_type(), 0.0);
    BlackScholes bs_minus(bs.get_strike(), S - h, bs.get_time_to_maturity(), bs.get_interest_rate(), bs.get_payoff_type(), 0.0);

    return (bs_plus(vol) - bs_minus(vol)) / (2 * h);
}

double gamma_finite_difference(BlackScholes &bs, double vol)
{
    double S = bs.get_spot();

    BlackScholes bs_plus(bs.get_strike(), S + h, bs.get_time_to_maturity(), bs.get_interest_rate(), bs.get_payoff_type(), 0.0);
    BlackScholes bs_minus(bs.get_strike(), S - h, bs.get_time_to_maturity(), bs.get_interest_rate(), bs.get_payoff_type(), 0.0);

    double delta_plus = delta_finite_difference(bs_plus, vol);
    double delta_minus = delta_finite_difference(bs_minus, vol);

    return (delta_plus - delta_minus) / (2 * h);
}

double vega_finite_difference(BlackScholes &bs, double vol)
{
    return (bs(vol + h) - bs(vol - h)) / (2 * h);
}

double trapezoidal_rule(std::function<double(double)> f, double a, double b, int N)
{

    int n = N;
    double h = (b - a) / n;
    double sum = 0.5 * (f(a) + f(b));

    for (int i = 1; i < n; i++)
    {
        double x = a + i * h;
        sum += f(x);
    }

    return h * sum;
}

double simpsons_rule(std::function<double(double)> f, double a, double b, int N)
{
    // requires even intervals
    int n = N;
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

double truncation_error(std::string method,
                        std::function<double(double)> f, double a, double b, int N)
{
    double integral_value;
    if (method == "trapezoidal")
    {
        integral_value = trapezoidal_rule(f, a, b, N);
    }
    else if (method == "simpsons")
    {
        integral_value = simpsons_rule(f, a, b, N);
    }

    return std::abs(integral_value - M_PI);
}

int convergence_iterations(std::string method,
                           std::function<double(double)> f, double a, double b, double epsilon)
{
    int N = 10; // Start with a very small number of intervals
    double prev_result;

    if (method == "trapezoidal")
    {
        prev_result = trapezoidal_rule(f, a, b, N);
    }
    else if (method == "simpsons")
    {
        prev_result = simpsons_rule(f, a, b, N);
    }
    else
    {
        std::cerr << "Error: Invalid method name '" << method << "'\n";
        return -1;
    }

    double current_result;
    int iterations = 0;

    while (true)
    {
        N *= 2; // Increase intervals by 500 each iteration
        iterations++;

        if (method == "trapezoidal")
        {
            current_result = trapezoidal_rule(f, a, b, N);
        }
        else if (method == "simpsons")
        {
            current_result = simpsons_rule(f, a, b, N);
        }

        double error = std::abs(current_result - prev_result);

        if (error < epsilon)
        {
            std::cout << "Converged in " << iterations << " iterations with final N = " << N << "\n";
            return iterations; // Return the number of iterations needed
        }

        prev_result = current_result;
    }
}
