add_library(BS_pricer
    BS_pricer.cpp
    "Option.cpp"
    CallPayoff.cpp
    Payoff.cpp
    PutPayoff.cpp
    "Rates.cpp"
    CrankNicolsonSolver.cpp
)

add_executable(main main.cpp)

target_link_libraries(main BS_pricer)
