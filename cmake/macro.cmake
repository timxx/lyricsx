macro(INCLUDE_PLATFORM prefix)
	if (WIN32)
		include(cmake/${prefix}_win.cmake)
	elseif(UNIX)
		include(cmake/${prefix}_unix.cmake)
	else()
		message(FATAL_ERROR "Unsuppoted platform")
	endif()
endmacro()

macro(INCLUDE_COMPILER_FLAGS)
	string(TOLOWER ${CMAKE_CXX_COMPILER_ID} _compiler)
	include(cmake/flags_${_compiler}.cmake)
endmacro()

macro(GENERATE_PUBLIC_HEADER target)
	set(_include_dir "${PROJECT_BINARY_DIR}/include/${target}")
	file(MAKE_DIRECTORY ${_include_dir})
	foreach(file IN ITEMS ${ARGN})
		file(WRITE "${_include_dir}/${file}" "#include \"${CMAKE_CURRENT_SOURCE_DIR}/${file}\"")
	endforeach()
endmacro()

macro(ADD_TEST_TARGET target)
	cmake_parse_arguments(LRCX_TEST
		""
		""
		"SRCS;LIBS;"
		${ARGN}
		)
	add_executable(${target}
		${LRCX_TEST_SRCS}
		)
	target_link_libraries(${target}
		Qt5::Test
		${LRCX_TEST_LIBS}
		)
endmacro()
