cmake_minimum_required(VERSION 3.22.0)

project(01_semaphore_for_ordering)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01_semaphore_for_ordering "Chapter06")

if(TARGET SharedUtils)
target_link_libraries(01_semaphore_for_ordering SharedUtils)
endif()
