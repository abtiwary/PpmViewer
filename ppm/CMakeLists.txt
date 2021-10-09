cmake_minimum_required(VERSION 3.20)
project(PPMImage)

set(CMAKE_CXX_STANDARD 17)

add_library(PPMImage src/PPMImage.cpp include/PPMImage.hpp)
include_directories(include)
