project(ImportExport)
###################################### PROJECT CONFIGURATION ############################################
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_COMPILE_FEATURES "cxx_decltype")

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	set(BUILD_POSTFIX _d)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)
################################### SOURCES AND SETTING #########################################################
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

#set(BUILD_WITH_SCRIPT_SUPPORT TRUE)

add_subdirectory(src)
add_subdirectory(examples)
