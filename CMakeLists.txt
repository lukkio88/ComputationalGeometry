cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ComputationalGeometry)

set(CMAKE_CXX_STANDARD 14)

set(SUBMODULE_DIR ${PROJECT_SOURCE_DIR}/Geometry)
set(INCLUDE_SUBMODULE ${SUBMODULE_DIR}/include)
add_subdirectory(${SUBMODULE_DIR})
#add_library(geometry STATIC IMPORTED)
#set_target_properties(geometry PROPERTIES IMPORTED_LOCATION ${SUBMODULE_DIR}/Debug/geometry.lib)

set(SOURCE_FOLDER ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_FOLDER ${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES
	#${SOURCE_FOLDER}/dcel.cpp
	#${SOURCE_FOLDER}/dcelIO.cpp
	${SOURCE_FOLDER}/line_seg_intersection.cpp
	${PROJECT_SOURCE_DIR}/main.cpp
	#${INCLUDE_FOLDER}/dcel.h
	#${INCLUDE_FOLDER}/dcelIO.h
	${INCLUDE_FOLDER}/line_seg_intersection.h)

add_executable(computational-geometry ${SOURCE_FILES})
target_include_directories(computational-geometry PUBLIC ${INCLUDE_FOLDER} ${INCLUDE_SUBMODULE})
target_link_libraries(computational-geometry geometry)