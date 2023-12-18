cmake_minimum_required(VERSION 3.22.0)

project(02_producer_consumer)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(02_producer_consumer "Chapter06")

if(TARGET SharedUtils)
target_link_libraries(02_producer_consumer SharedUtils)
endif()
