add_library(utils 
    color.cpp
    image.cpp
    timer.cpp
    transform.cpp
    utils.cpp
)

target_include_directories(utils PUBLIC .)

target_include_directories(utils PRIVATE ${PROJECT_SOURCE_DIR}/third-party)