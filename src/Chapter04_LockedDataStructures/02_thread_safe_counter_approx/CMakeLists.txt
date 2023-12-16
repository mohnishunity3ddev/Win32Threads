cmake_minimum_required(VERSION 3.22.0)

project(02_thread_safe_counter_approx)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02_thread_safe_counter_approx "Chapter04")

if(TARGET SharedUtils)
target_link_libraries(02_thread_safe_counter_approx SharedUtils)
endif()
