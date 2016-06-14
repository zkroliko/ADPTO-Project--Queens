cmake_minimum_required(VERSION 3.5)
project(adpto)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp include/Queen.h src/Queen.cpp include/Queen.h src/Board.cpp src/Loader.cpp include/Loader.h include/Board.h src/Solver.cpp include/Solver.h include/Debugging.h)
add_executable(adpto ${SOURCE_FILES})