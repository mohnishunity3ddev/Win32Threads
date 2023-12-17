cmake_minimum_required(VERSION 3.22.0)

project(03_consumer_producer_wrong)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03_consumer_producer_wrong "Chapter05")

if(TARGET SharedUtils)
target_link_libraries(03_consumer_producer_wrong SharedUtils)
endif()
