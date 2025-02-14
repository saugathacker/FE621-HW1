#include "OptionData.h"
#include <iostream>
#include <cmath>
#include "BlackScholes.h"

void OptionData::calculate_iv_and_greeks(double spotPrice, double interestRate)
{
    // **Skip calculation if lastPrice, bid, and ask are all zero**
    if ((lastPrice <= 0.0 || std::isnan(lastPrice)) &&
        (bid <= 0.0 || std::isnan(bid)) &&
        (ask <= 0.0 || std::isnan(ask)))
    {
        return; // Do not calculate IV if no valid market data exists
    }

    // **Use mid-price if bid/ask exist, otherwise use last price**
    double market_price = (bid > 0 && ask > 0) ? (bid + ask) / 2 : lastPrice;

    // **Create Black-Scholes model instance**
    BlackScholes bs(strike, spotPrice, timeToMaturity, interestRate,
                    (optionType == "Call" ? PayoffType::Call : PayoffType::Put));

    // std::cout << bs << std::endl;
    // std::cout << "Finding root with market price: " << market_price << std::endl;
    //**Measure time for Bisection Method**
    auto start_bisect = std::chrono::high_resolution_clock::now();
    bisectionImpliedVol = bisection_method(bs, market_price);
    auto end_bisect = std::chrono::high_resolution_clock::now();
    bisectionTime = std::chrono::duration<double, std::milli>(end_bisect - start_bisect).count();

    // **Measure time for Newton's Method**
    auto start_newton = std::chrono::high_resolution_clock::now();
    newtonImpliedVol = newton_method(bs, market_price);
    auto end_newton = std::chrono::high_resolution_clock::now();
    newtonTime = std::chrono::duration<double, std::milli>(end_newton - start_newton).count();

    // calculating greeks using BlackScholes derivation
    delta_bs = bs.get_delta(bisectionImpliedVol);
    gamma_bs = bs.get_gamma(bisectionImpliedVol);
    vega_bs = bs.get_vega(bisectionImpliedVol);

    // calculating greeks using Finite Difference method
    delta_fd = delta_finite_difference(bs, bisectionImpliedVol);
    gamma_fd = gamma_finite_difference(bs, bisectionImpliedVol);
    vega_fd = vega_finite_difference(bs, bisectionImpliedVol);
}

void OptionData::calculate_bs_price(double spot, double rate, double vol)
{
    BlackScholes bs_model(strike, spot, timeToMaturity, rate,
                          (optionType == "Call" ? PayoffType::Call : PayoffType::Put), 0.02);

    bs_price = bs_model(vol);
}