cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

# for cleaning build types
include(cmake/precheck.cmake)
project(lyricsx)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 1)
set(version "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(LRCX_RUNTIME_DIR "${CMAKE_INSTALL_PREFIX}/lib/lyricsx")

include(CMakeParseArguments)
include(CheckIncludeFiles)
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

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5DBus QUIET)

if (DEFINED Qt5DBus_FOUND)
	set(LRCX_HAVE_DBUS TRUE)
endif()

if (LRCX_ENABLE_TESTS)
	find_package(Qt5Test REQUIRED)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_INSTALL_RPATH "${LRCX_RUNTIME_DIR}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# for generating qm files only
set(LRCX_TRANSLATIONS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin/translations")

set(LRCX_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/lyricsx")
set(LRCX_TRANSLAIONS_DIR "${LRCX_DATA_DIR}/translations")

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)

# copy the Qt libs to bin dir
if (LRCX_ENABLE_TESTS)
	set(QT5_TESTLIB Qt5::Test)
endif()

if (LRCX_HAVE_DBUS)
	set(QT5_DBUSLIB Qt5::DBus)
endif()

copy_qt_libs(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	Qt5::Core
	Qt5::Widgets
	Qt5::Multimedia
	Qt5::Gui
	Qt5::Network # Qt5::Multimedia
	${QT5_DBUSLIB}
	${QT5_TESTLIB}
	)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(src)
add_subdirectory(data)

if (LRCX_ENABLE_TESTS)
	add_subdirectory(tests)
endif()
