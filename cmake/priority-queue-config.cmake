# PRIORITY_QUEUE CMake configuration file:
# This file is meant to be placed in a cmake subfolder of PRIORITY_QUEUE-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(PRIORITY_QUEUE PROPERTIES
    URL "https://g10.app/status/#abstract_data_ii"
    DESCRIPTION "priority queue ADT"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(PRIORITY_QUEUE_FOUND TRUE)

# For compatibility with autotools PRIORITY_QUEUE-config.cmake, provide PRIORITY_QUEUE_* variables.

set_and_check(PRIORITY_QUEUE_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(PRIORITY_QUEUE_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(PRIORITY_QUEUE_INCLUDE_DIR  "${PRIORITY_QUEUE_PREFIX}/include")
set(PRIORITY_QUEUE_INCLUDE_DIRS           "${PRIORITY_QUEUE_INCLUDE_DIR}")
set_and_check(PRIORITY_QUEUE_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(PRIORITY_QUEUE_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(PRIORITY_QUEUE_LIBRARIES priority_queue::priority_queue)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated PRIORITY_QUEUE-target.cmake files.

set(_PRIORITY_QUEUE_library     "${PRIORITY_QUEUE_LIBDIR}/priority_queue.lib")
set(_PRIORITY_QUEUE_dll_library "${PRIORITY_QUEUE_BINDIR}/priority_queue.dll")
if(EXISTS "${_PRIORITY_QUEUE_library}" AND EXISTS "${_PRIORITY_QUEUE_dll_library}")
    if(NOT TARGET priority_queue::priority_queue)
        add_library(priority_queue::priority_queue SHARED IMPORTED)
        set_target_properties(priority_queue::priority_queue
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${PRIORITY_QUEUE_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_PRIORITY_QUEUE_library}"
                IMPORTED_LOCATION "${_PRIORITY_QUEUE_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "PRIORITY_QUEUE_SHARED"
                INTERFACE_PRIORITY_QUEUE_SHARED "ON"
        )
    endif()
    set(PRIORITY_QUEUE_PRIORITY_QUEUE_FOUND TRUE)
else()
    set(PRIORITY_QUEUE_PRIORITY_QUEUE_FOUND FALSE)
endif()
unset(_PRIORITY_QUEUE_library)
unset(_PRIORITY_QUEUE_dll_library)

check_required_components(PRIORITY_QUEUE)
