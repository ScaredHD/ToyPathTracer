
add_library(sampler 
    sampler.cpp
)

target_link_libraries(sampler utils)

target_include_directories(sampler PUBLIC .)