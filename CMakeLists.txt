cmake_minimum_required(VERSION 3.16)

project(raspi_prank_sensor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(core
    src/sensor/hcsr04.cpp
    src/audio/audioplayer.cpp
)

target_include_directories(core
    PUBLIC
        include
)

target_compile_options(core
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(app
    src/main.cpp
)

target_link_libraries(app
    PRIVATE
        core
        gpiod
)
