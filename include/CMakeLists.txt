# Indicate the source files
set(LIB_SRCS genetic_algorithm.h  genetic_algorithm.c)

# Set the library type as static
SET (LIB_TYPE STATIC)

add_library (algo_lib ${LIB_TYPE} ${LIB_SRCS})