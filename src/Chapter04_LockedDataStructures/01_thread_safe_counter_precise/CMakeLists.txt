cmake_minimum_required(VERSION 3.22.0)

project(01_thread_safe_counter_precise)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01_thread_safe_counter_precise "Chapter04")

if(TARGET SharedUtils)
target_link_libraries(01_thread_safe_counter_precise SharedUtils)
endif()
