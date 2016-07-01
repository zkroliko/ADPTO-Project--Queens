cmake_minimum_required(VERSION 3.5)
project(adptoRaw)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -std=c++0x -std=gnu++0x")

set(SOURCE_FILES adpto.cpp)
add_executable(adptoRaw ${SOURCE_FILES})