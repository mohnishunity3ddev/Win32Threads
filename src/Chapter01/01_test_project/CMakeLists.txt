cmake_minimum_required(VERSION 3.22.0)

project(01_test_project)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(01_test_project "Chapter01")

if(TARGET SharedUtils)
target_link_libraries(01_test_project SharedUtils)
endif()
