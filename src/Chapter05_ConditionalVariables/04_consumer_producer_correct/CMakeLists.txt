cmake_minimum_required(VERSION 3.22.0)

project(04_consumer_producer_correct)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(04_consumer_producer_correct "Chapter05")

if(TARGET SharedUtils)
target_link_libraries(04_consumer_producer_correct SharedUtils)
endif()
