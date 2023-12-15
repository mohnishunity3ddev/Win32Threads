cmake_minimum_required(VERSION 3.22.0)

project(05_ticket_turn_lock)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(05_ticket_turn_lock "Chapter03")

if(TARGET SharedUtils)
target_link_libraries(05_ticket_turn_lock SharedUtils)
endif()
