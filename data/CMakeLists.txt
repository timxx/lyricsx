if (UNIX)
	add_subdirectory(desktop)
endif()

install_files(${CMAKE_INSTALL_PREFIX}/share/licenses/lyricsx LICENSE)
