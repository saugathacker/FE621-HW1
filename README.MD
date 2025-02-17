# **Options Pricing and Implied Volatility Estimation**

## **Introduction**

This repository contains an implementation of option pricing, implied volatility estimation, and numerical integration methods using C++ and Python. The code includes methods for solving Black-Scholes pricing, computing option Greeks, and analyzing options market data.

## **Project Structure**

### **C++ Files (Core Implementation)**

- **BlackScholes.cpp / BlackScholes.h** – Implements the Black-Scholes pricing model and computes Greeks (Delta, Gamma, Vega).
- **OptionData.cpp / OptionData.h** – Defines a structure for storing individual option contract data and methods to compute implied volatility.
- **Ticker.cpp / Ticker.h** – Manages a collection of OptionData objects for a specific ticker (e.g., NVDA, SPY).
- **util.cpp / util.h** – Contains helper functions, including root-finding methods (Bisection, Newton, Secant), numerical integration, and normal distribution functions.
- **computation.cpp** – The main driver file that loads data, computes implied volatilities, Greeks, and performs numerical integration tests.
- **maintest.cpp** – A separate testing file for verifying implementations.

### **Python Files (Data Handling & Visualization)**

- **download-data.ipynb** – A Jupyter Notebook for downloading and preparing options data.
- **visualize_data.ipynb** – A Jupyter Notebook for plotting results and analyzing trends in implied volatility.

### **Data Files**

- **options_data1.csv / options_data2.csv** – Raw input data files containing options data (strike, bid, ask, market price, etc.).
- **NVDA_outputData1.csv / NVDA_outputData2.csv** – Processed data for NVDA options, including computed implied volatilities.
- **SPY_outputData1.csv / SPY_outputData2.csv** – Processed data for SPY options.
- **^VIX_outputData1.csv / ^VIX_outputData2.csv** – Processed data for the VIX index.

## **Build System**

- **CMakeLists.txt** – Configuration file for building the project using CMake.

## **How to Run the Code**

### **C++ Implementation**

Build the project using CMake:

```sh
cmake -Bbuild
cmake --build build
```

Run the compiled executable:

```sh
./build/main
```

### **Python Notebooks**

To run the Jupyter Notebooks:

1. Open a terminal and start Jupyter Notebook:
   ```sh
   jupyter notebook
   ```
2. Open and execute **download-data.ipynb** to get options data.
3. Open and execute **visualize_data.ipynb** to analyze results.

## **Dependencies**

### **C++ Requirements:**

- C++20 or later
- CMake
- Standard math libraries (cmath, numbers)

### **Python Requirements:**

- pandas
- numpy
- matplotlib
- scipy
- jupyter

Install required Python packages using:

```sh
pip install pandas numpy matplotlib scipy jupyter
```
