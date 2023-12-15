cmake_minimum_required(VERSION 3.22.0)

project(01_using_simple_flag)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01_using_simple_flag "Chapter03")

if(TARGET SharedUtils)
target_link_libraries(01_using_simple_flag SharedUtils)
endif()
