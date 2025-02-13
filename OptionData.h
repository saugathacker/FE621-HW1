#ifndef OPTIONDATA_H
#define OPTIONDATA_H

#include <string>
#include "util.h"
#include <chrono>

struct OptionData
{
    std::string expiration;
    double timeToMaturity;
    double strike;
    std::string optionType;
    double lastPrice;
    double bid;
    double ask;
    double volume;
    double openInterest;
    double impliedVolatility;
    bool inTheMoney;

    double bisectionImpliedVol;
    double newtonImpliedVol;
    double bisectionTime; // Time taken for Bisection
    double newtonTime;

    double delta_bs;
    double gamma_bs;
    double vega_bs;

    double delta_fd;
    double gamma_fd;
    double vega_fd;

    // Constructor for initialization
    OptionData(const std::string &exp, double ttm, double strk, const std::string &type,
               double lp, double b, double a, double vol, double oi, double iv, bool itm)
        : expiration(exp), timeToMaturity(ttm), strike(strk), optionType(type),
          lastPrice(lp), bid(b), ask(a), volume(vol), openInterest(oi),
          impliedVolatility(iv), inTheMoney(itm), bisectionImpliedVol(0), newtonImpliedVol(0) {}

    // Calculate IV using both Bisection and Newton methods
    void calculate_iv_and_greeks(double spotPrice, double interestRate);
};

#endif