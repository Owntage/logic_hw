cmake_minimum_required(VERSION 2.8)

project(math_logic_hw)

include_directories(${PROJECT_SOURCE_DIR}/axe_parser)

add_definitions(-std=c++11 -g)

add_executable(hw1 ${PROJECT_SOURCE_DIR}/source/hw1.cpp)

add_executable(axe_test ${PROJECT_SOURCE_DIR}/source/axe_test.cpp)