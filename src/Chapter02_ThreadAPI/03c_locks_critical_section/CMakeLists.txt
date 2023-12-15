cmake_minimum_required(VERSION 3.22.0)

project(03c_locks_critical_section)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03c_locks_critical_section "Chapter02")

if(TARGET SharedUtils)
target_link_libraries(03c_locks_critical_section SharedUtils)
endif()


if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
target_compile_options(03c_locks_critical_section PRIVATE -Wno-deprecated-volatile)
endif()