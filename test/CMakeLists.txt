add_executable(v3_test
    fresnel_test.cpp
    intersection_test.cpp
    sampler_test.cpp
    shape_test.cpp
    transformation_test.cpp
    utils_test.cpp
)

enable_testing()
add_subdirectory(googletest-main)

target_link_libraries(v3_test 
    gtest_main
    utils
    geometry
    sampler
)

include(GoogleTest)
gtest_discover_tests(v3_test)