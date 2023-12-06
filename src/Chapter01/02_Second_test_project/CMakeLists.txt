cmake_minimum_required(VERSION 3.22.0)

project(02_Second_test_project)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02_Second_test_project "Chapter1")

if(TARGET SharedUtils)
target_link_libraries(02_Second_test_project SharedUtils)
endif()
