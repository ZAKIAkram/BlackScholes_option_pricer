
enable_testing()

add_executable(
  all_tests
  CallPayoff_test.cpp
  PutPayoff_test.cpp
)

target_link_libraries(
  all_tests
  BS_pricer         
  GTest::gtest_main  
)

include(GoogleTest)
gtest_discover_tests(all_tests)
