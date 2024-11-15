enable_testing() 

add_executable(
  BS_pricer_test
  BS_pricer_test.cpp
)

target_link_libraries(
  BS_pricer_test
  GTest::gtest_main 
)

include(GoogleTest)
gtest_discover_tests(BS_pricer_test) 
