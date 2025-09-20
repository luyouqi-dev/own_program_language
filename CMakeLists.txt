cmake_minimum_required(VERSION 3.22)
project(PLinC)

set(CMAKE_CXX_STANDARD 17)

add_executable(PLinC main.cpp
        include/Heap.hpp
        include/Program.hpp)
