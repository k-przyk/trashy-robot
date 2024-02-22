cmake_minimum_required(VERSION 3.5)


project(mc_test) 


add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_executable(mc_test src/main.cpp)
target_link_libraries(mc_test PRIVATE mc)