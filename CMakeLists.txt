cmake_minimum_required(VERSION 3.20)
project(PPMImage)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_subdirectory(vendor/spdlog)

add_executable(PPMImage
        main.cpp
        PPMImage.hpp PPMImage.cpp)
target_link_libraries(PPMImage SDL2::SDL2 spdlog::spdlog)
