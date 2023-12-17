cmake_minimum_required(VERSION 3.22.0)

project(Win32Threads CXX C)

set(EXTERN_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external)

set(CMAKE_CXX_STANDARD 20) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CLANG TRUE)
else()
    set(CLANG FALSE)
endif()

# Definitions
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    add_definitions(-DWIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

if(CLANG)
# Remove the -Wdeprecated-volatile flag globally
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wno-deprecated-volatile>")
endif()

option(USE_DEBUG "Enter debug mode" OFF)
if (USE_DEBUG)
  add_definitions(-DDEBUG)
endif()

# set(ANALYZE "ON" CACHE BOOL "Analyze Executables")
# if(ANALYZE)
#   if(MSVC)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
#   elseif(CLANG)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wthread-safety -g")
#   endif()
# endif()


###### Find OpenGL
# find_package(OpenGL REQUIRED)

###### GLFW Stuff
# set(GLFW_BUILD_EXAMPLES     OFF CACHE BOOL "")
# set(GLFW_BUILD_TESTS        OFF CACHE BOOL "")
# set(GLFW_BUILD_DOCS         OFF CACHE BOOL "")
# set(GLFW_INSTALL            OFF CACHE BOOL "")
# add_subdirectory(external/glfw)

###### Assimp Stuff
# set(ASSIMP_NO_EXPORT            ON CACHE BOOL "")
# set(ASSIMP_BUILD_DRACO          OFF CACHE BOOL "")
# set(ASSIMP_BUILD_ASSIMP_TOOLS   OFF CACHE BOOL "")
# set(ASSIMP_BUILD_TESTS          OFF CACHE BOOL "")
# set(ASSIMP_INSTALL_PDB          OFF CACHE BOOL "")
# set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "")
# set(ASSIMP_BUILD_OBJ_IMPORTER   ON CACHE BOOL "")
# set(ASSIMP_BUILD_GLTF_IMPORTER  ON CACHE BOOL "")
# if(WIN32)
#     set(ASSIMP_BUILD_ZLIB       ON CACHE BOOL "")
# endif()
# add_subdirectory(external/assimp)

# if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-attributes -Wno-defaulted-function-deleted -Wno-braced-scalar-init -Wno-writable-strings -Wno-format -Wno-deprecated-declarations -Wno-macro-redefined")
# endif ()

###### GLAD Stuff
# set(GLAD_SOURCES external/glad/src/glad.c)
# add_library(glad ${GLAD_SOURCES})

###### Include Directories ...
# include_directories(external/glfw/include)
# include_directories(external/glm)
# include_directories(external/glad/include)
# include_directories(external/stb)
# include_directories(external/assimp/include)
# include_directories($<TARGET_FILE_DIR:assimp>/../include) # there is one config.h file which gets build in assimp's build directory by cmake.
# include_directories(external/imgui)
include_directories(common)

# if(WIN32)
#     set_property(TARGET zlibstatic    PROPERTY FOLDER "ThirdPartyLibraries")
# endif()
# set_property(TARGET glfw            PROPERTY FOLDER "ThirdPartyLibraries")
# set_property(TARGET glad            PROPERTY FOLDER "ThirdPartyLibraries")
# set_property(TARGET assimp          PROPERTY FOLDER "ThirdPartyLibraries")


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include Common SharedUtils
add_subdirectory(common)
add_subdirectory(src/Chapter01/01_hello_world_example)
add_subdirectory(src/Chapter01/02_Joinable_Detached_Threads)
# add_subdirectory(src/Chapter01/03_notification_chains)
add_subdirectory(src/Chapter01/04_race_condition_problem)

add_subdirectory(src/Chapter02_ThreadAPI/01_thread_creation)
add_subdirectory(src/Chapter02_ThreadAPI/02_thread_completion)
add_subdirectory(src/Chapter02_ThreadAPI/03a_locks_mutex)
add_subdirectory(src/Chapter02_ThreadAPI/03b_locks_interlocked_ops)
add_subdirectory(src/Chapter02_ThreadAPI/03c_locks_critical_section)
add_subdirectory(src/Chapter02_ThreadAPI/04_conditional_variables)

add_subdirectory(src/Chapter03_BuildingLocks/01_using_simple_flag)
add_subdirectory(src/Chapter03_BuildingLocks/02_working_spin_locks_test_and_set)
add_subdirectory(src/Chapter03_BuildingLocks/03_compare_and_swap)
add_subdirectory(src/Chapter03_BuildingLocks/04_load_linked_store_conditional)
add_subdirectory(src/Chapter03_BuildingLocks/05_ticket_turn_lock)

add_subdirectory(src/Chapter04_LockedDataStructures/01_thread_safe_counter_precise)
add_subdirectory(src/Chapter04_LockedDataStructures/02_thread_safe_counter_approx)
add_subdirectory(src/Chapter04_LockedDataStructures/03_concurrent_linked_lists)
add_subdirectory(src/Chapter04_LockedDataStructures/04_concurrent_queues)

add_subdirectory(src/Chapter05_ConditionalVariables/01_parent_wait_child_spinlock)
add_subdirectory(src/Chapter05_ConditionalVariables/02_simple_cond_var_demo)
add_subdirectory(src/Chapter05_ConditionalVariables/03_consumer_producer_wrong)

add_subdirectory(src/Chapter05_ConditionalVariables/04_consumer_producer_correct)

add_subdirectory(src/Chapter05_ConditionalVariables/05_consumer_producer_general)
