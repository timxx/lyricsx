cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(lyricsx)

set(version 1.0.0)

include(CMakeParseArguments)
include(CheckIncludeFiles)
include(cmake/precheck.cmake)
include(cmake/macro.cmake)
include(cmake/options.cmake)
include_platform(flags)

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(src)

if (LRCX_ENABLE_TESTS)
    add_subdirectory(tests)
endif()
