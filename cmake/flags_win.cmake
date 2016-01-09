set(LRCX_COMPILER_FLAGS "")

if (LRCX_WARN_AS_ERROR)
	set(LRCX_COMPILER_FLAGS "${LRCX_COMPILER_FLAGS} /WX")
endif()

set(IGNORED_WARNINGS
	4251 # needs to have dll-interface to be used by clients of class
	4996 # This function or variable may be unsafe
	)

foreach(warning ${IGNORED_WARNINGS})
	set(CMAKE_C_FLAGS "$/wd${warning} ${CMAKE_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "/wd${warning} ${CMAKE_CXX_FLAGS}")
endforeach()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LRCX_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LRCX_COMPILER_FLAGS}")