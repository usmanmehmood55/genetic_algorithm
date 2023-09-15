cmake_minimum_required(VERSION 3.19)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES ASM C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set flags for Debug build
if(${CMAKE_BUILD_TYPE} MATCHES Debug)
  set(CMAKE_C_FLAGS "-O0 -g3")
endif()

# Set flags for Release build
if(${CMAKE_BUILD_TYPE} MATCHES Release)
  set(CMAKE_C_FLAGS "-O3")
endif()

# Set flags for Test build
if(${CMAKE_BUILD_TYPE} MATCHES Test)
  set(CMAKE_C_FLAGS "-O0 -g3 --coverage")
endif()

# Add source file based on build type
if(${CMAKE_BUILD_TYPE} MATCHES Test)
  add_executable(${PROJECT_NAME} test/test_genetic_algorithm.c)
  add_subdirectory(c_asserts)
else()
  add_executable(${PROJECT_NAME} main.c)
endif()

# Add library subdirectory
add_subdirectory(genetic_algorithm_utils)
add_subdirectory(app_init)

# Conditionally link libraries
if(${CMAKE_BUILD_TYPE} MATCHES Test)
  target_link_libraries(${PROJECT_NAME} PRIVATE gcov c_asserts_obj)
endif()