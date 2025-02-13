#pragma once
#include <string>
#include <vector>
#include <memory>
#include "OptionData.h"

// Class to store and manage options for a specific ticker
class Ticker
{
private:
    std::string tickerName; // Stock ticker symbol
    double spotPrice;
    double interestRate;
    std::vector<std::unique_ptr<OptionData>> options; // Unique ptr vector of OptionData

public:
    // Constructor
    Ticker(const std::string &name, double spot, double rate);

    // Add option data to the existing Ticker object
    void addOptionData(std::unique_ptr<OptionData> option);

    // Getter for ticker name
    std::string getTickerName() const { return tickerName; }
    double getOptionsSize() const { return options.size(); };

    // Find and return a pointer to the OptionData that matches strike and type
    OptionData *findOption(double strike, const std::string &expiration, const std::string &optionType) const;

    void calculate_implied_vols_and_greeks();
    void calculate_put_call_parity();

    double getSpotPrice() const { return spotPrice; };
    double getInterestRate() const { return interestRate; };

    // **Function to write all options to a CSV file**
    void write_to_csv(const std::string &filename) const;
};