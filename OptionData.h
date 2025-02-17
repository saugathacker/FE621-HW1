#ifndef OPTIONDATA_H
#define OPTIONDATA_H

#include <string>
#include "util.h"
#include <chrono>

// struct to hold all the information of the option chain and the calculated IVs and Greeks
struct OptionData
{
  // information from the downloaded data
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

  // calculated Implied Vol
  double bisectionImpliedVol;
  double newtonImpliedVol;
  double secantImpliedVol;
  double bisectionTime;
  double newtonTime;
  double secantTime;

  // calculated greeks using diff approach
  double delta_bs;
  double gamma_bs;
  double vega_bs;

  double delta_fd;
  double gamma_fd;
  double vega_fd;

  // calculated parity and bs price
  double parity_price;
  double bs_price;

  // Constructor for initialization
  OptionData(const std::string &exp, double ttm, double strk, const std::string &type,
             double lp, double b, double a, double vol, double oi, double iv, bool itm)
      : expiration(exp), timeToMaturity(ttm), strike(strk), optionType(type),
        lastPrice(lp), bid(b), ask(a), volume(vol), openInterest(oi),
        impliedVolatility(iv), inTheMoney(itm), bisectionImpliedVol(0), newtonImpliedVol(0), bisectionTime(0), newtonTime(0),
        delta_bs(0), gamma_bs(0), vega_bs(0), delta_fd(0), gamma_fd(0), vega_fd(0) {}

  void calculate_iv_and_greeks(double spotPrice, double interestRate);
  void calculate_bs_price(double spot, double rate, double vol);
};

#endif