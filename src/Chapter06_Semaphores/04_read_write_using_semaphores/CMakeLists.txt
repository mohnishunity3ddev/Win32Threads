cmake_minimum_required(VERSION 3.22.0)

project(04_read_write_using_semaphores)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04_read_write_using_semaphores "Chapter06")

if(TARGET SharedUtils)
target_link_libraries(04_read_write_using_semaphores SharedUtils)
endif()
