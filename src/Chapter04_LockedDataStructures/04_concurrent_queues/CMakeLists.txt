cmake_minimum_required(VERSION 3.22.0)

project(04_concurrent_queues)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04_concurrent_queues "Chapter04")

if(TARGET SharedUtils)
target_link_libraries(04_concurrent_queues SharedUtils)
endif()
