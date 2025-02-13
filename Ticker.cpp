#include "Ticker.h"
#include "util.h"
#include <fstream>
#include <iostream>

// Constructor
Ticker::Ticker(const std::string &name, double spot, double rate)
    : tickerName(name), spotPrice(spot), interestRate(rate) {}

// Add new option data to the existing Ticker object
void Ticker::addOptionData(std::unique_ptr<OptionData> option)
{
    options.push_back(std::move(option));
}

// Find an option based on strike price and option type
OptionData *Ticker::findOption(double strike, const std::string &optionType) const
{
    for (const auto &option : options)
    {
        if (option->strike == strike && option->optionType == optionType)
        {
            return option.get(); // Return raw pointer (safe since unique_ptr manages memory)
        }
    }
    return nullptr; // Return nullptr if not found
}

void Ticker::calculate_implied_vols_and_greeks()
{
    for (auto &option : options)
    {
        option->calculate_iv_and_greeks(spotPrice, interestRate); // Each option calculates its IV
    }
}

void Ticker::write_to_csv(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // **Write CSV Header**
    file << "Ticker,Expiration,TimeToMaturity,Strike,OptionType,LastPrice,"
         << "Bid,Ask,Volume,OpenInterest,ImpliedVolatility,BisectionIV,NewtonIV,"
         << "Delta_bs,Gamma_bs,Vega_bs,Delta_fd,Gamma_fd,Vega_fd,InTheMoney\n";

    // **Write Option Data**
    for (const auto &option : options)
    {
        file << tickerName << "," // Add ticker symbol
             << option->expiration << ","
             << option->timeToMaturity << ","
             << option->strike << ","
             << option->optionType << ","
             << option->lastPrice << ","
             << option->bid << ","
             << option->ask << ","
             << option->volume << ","
             << option->openInterest << ","
             << option->impliedVolatility << ","
             << option->bisectionImpliedVol << ","
             << option->newtonImpliedVol << ","
             << option->delta_bs << ","
             << option->gamma_bs << ","
             << option->vega_bs << ","
             << option->delta_fd << ","
             << option->gamma_fd << ","
             << option->vega_fd << ","
             << (option->inTheMoney ? "True" : "False") << "\n";
    }

    file.close();
    std::cout << "CSV file written successfully: " << filename << std::endl;
}