add_library(BS_pricer BS_pricer.cpp "main.cpp")
add_executable(main main.cpp BS_pricer.cpp)
