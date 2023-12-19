cmake_minimum_required(VERSION 3.22.0)

project(03_reader_writer_locks)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03_reader_writer_locks "Chapter06")

if(TARGET SharedUtils)
target_link_libraries(03_reader_writer_locks SharedUtils)
endif()
