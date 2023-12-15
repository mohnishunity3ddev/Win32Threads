cmake_minimum_required(VERSION 3.22.0)

project(04_load_linked_store_conditional)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04_load_linked_store_conditional "Chapter03")

if(TARGET SharedUtils)
target_link_libraries(04_load_linked_store_conditional SharedUtils)
endif()
