cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(2DGeometry)

set(CMAKE_CXX_STANDARD 11)

include_directories(./)
set(HEADER_FILES util.h point2D.h segment.h)
set(SRC_FILES main.cpp util.cpp point2D.cpp segment.cpp)

add_executable(2DGeometry ${SRC_FILES} ${HEADER_FILES})