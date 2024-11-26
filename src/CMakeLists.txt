add_library(BS_pricer
    
    VanillaOption.cpp
    CallPayoff.cpp
    Payoff.cpp
    PutPayoff.cpp
    Rates.cpp
    CrankNicolsonSolver.cpp
    Greeks.cpp
    BS_pricer.cpp
    Greeks_bs.cpp
)

add_executable(main main.cpp)

target_link_libraries(main BS_pricer)
