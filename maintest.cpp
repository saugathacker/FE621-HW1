#include "util.h"
#include <iostream>

using namespace std;

// Function to test Normal CDF against Z-table values
void test_norm_cdf()
{
    std::cout << "Testing norm_cdf against Z-table values:\n";

    // Sample Z-table values (Z, Expected CDF)
    struct TestCase
    {
        double z;
        double expected_cdf;
    };

    TestCase testCases[] = {
        {0.0, 0.5000},
        {0.5, 0.6915},
        {1.0, 0.8413},
        {1.5, 0.9332},
        {2.0, 0.9772},
        {2.5, 0.9938},
        {3.0, 0.9987},
        {-0.5, 0.3085},
        {-1.0, 0.1587},
        {-1.5, 0.0668},
        {-2.0, 0.0228},
        {-2.5, 0.0062},
        {-3.0, 0.0013}};

    for (const auto &test : testCases)
    {
        double computed_cdf = norm_cdf(test.z);
        std::cout << "Z = " << test.z
                  << " | Computed CDF = " << computed_cdf
                  << " | Expected CDF = " << test.expected_cdf
                  << " | Error = " << std::fabs(computed_cdf - test.expected_cdf)
                  << "\n";
    }
}

// Function to test Normal PDF values
void test_norm_pdf()
{
    std::cout << "\nTesting norm_pdf:\n";

    // Expected values from standard normal PDF formula
    struct TestCase
    {
        double x;
        double expected_pdf;
    };

    TestCase testCases[] = {
        {0.0, 0.39894},
        {0.5, 0.35206},
        {1.0, 0.24197},
        {1.5, 0.12952},
        {2.0, 0.05399},
        {2.5, 0.01753},
        {3.0, 0.00443},
        {-0.5, 0.35206},
        {-1.0, 0.24197},
        {-1.5, 0.12952},
        {-2.0, 0.05399},
        {-2.5, 0.01753},
        {-3.0, 0.00443}};

    for (const auto &test : testCases)
    {
        double computed_pdf = norm_pdf(test.x);
        std::cout << "X = " << test.x
                  << " | Computed PDF = " << computed_pdf
                  << " | Expected PDF = " << test.expected_pdf
                  << " | Error = " << std::fabs(computed_pdf - test.expected_pdf)
                  << "\n";
    }
}

int main()
{

    // testing the implemntation of norm_cdf and norm_pdf
    test_norm_cdf();
    test_norm_pdf();

    return 0;
}