cmake_minimum_required(VERSION 3.10)
project(raylib_minecraft C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Raylib CONFIG REQUIRED)

add_executable(game main.c)

target_link_libraries(game PRIVATE raylib)
