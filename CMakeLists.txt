cmake_minimum_required(VERSION 3.0)

project(blur_app)
find_package(OpenCV REQUIRED)

add_executable(blur_app main.cpp)
target_link_libraries(blur_app ${OpenCV_LIBS})