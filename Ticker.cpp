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

// Find an option based on strike, expiration, and type
OptionData *Ticker::findOption(double strike, const std::string &expiration, const std::string &optionType) const
{
    for (const auto &option : options)
    {
        if (option->strike == strike && option->expiration == expiration && option->optionType == optionType)
        {
            return option.get(); // Return raw pointer (safe since unique_ptr manages memory)
        }
    }
    return nullptr; // Return nullptr if no matching option is found
}

void Ticker::calculate_implied_vols_and_greeks()
{
    for (auto &option : options)
    {
        option->calculate_iv_and_greeks(spotPrice, interestRate); // Each option calculates its IV
    }
}

void Ticker::calculate_put_call_parity()
{
    for (const auto &option : options)
    {
        // Find corresponding Call if current option is a Put
        if (option->optionType == "Put")
        {
            OptionData *callOption = findOption(option->strike, option->expiration, "Call");
            if (callOption)
            {
                double discount_factor = exp(-interestRate * option->timeToMaturity);
                option->parity_price = callOption->lastPrice - spotPrice + (option->strike * discount_factor);
            }
        }
        // Find corresponding Put if current option is a Call
        else if (option->optionType == "Call")
        {
            OptionData *putOption = findOption(option->strike, option->expiration, "Put");
            if (putOption)
            {
                double discount_factor = exp(-interestRate * option->timeToMaturity);
                option->parity_price = putOption->lastPrice + spotPrice - (option->strike * discount_factor);
            }
        }
    }
}

void Ticker::calculate_bs_price_from_other_ticker(const std::unique_ptr<Ticker> &tickerData1)
{
    size_t i = 0, j = 0;

    while (i < options.size() && j < tickerData1->options.size())
    {
        OptionData *myOption = options[i].get();
        OptionData *otherOption = tickerData1->options[j].get();

        // If options match by strike, expiration, and type
        if (myOption->strike == otherOption->strike &&
            myOption->expiration == otherOption->expiration &&
            myOption->optionType == otherOption->optionType)
        {
            // Use Black-Scholes model with other ticker's IV
            if (otherOption->bisectionImpliedVol > 0)
            {
                myOption->calculate_bs_price(spotPrice, interestRate, otherOption->bisectionImpliedVol);
            }

            // Move both indices forward
            i++;
            j++;
        }
        else
        {
            // If not matched, find the corresponding option in `tickerData1`
            otherOption = tickerData1->findOption(myOption->strike, myOption->expiration, myOption->optionType);

            if (otherOption && otherOption->bisectionImpliedVol > 0)
            {
                myOption->calculate_bs_price(spotPrice, interestRate, otherOption->bisectionImpliedVol);
            }

            // Move forward only the iterator for `this` Ticker
            i++;
        }
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
         << "Delta_bs,Gamma_bs,Vega_bs,Delta_fd,Gamma_fd,Vega_fd,Parity_price,Bs_price,InTheMoney\n";

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
             << option->parity_price << ","
             << option->bs_price << ","
             << (option->inTheMoney ? "True" : "False") << "\n";
    }

    file.close();
    std::cout << "CSV file written successfully: " << filename << std::endl;
}