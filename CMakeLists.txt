cmake_minimum_required(VERSION 3.2.2)
set (CMAKE_CXX_STANDARD 11)
project(scales)

file(GLOB SOURCES "src/*.cpp")

add_executable(scales ${SOURCES})
