cmake_minimum_required(VERSION 3.22.0)

project(03_compare_and_swap)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03_compare_and_swap "Chapter03")

if(TARGET SharedUtils)
target_link_libraries(03_compare_and_swap SharedUtils)
endif()
