cmake_minimum_required(VERSION 3.22.0)

project(02_working_spin_locks_test_and_set)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02_working_spin_locks_test_and_set "Chapter03")

if(TARGET SharedUtils)
target_link_libraries(02_working_spin_locks_test_and_set SharedUtils)
endif()
