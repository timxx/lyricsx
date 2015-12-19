cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(lyricsx)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(version "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(LRCX_RUNTIME_DIR "${CMAKE_INSTALL_PREFIX}/lyricsx/lib")

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
SET(CMAKE_INSTALL_RPATH "${LRCX_RUNTIME_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(src)
add_subdirectory(data)

if (LRCX_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
