cmake_minimum_required(VERSION 2.8)

project(tripping-archer)

file(GLOB src src/*.cpp)
include_directories(include/)
add_executable(tripping-archer main.cpp ${src})
