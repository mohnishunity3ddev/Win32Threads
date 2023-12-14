cmake_minimum_required(VERSION 3.22.0)

project(03b_locks_interlocked_ops)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03b_locks_interlocked_ops "Chapter02")

if(TARGET SharedUtils)
target_link_libraries(03b_locks_interlocked_ops SharedUtils)
endif()
