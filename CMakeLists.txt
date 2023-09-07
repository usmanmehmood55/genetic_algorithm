cmake_minimum_required(VERSION 3.19)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES ASM C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set build flags
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "-O0 -g3 --coverage")

add_executable(${PROJECT_NAME} main.c)

# Add library subdirectory
add_subdirectory(genetic_algorithm)

target_link_libraries(${PROJECT_NAME} gcov)