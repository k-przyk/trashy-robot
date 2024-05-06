cmake_minimum_required(VERSION 3.10)
project(I2C CXX)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -g -Wall -Werror")

# Define the shared library
add_library(I2C SHARED I2C.cpp I2C.h)

# Set the library version
set_target_properties(I2C PROPERTIES
    VERSION 0.1
    SOVERSION 0
)

# Installation rules
install(TARGETS I2C
    LIBRARY DESTINATION lib
)