#include "Ticker.h"
#include "util.h"
#include <iostream>
#include <functional>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <unordered_map> // For fast lookup of existing tickers

using namespace std;

int main()
{

    // // test bisection method
    // BlackScholes bs(130, 130.694, 0.021918, 0.0423,
    //                 PayoffType::Put);
    // cout << "Option Price: " << bs(0.46) << endl;
    // double market = 3.175;
    // cout << "Market Price: " << market << endl;

    // double iv = bisection_method(bs, market, true);

    // cout << "Bisection test: " << iv << endl;

    // ifstream ifile("options_data2.csv", ios::in);
    // if (!ifile.is_open())
    // {
    //     cout << "Error opening file for input!" << endl;
    //     return 1;
    // }

    // std::unordered_map<std::string, std::unique_ptr<Ticker>> tickers; // Map to store unique tickers

    // string line_;
    // int counter = 0;

    // // getting rid of headers
    // getline(ifile, line_);

    // while (getline(ifile, line_))
    // {
    //     stringstream ss(line_);
    //     string temp;

    //     string ticker, expiration, optionType;
    //     double timeToMaturity, strike, lastPrice, bid, ask, volume, openInterest, impliedVolatility, spotPrice, interestRate;
    //     bool inTheMoney;

    //     std::getline(ss, temp, ','); // Ignore first empty column
    //     std::getline(ss, ticker, ',');
    //     std::getline(ss, expiration, ',');
    //     std::getline(ss, temp, ',');
    //     timeToMaturity = std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     strike = std::stod(temp);
    //     std::getline(ss, optionType, ',');
    //     std::getline(ss, temp, ',');
    //     lastPrice = temp.empty() ? 0.0 : std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     bid = temp.empty() ? 0.0 : std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     ask = temp.empty() ? 0.0 : std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     volume = temp.empty() ? 0.0 : std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     openInterest = temp.empty() ? 0.0 : std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     impliedVolatility = temp.empty() ? 0.0 : std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     inTheMoney = (temp == "True");
    //     std::getline(ss, temp, ',');
    //     spotPrice = std::stod(temp);
    //     std::getline(ss, temp, ',');
    //     interestRate = std::stod(temp);
    //     interestRate = interestRate / 100;

    //     if (tickers.find(ticker) == tickers.end())
    //     {
    //         tickers[ticker] = make_unique<Ticker>(ticker, spotPrice, interestRate);
    //     }

    //     // Create a new OptionData object
    //     auto option = std::make_unique<OptionData>(expiration, timeToMaturity, strike, optionType,
    //                                                lastPrice, bid, ask, volume, openInterest, impliedVolatility, inTheMoney);

    //     // Add option data to the existing Ticker object
    //     tickers[ticker]->addOptionData(std::move(option));
    // }

    // ifile.close();

    // for (const auto &[ticker, tickerObj] : tickers)
    // {
    //     cout << "ticker name: " << tickerObj->getTickerName() << " no of options: " << tickerObj->getOptionsSize() << endl;
    // }

    // tickers["SPY"]->calculate_implied_vols_and_greeks();
    // tickers["SPY"]->write_to_csv("output.csv");

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
    int N = 1000000;

    cout << "trapezoidal rule integral approx: " << trapezoidal_rule(real_valued_func, a_minus, a_plus, N) << endl;
    cout << "truncation error of trapezoidal rule: " << truncation_error("trapezoidal", real_valued_func, a_minus, a_plus, N) << endl;
    cout << "convergence test took " << convergence_iterations("trapezoidal", real_valued_func, a_minus, a_plus) << " iterations" << endl;
    cout << "simpsons rule integral approx: " << simpsons_rule(real_valued_func, a_minus, a_plus, N) << endl;
    cout << "truncation error of simpsons rule: " << truncation_error("simpsons", real_valued_func, a_minus, a_plus, N) << endl;
    cout << "convergence test took " << convergence_iterations("simpsons", real_valued_func, a_minus, a_plus) << " iterations" << endl;

    return 0;
}