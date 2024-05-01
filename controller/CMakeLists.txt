cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(trash-controller CXX)

# place binaries and libraries according to GNU standards
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

add_custom_target(cleanup
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMENT "Cleaning build artifacts"
)

find_package(Threads)
find_package(cppzmq)

# Test script for point generation
add_executable(
    point_generator
    src/point_generator.cpp
)
target_link_libraries(
    point_generator
    PRIVATE cppzmq ${CMAKE_THREAD_LIBS_INIT}
)
target_include_directories(
    point_generator 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
) 

# Test script for point reception
add_executable(
    cmd_reciever
    src/cmd_reciever.cpp
)
target_link_libraries(
    cmd_reciever
    PRIVATE cppzmq ${CMAKE_THREAD_LIBS_INIT}
)
target_include_directories(
    cmd_reciever 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
) 

# Script for PC controller functionality
add_executable(
    auto_driver
    src/auto_driver.cpp
)
target_link_libraries(
    auto_driver
    PRIVATE cppzmq ${CMAKE_THREAD_LIBS_INIT}
)
target_include_directories(
    auto_driver 
    PUBLIC ${PROJECT_SOURCE_DIR}/include
) 