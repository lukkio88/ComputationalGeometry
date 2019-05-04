cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ComputationalGeometry)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(./Point)
add_subdirectory(./DCEL)
set(INCLUDE_FOLDERS ./ ./Point ./DCEL)
set(SRC_FILES main.cpp)

add_executable(computational-geometry ${SRC_FILES})
target_include_directories(computational-geometry PUBLIC ${INCLUDE_FOLDERS})
target_link_libraries(computational-geometry dcel 2d-geometry)