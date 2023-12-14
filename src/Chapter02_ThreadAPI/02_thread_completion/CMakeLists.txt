cmake_minimum_required(VERSION 3.22.0)

project(02_thread_completion)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02_thread_completion "Chapter02")

if(TARGET SharedUtils)
target_link_libraries(02_thread_completion SharedUtils)
endif()
