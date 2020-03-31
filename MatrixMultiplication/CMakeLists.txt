cmake_minimum_required(VERSION 3.3.2)

project(my_hpx_project CXX)

find_package(HPX REQUIRED)
add_hpx_executable(MatrixMul
    SOURCES MatrixMultiplication.cpp)