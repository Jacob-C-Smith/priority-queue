# Need CMake 3.0
cmake_minimum_required (VERSION 3.28.0)

# This is the name of the repository
project ("priority queue")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the log module
if ( NOT "${HAS_LOG}")
    
    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()


# Add the priority queue project
if ( NOT "${HAS_PRIORITY_QUEUE}")

    # Log
    message("[priority queue] Adding priority queue project")
    
    # Set the include directory
    set(PRIORITY_QUEUE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the priority queue project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_PRIORITY_QUEUE true PARENT_SCOPE)
        set(PRIORITY_QUEUE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include PARENT_SCOPE)
    endif()
endif()

# Add source to this project's executable.
add_executable(priority_queue_example "main.c" "priority_queue.c")
add_dependencies(priority_queue_example priority_queue)
target_include_directories(priority_queue_example PUBLIC ${PRIORITY_QUEUE_INCLUDE_DIR})
target_link_libraries(priority_queue_example priority_queue)

# Add source to the tester
add_executable (priority_queue_test "priority_queue_test.c" "priority_queue.c")
add_dependencies(priority_queue_test sync priority_queue log)
target_include_directories(priority_queue_test PUBLIC ${PRIORITY_QUEUE_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(priority_queue_test sync priority_queue log)

# Add source to the library
add_library(priority_queue SHARED "priority_queue.c")
add_dependencies(priority_queue sync)
target_include_directories(priority_queue PUBLIC ${PRIORITY_QUEUE_INCLUDE_DIR} ${SYNC_INCLUDE_DIR})
target_link_libraries(priority_queue sync)
