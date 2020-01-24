cmake_minimum_required(VERSION 3.8)
project(bigrams)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.cpp grams_computing.cpp grams_computing.h)
add_executable(bigrams ${SOURCE_FILES})