cmake_minimum_required(VERSION 3.22.0)

project(01_hello_world_example)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01_hello_world_example "Chapter01")

if(TARGET SharedUtils)
target_link_libraries(01_hello_world_example SharedUtils)
endif()
