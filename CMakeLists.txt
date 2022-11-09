# Set the minimum CMake version
cmake_minimum_required(VERSION 2.8.12)

# Set the project name
project (GEN_ALG)

# Set build type to Debug
set(CMAKE_BUILD_TYPE Debug)

# Add library subdirectory
set(GEN_ALG_INCLUDE_ALGO_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/algo_lib)
add_subdirectory(include)
include_directories(${GEN_ALG_INCLUDE_ALGO_LIB_DIR})

# Add the main file, and name of its executable
add_executable(genetic_algorithm main.c)

# Link the library to the main file
target_link_libraries(genetic_algorithm algo_lib)