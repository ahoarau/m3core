cmake_minimum_required(VERSION 2.8)
project(m3core)
# Tmp solution for the FindXXX.cmake 
set(CMAKE_MODULE_PATH 
    "${CMAKE_CURRENT_SOURCE_DIR}/../meka-cmake-modules/Modules/"
    ${CMAKE_MODULE_PATH})

set(M3RT_SRC ${PROJECT_SOURCE_DIR}/src)

# Include dirs to look for
include_directories(
    ${M3RT_SRC}
    ${CMAKE_BINARY_DIR}/src # Include this directory for the generated protobufs headers
)


add_subdirectory(src)
add_subdirectory(python)
