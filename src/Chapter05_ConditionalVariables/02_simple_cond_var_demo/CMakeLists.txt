cmake_minimum_required(VERSION 3.22.0)

project(02_simple_cond_var_demo)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02_simple_cond_var_demo "Chapter05")

if(TARGET SharedUtils)
target_link_libraries(02_simple_cond_var_demo SharedUtils)
endif()
