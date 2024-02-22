cmake_minimum_required(VERSION 3.5)
add_library(mc SHARED mc.cpp) 
target_include_directories(mc PUBLIC ${PROJECT_SOURCE_DIR}/include) 