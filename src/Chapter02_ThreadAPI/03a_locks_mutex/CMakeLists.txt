cmake_minimum_required(VERSION 3.22.0)

project(03a_locks_mutex)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03a_locks_mutex "Chapter02")

if(TARGET SharedUtils)
target_link_libraries(03a_locks_mutex SharedUtils)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
target_compile_options(03a_locks_mutex PRIVATE -Wno-deprecated-volatile)
endif()
