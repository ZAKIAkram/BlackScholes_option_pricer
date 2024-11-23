add_library(BS_pricer BS_pricer.cpp "main.cpp" "VanillaOption.cpp" "CallPayoff.cpp" "Payoff.cpp" "PutPayoff.cpp" "Rate.cpp" "CrankNicolsonSolver.cpp")
add_executable(main main.cpp BS_pricer.cpp "VanillaOption.cpp" "CallPayoff.cpp" "Payoff.cpp" "PutPayoff.cpp" "Rate.cpp" "CrankNicolsonSolver.cpp")
