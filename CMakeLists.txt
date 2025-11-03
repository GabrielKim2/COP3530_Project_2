cmake_minimum_required(VERSION 3.10)
project(COP3530_Project_2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This project is small and uses header-only KDTree/Point implementations.
# Only the translation unit that contains `main()` is required here.
add_executable(
    COP3530_Project_2
    src/main.cpp
    src/QuadTree.cpp
)