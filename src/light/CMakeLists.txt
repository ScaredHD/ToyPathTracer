cmake_minimum_required(VERSION 3.20)

add_library(light
    light.cpp
)

target_link_libraries(light utils geometry)

target_include_directories(light PUBLIC .)