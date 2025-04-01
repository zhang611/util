cmake_minimum_required(VERSION 3.10)
project(whitted)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(whitted main.cpp Scene.cpp Renderer.cpp)