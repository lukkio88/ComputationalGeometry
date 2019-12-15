cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(GeometryPrimitiveTest)

set(CMAKE_CXX_STANDARD 11)

include_directories(../Point)
set(SRC_FILES geometry-primitive-test.cpp)

add_executable(geometry-primitive-test ${SRC_FILES})
target_include_directories(geometry-primitive-test PUBLIC ../Point)
target_link_libraries(geometry-primitive-test 2d-geometry)