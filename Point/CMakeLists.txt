cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(2DGeometry)

set(CMAKE_CXX_STANDARD 11)

include_directories(./)
set(HEADER_FILES util.h point2D.h segment.h line_seg_intersection.h)
set(SRC_FILES util.cpp point2D.cpp segment.cpp line_seg_intersection.cpp)

#if(WIN32)
#	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
#endif()

add_library(2d-geometry STATIC ${SRC_FILES} ${HEADER_FILES})
#set_target_properties(2d-geometry PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})