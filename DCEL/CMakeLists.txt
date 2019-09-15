cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(2DGeometry)

set(CMAKE_CXX_STANDARD 11)

include_directories(./ ../Point)
set(HEADER_FILES dcel.h dcelIO.h)
set(SRC_FILES dcel.cpp dcelIO.cpp event_point.cpp edge.cpp)

add_library(dcel STATIC ${SRC_FILES} ${HEADER_FILES})
target_link_libraries(dcel 2d-geometry)
#set_target_properties(2d-geometry PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})