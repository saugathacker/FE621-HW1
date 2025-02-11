#include "BlackScholes.h"
#include <numbers>
#include <cmath>
#include <iostream>

// Constructor Implementation
BlackScholes::BlackScholes(double strike, double spot, double time_to_maturity,
                           double interest_rate, PayoffType payoff_type, double dividend_yield)
    : strike_(strike), spot_(spot), time_to_maturity_(time_to_maturity),
      interest_rate_(interest_rate), dividend_yield_(dividend_yield), payoff_type_(payoff_type) {}

// Function Implementation
double BlackScholes::operator()(double vol)
{

    using std::exp;
    auto norm_args = compute_norm_args_(vol);
    double d1 = norm_args[0];
    double d2 = norm_args[1];

    auto norm_cdf = [](double x)
    {
        return (1.0 + std::erf(x / std::numbers::sqrt2)) / 2;
    };

    double nD1 = norm_cdf(d1);
    double nD2 = norm_cdf(d2);

    int phi = static_cast<int>(payoff_type_);

    double payoff = phi * (spot_ * exp(-dividend_yield_ * time_to_maturity_)) * (phi * nD1) - (strike_ * exp(-interest_rate_ * time_to_maturity_)) * (phi * nD2);

    return payoff;
}

// Helper function to compute d1 and d2
std::array<double, 2> BlackScholes::compute_norm_args_(double vol)
{

    double log_term = std::log(spot_ / strike_);

    double drift_term = (interest_rate_ - dividend_yield_ + 0.5 * vol * vol) * time_to_maturity_;

    double denom = vol * std::sqrt(time_to_maturity_);

    double d1 = (log_term + drift_term) / denom;

    double d2 = d1 - denom;

    return {d1, d2}; // Explicitly constructing std::array
}
