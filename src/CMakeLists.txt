add_library(BS_pricer
    BS_pricer.cpp
    VanillaOption.cpp
    CallPayoff.cpp
    Payoff.cpp
    PutPayoff.cpp
    Rate.cpp
    CrankNicolsonSolver.cpp
    Greeks.cpp
    BS_pricer_update.cpp)

add_executable(main main.cpp)

target_link_libraries(main BS_pricer)
