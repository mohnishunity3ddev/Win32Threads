cmake_minimum_required(VERSION 3.22.0)

project(04_race_condition_problem)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04_race_condition_problem "Chapter01")

if(TARGET SharedUtils)
target_link_libraries(04_race_condition_problem SharedUtils)
endif()


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
target_compile_options(04_race_condition_problem PRIVATE -Wno-deprecated-volatile)
endif()