cmake_minimum_required(VERSION 3.28)
project(test_rockstone)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

add_subdirectory(src)
add_subdirectory(test)
