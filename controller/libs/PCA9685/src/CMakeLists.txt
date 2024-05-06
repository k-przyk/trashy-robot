cmake_minimum_required(VERSION 3.10)
project(PCA9685 CXX)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fPIC -g -Wall -Werror")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/../../I2C/src)  # Include I2C library headers

# Define the shared library
add_library(PCA9685 SHARED PCA9685.cpp PCA9685.h)

# Link the library with the I2C library
target_link_libraries(PCA9685 PUBLIC ${PROJECT_SOURCE_DIR}/../../I2C/src/libI2C.so)
target_include_directories(
    PCA9685 
    PUBLIC ${PROJECT_SOURCE_DIR}/../../I2C/src/
)

# Set the library version
set_target_properties(PCA9685 PROPERTIES
    VERSION 0.1
    SOVERSION 0
)

# Installation rules
install(TARGETS PCA9685
    LIBRARY DESTINATION lib
)
