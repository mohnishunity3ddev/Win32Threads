cmake_minimum_required(VERSION 3.22.0)

project(02_Joinable_Detached_Threads)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02_Joinable_Detached_Threads "Chapter01")

if(TARGET SharedUtils)
target_link_libraries(02_Joinable_Detached_Threads SharedUtils)
endif()
