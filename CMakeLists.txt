cmake_minimum_required(VERSION 3.10)
project(raylib_minecraft C)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib CONFIG REQUIRED)

set(SOURCES
  src/scenes/scene_3d_game.c
  src/sprites/player.c
  src/core/voxel.c
  src/utils/resource_tracker.c
)

add_executable(game main.c ${SOURCES})

target_include_directories(game PRIVATE includes)

target_link_libraries(game PRIVATE raylib)
