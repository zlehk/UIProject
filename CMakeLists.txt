cmake_minimum_required(VERSION 3.17)
project(Solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_subdirectory(test)
add_subdirectory(src)