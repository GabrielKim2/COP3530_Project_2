cmake_minimum_required(VERSION 3.10)
project(COP3530_Project_2)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(include)

add_executable(COP3530_Project_2
        src/main.cpp
        src/KDTree.cpp
        src/QuadTree.cpp
        src/Point.h
)