cmake_minimum_required(VERSION 3.22.0)

project(01_thread_creation)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01_thread_creation "Chapter02")

if(TARGET SharedUtils)
target_link_libraries(01_thread_creation SharedUtils)
endif()
