cmake_minimum_required(VERSION 3.10)
project(raylib_minecraft C)

# set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib CONFIG REQUIRED)

set(SOURCES
  src/scene_voxel_game.c
  src/player.c
  src/voxel.c
  src/resource_tracker.c
  src/event_handler.c
  src/input_handler.c
  src/debug_logger.c
)

add_executable(game src/main.c ${SOURCES})

target_include_directories(game PRIVATE includes)

target_link_libraries(game PRIVATE raylib m)

