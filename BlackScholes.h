#pragma once
#include <array>
#include <numbers>
#include <cmath>
#include <iostream>

enum class PayoffType
{
    Call = 1,
    Put = -1
};

class BlackScholes
{
public:
    BlackScholes(double strike, double spot, double time_to_maturity,
                 double interest_rate, PayoffType payoff_type, double dividend_yield = 0);
    double operator()(double vol) const;
    double get_delta(double vol) const;
    double get_gamma(double vol) const;
    double get_vega(double vol) const;

    double get_spot() const { return spot_; }
    double get_strike() const { return strike_; }
    double get_time_to_maturity() const { return time_to_maturity_; }
    double get_interest_rate() const { return interest_rate_; }
    PayoffType get_payoff_type() const { return payoff_type_; }

private:
    double spot_, strike_, time_to_maturity_;
    double interest_rate_, dividend_yield_;
    PayoffType payoff_type_;
    std::array<double, 2> compute_norm_args_(double vol) const;
    friend std::ostream &operator<<(std::ostream &os, const BlackScholes &bs);
};