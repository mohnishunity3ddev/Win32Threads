cmake_minimum_required(VERSION 3.22.0)

project(03.Test_Another_Project)

include(../../../cmake_macros/prac.cmake)

SETUP_APP(03.Test_Another_Project "Chapter1")

if(TARGET SharedUtils)
target_link_libraries(03.Test_Another_Project SharedUtils)
endif()
