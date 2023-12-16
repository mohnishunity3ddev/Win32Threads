cmake_minimum_required(VERSION 3.22.0)

project(03_concurrent_linked_lists)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03_concurrent_linked_lists "Chapter04")

if(TARGET SharedUtils)
target_link_libraries(03_concurrent_linked_lists SharedUtils)
endif()
