cmake_minimum_required(VERSION 4.0)
project(chip8 C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(chip8 main.c
        chip8.c
        chip8.h
        render.c
        render.h)

target_link_libraries(chip8 ${SDL2_LIBRARIES})