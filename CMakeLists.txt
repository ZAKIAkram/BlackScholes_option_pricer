# CMakeList.txt : CMake project for BS_pricer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
#add_executable (BS_pricer "src/BS_pricer.cpp" "include/BS_pricer.h")

#if (CMAKE_VERSION VERSION_GREATER 3.12)
 # set_property(TARGET BS_pricer PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Add tests and install targets if needed.

cmake_minimum_required(VERSION 3.16.3)

project(BS_pricer CXX)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS)
if(MSVC)
    add_compile_options(/EHsc)
endif()


# Include directories for all subprojects
include_directories(include)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows
FetchContent_MakeAvailable(googletest)

get_target_property(GTest_Debug_Info GTest::gtest IMPORTED_LOCATION)
message(STATUS "GTest::gtest location: ${GTest_Debug_Info}")
add_definitions(-DGTEST_HAS_ABSL=0)
add_definitions(-DGTEST_OS_ESP8266=1)



# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)

