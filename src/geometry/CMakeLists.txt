
add_library(geometry 
    scene.cpp
    shape.cpp
    objects.cpp
    intersection.cpp
)

target_link_libraries(geometry utils material light sampler)

target_include_directories(geometry PUBLIC .)