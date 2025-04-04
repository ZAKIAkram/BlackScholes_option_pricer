
add_library(BS_pricer STATIC
    Option.cpp
    CallPayoff.cpp
    Payoff.cpp
    PutPayoff.cpp
    Rates.cpp
    CrankNicolsonSolver.cpp
    EuropeanOption.cpp 
    AmericanOption.cpp
    Greeks.cpp
    BlackScholesExplicit.cpp
    vbaInterface.cpp
)

add_library(BS_DLL SHARED
    vbaInterface.cpp
    Option.cpp
    CallPayoff.cpp
    Payoff.cpp
    PutPayoff.cpp
    Rates.cpp
    CrankNicolsonSolver.cpp
    EuropeanOption.cpp 
    AmericanOption.cpp
    Greeks.cpp
)

target_include_directories(BS_DLL PUBLIC ${CMAKE_SOURCE_DIR}/include)

set_target_properties(BS_DLL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/out/build/x64-Debug/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/out/build/x64-Release/bin
    OUTPUT_NAME "CN_pricer_lib"
)

add_executable(main main.cpp 
    EuropeanOption.cpp 
    AmericanOption.cpp 
    Greeks.cpp 
    BlackScholesExplicit.cpp 
    vbaInterface.cpp)

target_link_libraries(main BS_pricer)
