cmake_minimum_required(VERSION 3.0)

project(blur_app)
find_package(OpenCV REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(blur_app main.cpp region_blur.h region_blur.cpp)
target_link_libraries(blur_app ${OpenCV_LIBS})
