#include <gtest/gtest.h>
#include "Rates.hpp"


TEST(RateTests, ConstructorWithN_ValidInput) {
    Rates rate(0.05, 0.1, 10);  
    EXPECT_NEAR(rate.at(0.0), 0.05, 1e-6);
    EXPECT_NEAR(rate.at(0.1), 0.055, 1e-6);
    EXPECT_NEAR(rate.at(0.9), 0.095, 1e-6);
}

TEST(RateTests, ConstructorWithDt_ValidInput) {
    Rates rate(0.05, 0.1, 0.1);  
    EXPECT_NEAR(rate.at(0.0), 0.05, 1e-6);
    EXPECT_NEAR(rate.at(0.1), 0.055, 1e-6);
    EXPECT_NEAR(rate.at(0.9), 0.095, 1e-6);
}

TEST(RateTests, ConstructorWithRates) {
    std::unordered_map<double, double> rates = { {0.0, 0.05}, {0.5, 0.07}, {1.0, 0.1} };
    Rates rate(rates);

    EXPECT_NEAR(rate.at(0.0), 0.05, 1e-6);
    EXPECT_NEAR(rate.at(0.5), 0.07, 1e-6);
    EXPECT_NEAR(rate.at(1.0), 0.1, 1e-6);
}

TEST(RateTests, AtMethod_OutOfRange) {
    Rates rate(0.05, 0.1, 0.1);
    EXPECT_THROW(rate.at(-0.1), std::out_of_range);
    EXPECT_THROW(rate.at(1.1), std::out_of_range);
}

TEST(RateTests, AtMethod_MissingRate) {
    std::unordered_map<double, double> rates = { {0.0, 0.05}, {0.5, 0.07}, {1.0, 0.1} };
    Rates rate(rates);

    EXPECT_THROW(rate.at(0.25), std::out_of_range); 
}

TEST(RateTests, ConstructorWithDt_InvalidDt) {
    EXPECT_THROW(Rates(0.05, 0.1, 0), std::invalid_argument);
    EXPECT_THROW(Rates(0.05, 0.1, -0.1), std::invalid_argument);
    EXPECT_THROW(Rates(0.05, 0.1, 1.0), std::invalid_argument);
}

TEST(RateTests, AtMethod_Bounds) {
    Rates rate(0.05, 0.1, 0.1);

    EXPECT_NEAR(rate.at(0.0), 0.05, 1e-6);  
    EXPECT_NEAR(rate.at(1.0), 0.1, 1e-6);  
}
