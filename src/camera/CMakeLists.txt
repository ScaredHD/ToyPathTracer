
add_library(camera 
    camera.cpp
)

target_link_libraries(camera utils)

target_include_directories(camera PUBLIC .)