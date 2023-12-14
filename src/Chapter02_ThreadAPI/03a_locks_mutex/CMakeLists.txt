cmake_minimum_required(VERSION 3.22.0)

project(03a_locks_mutex)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03a_locks_mutex "Chapter02")

if(TARGET SharedUtils)
target_link_libraries(03a_locks_mutex SharedUtils)
endif()
