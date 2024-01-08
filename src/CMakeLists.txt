
add_executable(v3 main.cpp)

add_subdirectory(camera)
add_subdirectory(core)
add_subdirectory(geometry)
add_subdirectory(light)
add_subdirectory(material)
add_subdirectory(sampler)
add_subdirectory(utils)

target_link_libraries(v3 camera core utils)

target_include_directories(v3 PUBLIC .)
