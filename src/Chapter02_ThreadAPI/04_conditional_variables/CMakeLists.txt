cmake_minimum_required(VERSION 3.22.0)

project(04_conditional_variables)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04_conditional_variables "Chapter02")

if(TARGET SharedUtils)
target_link_libraries(04_conditional_variables SharedUtils)
endif()
