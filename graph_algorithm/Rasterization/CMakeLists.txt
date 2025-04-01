cmake_minimum_required(VERSION 3.0)
project(Rasterization)
set(CMAKE_CXX_STANDARD 14)


set(OpenCV_DIR "E:/Library/opencv/opencv/build/x64/vc16")
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "version:${OpenCV_VERSION}")
message(STATUS "libraries:${OpenCV_LIBS}")
message(STATUS "include path:${OpenCV_INCLUDE_DIRS}")


add_executable(${PROJECT_NAME} main.cpp rasterizer.cpp Triangle.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE "E:/Library/eigen-3.4.0")
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

