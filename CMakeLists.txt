cmake_minimum_required(VERSION 3.20)
project(PPMApp)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_subdirectory(ppm)
add_subdirectory(vendor/spdlog)

add_executable(PPMApp main.cpp)
target_link_libraries(PPMApp PPMImage SDL2::SDL2 spdlog::spdlog)
