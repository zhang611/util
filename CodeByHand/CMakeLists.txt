cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 17)

include_directories("E:/Library/stb")
add_executable(main convolution/main.cpp)