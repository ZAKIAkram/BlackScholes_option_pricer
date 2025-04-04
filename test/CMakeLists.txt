
enable_testing()

add_executable(
  non_regression_tests
  CallPayoff_test.cpp
  PutPayoff_test.cpp
  Rate_test.cpp
  vbaInterface_test.cpp
  CrankNicolsonSolver_test.cpp "BlackScholesExplicitSolver_test.cpp" "Greeks_test.cpp" "EuropeanOption_test.cpp" "AmericanOption_test.cpp")

target_link_libraries(
  non_regression_tests
  BS_pricer         
  GTest::gtest_main  
)

include(GoogleTest)
gtest_discover_tests(non_regression_tests)
