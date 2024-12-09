add_library(BS_pricer
    
    "Option.cpp"
    CallPayoff.cpp
    Payoff.cpp
    PutPayoff.cpp
    "Rates.cpp"
    CrankNicolsonSolver.cpp
 "EuropeanOption.cpp" "AmericanOption.cpp" "Greeks.cpp")

add_executable(main main.cpp "EuropeanOption.cpp" "AmericanOption.cpp" "Greeks.cpp")

target_link_libraries(main BS_pricer)
