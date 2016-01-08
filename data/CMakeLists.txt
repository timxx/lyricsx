if (UNIX)
	add_subdirectory(desktop)
endif()

add_subdirectory(translations)

install_files(${CMAKE_INSTALL_PREFIX}/share/licenses/lyricsx ../LICENSE)
