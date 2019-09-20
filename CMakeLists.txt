cmake_minimum_required (VERSION 2.6)
project (geomlib)
set (CMAKE_CXX_STANDARD 14)

set(GEOM_SOURCES
    src/glslMathTest.cpp
    src/manifold.cpp
    src/manifoldTest.cpp
    include/glslMath.h
    include/manifold.h)

add_library(geom ${GEOM_SOURCES})
target_include_directories(geom PUBLIC "include")

add_executable(geomTest src/mainTests.cpp)

target_link_libraries(geomTest geom)

