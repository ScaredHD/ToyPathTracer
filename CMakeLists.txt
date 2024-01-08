cmake_minimum_required(VERSION 3.20)

project(rt_v3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O2")

add_subdirectory(test)

add_subdirectory(src)




