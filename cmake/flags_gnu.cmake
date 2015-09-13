if (LRCX_ENABLE_ASAN)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
		OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_LESS 4.8)
		message(WARNING "Your gcc version may not use Address-Sanitizer")
	endif()
	set(LRCX_COMPILER_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${LRCX_COMPILER_FLAGS}")
	set(LRCX_LINKER_FLAGS "-fsanitize=address -lasan ${LRCX_LINKER_FLAGS}")
endif()
