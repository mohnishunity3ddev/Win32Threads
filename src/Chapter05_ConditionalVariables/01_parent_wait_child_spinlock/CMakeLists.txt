cmake_minimum_required(VERSION 3.22.0)

project(01_parent_wait_child_spinlock)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01_parent_wait_child_spinlock "Chapter05")

if(TARGET SharedUtils)
target_link_libraries(01_parent_wait_child_spinlock SharedUtils)
endif()
