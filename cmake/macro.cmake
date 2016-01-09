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

macro(INSTALL_BIN target)
	install(TARGETS ${target} RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endmacro()

macro(INSTALL_LIB target)
	install(TARGETS ${target} LIBRARY DESTINATION "${LRCX_RUNTIME_DIR}")
endmacro()

macro(INSTALL_FILES destination)
	install(FILES ${ARGN} DESTINATION ${destination})
endmacro()

macro(TRANSLATE_TS_FILES output out_dir)
	find_program(QT_LRELEASE_EXECUTABLE NAMES lrelease-qt5 lrelease DOC "lrelease path")

	file(MAKE_DIRECTORY ${out_dir})
	foreach(ts_file ${ARGN})
		get_filename_component(filename ${ts_file} NAME_WE)
		set(qm_file ${out_dir}/${filename}.qm)
		add_custom_command(
			OUTPUT ${qm_file}
			COMMAND ${QT_LRELEASE_EXECUTABLE} ARGS -silent ${ts_file} -qm ${qm_file}
			DEPENDS ${ts_file}
			COMMENT "Generating ${filename}.qm"
			)
		add_custom_target(trans_${filename} ALL DEPENDS ${qm_file})
		list(APPEND qm_files ${qm_file})
	endforeach()

	set(${output} ${qm_files})
endmacro()
