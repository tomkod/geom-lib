set(GEOM_SOURCES
    src/glslMathTest.cpp
    src/manifold.cpp
    src/manifoldTest.cpp
    include/glslMath.h
    include/manifold.h)

add_library(geom ${GEOM_SOURCES})
target_include_directories(geom PUBLIC "include")
