
add_library(material 
    bxdf.cpp
    material.cpp
    fresnel.cpp
)

target_link_libraries(material utils sampler)

target_include_directories(material PUBLIC .)