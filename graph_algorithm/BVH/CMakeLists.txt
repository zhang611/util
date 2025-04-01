cmake_minimum_required(VERSION 3.10)
project(BVH)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BVH main.cpp Vector.cpp Scene.cpp BVH.cpp Renderer.cpp)
