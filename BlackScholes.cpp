#include "BlackScholes.h"
#include "util.h"

// Constructor Implementation
BlackScholes::BlackScholes(double strike, double spot, double time_to_maturity,
                           double interest_rate, PayoffType payoff_type, double dividend_yield)
    : strike_(strike), spot_(spot), time_to_maturity_(time_to_maturity),
      interest_rate_(interest_rate), dividend_yield_(dividend_yield), payoff_type_(payoff_type) {}

// Function Implementation
double BlackScholes::operator()(double vol) const
{

    using std::exp;
    auto norm_args = compute_norm_args_(vol);
    double d1 = norm_args[0];
    double d2 = norm_args[1];

    int phi = static_cast<int>(payoff_type_);

    double nD1 = norm_cdf(phi * d1);
    double nD2 = norm_cdf(phi * d2);

    double discountFactor = exp(-interest_rate_ * time_to_maturity_);
    double dividendFactor = exp(-dividend_yield_ * time_to_maturity_);

    double payoff = phi * (spot_ * dividendFactor * nD1 - strike_ * discountFactor * nD2);

    return payoff;
}

// Helper function to compute d1 and d2
std::array<double, 2> BlackScholes::compute_norm_args_(double vol) const
{

    double log_term = std::log(spot_ / strike_);

    double drift_term = (interest_rate_ - dividend_yield_ + 0.5 * vol * vol) * time_to_maturity_;

    double denom = vol * std::sqrt(time_to_maturity_);

    double d1 = (log_term + drift_term) / denom;

    double d2 = d1 - denom;

    return {d1, d2}; // Explicitly constructing std::array
}

double BlackScholes::get_delta(double vol) const
{
    auto norm_args = compute_norm_args_(vol);
    double d1 = norm_args[0];
    double nD1 = norm_cdf(d1);

    return (payoff_type_ == PayoffType::Call ? nD1 : nD1 - 1);
}

double BlackScholes::get_gamma(double vol) const
{
    auto norm_args = compute_norm_args_(vol);
    double d1 = norm_args[0];
    double nD1 = norm_pdf(d1);

    return nD1 / (spot_ * vol * ::sqrt(time_to_maturity_));
}

double BlackScholes::get_vega(double vol) const
{
    auto norma_args = compute_norm_args_(vol);
    double d1 = norma_args[0];
    double nD1 = norm_pdf(d1);

    return spot_ * std::sqrt(time_to_maturity_) * nD1;
}

std::ostream &operator<<(std::ostream &os, const BlackScholes &bs)
{
    os << "Black-Scholes Model:\n"
       << "  Spot Price: " << bs.spot_ << "\n"
       << "  Strike Price: " << bs.strike_ << "\n"
       << "  Time to Maturity: " << bs.time_to_maturity_ << "\n"
       << "  Interest Rate: " << bs.interest_rate_ << "\n"
       << "  Dividend Yield: " << bs.dividend_yield_ << "\n"
       << "  Payoff Type: " << (bs.payoff_type_ == PayoffType::Call ? "Call" : "Put") << "\n";
    return os;
}