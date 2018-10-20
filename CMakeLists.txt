cmake_minimum_required(VERSION 3.6)
project(aru)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_PREFIX_PATH "/opt/qt/;${CMAKE_PREFIX_PATH}")

set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_USE_MATH_DEFINES)

add_subdirectory(haru)
