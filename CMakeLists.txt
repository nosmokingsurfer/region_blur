cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

project(blur_app)
find_package(OpenCV 4.2 REQUIRED)
find_package(Boost 1.68 REQUIRED)

include_directories( ${Boost_INCLUDE_DIR} )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(blur_app main.cpp region_blur.h region_blur.cpp)
target_link_libraries(blur_app ${OpenCV_LIBS} ${Boost_PROGRAM_OPTIONS_LIBRARY})
