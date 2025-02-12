#pragma once
#include <array>

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
    double operator()(double vol);
    double get_delta() const;
    double get_gamma() const;
    double get_vega() const;

private:
    double spot_, strike_, time_to_maturity_;
    double interest_rate_, dividend_yield_;
    PayoffType payoff_type_;
    std::array<double, 2> compute_norm_args_(double vol);
};