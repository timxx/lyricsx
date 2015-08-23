if (LRCX_ENABLE_ASAN)
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
		OUTPUT_VARIABLE CLANG_VERSION)
	if (CLANG_VERSION VERSION_LESS 3.8)
		message(WARNING "Your clang version may not use Address-Sanitizer")
	endif()
	set(LRCX_COMPILER_FLAGS "-fsanitize=address -fno-omit-frame-pointer ${LRCX_COMPILER_FLAGS}")
endif()
