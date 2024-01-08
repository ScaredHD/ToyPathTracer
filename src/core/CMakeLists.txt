add_library(core 
    renderer.cpp 
    pathtracer.cpp
)

target_link_libraries(core camera geometry sampler)

target_include_directories(core PUBLIC .)