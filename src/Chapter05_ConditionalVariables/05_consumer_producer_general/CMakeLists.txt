cmake_minimum_required(VERSION 3.22.0)

project(05_consumer_producer_general)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(05_consumer_producer_general "Chapter05")

if(TARGET SharedUtils)
target_link_libraries(05_consumer_producer_general PRIVATE SharedUtils)
endif()