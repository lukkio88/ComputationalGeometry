cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ComputationalGeometry)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(./Point)
set(INCLUDE_FOLDERS ./Point)
set(SRC_FILES main.cpp)

add_executable(computational-geometry ${SRC_FILES})
target_link_libraries(computational-geometry 2d-geometry)