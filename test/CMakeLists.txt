
enable_testing()

add_executable(
  non_regression_tests
  CallPayoff_test.cpp
  PutPayoff_test.cpp
  Rate_test.cpp
  BlackScholesExplicit_test.cpp
  AmericanOption_test.cpp
  EuropeanOption_test.cpp)

target_link_libraries(
  non_regression_tests
  BS_pricer         
  GTest::gtest_main  
)

include(GoogleTest)
gtest_discover_tests(non_regression_tests)
