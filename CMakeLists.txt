cmake_minimum_required(VERSION 3.25)
project(graph)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(graph
        main.cpp
        myGraph.cpp
        myGraph.h)
