find_program(XDG_MIME xdg-mime)
find_program(UPDATE_ICON gtk-update-icon-cache)
find_program(UPDATE_DESKTOP update-desktop-database)

install(DIRECTORY icons DESTINATION ${CMAKE_INSTALL_PREFIX}/share)
install(FILES menus/lyricsx.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)

if (XDG_MIME)
	install(CODE "
		execute_process(COMMAND ${XDG_MIME} install
		--novendor ${CMAKE_CURRENT_SOURCE_DIR}/mimetypes/x-lyrics.xml)"
		)
endif()

if (UPDATE_ICON)
	install(CODE "
		execute_process(COMMAND ${UPDATE_ICON} -f -t -q
			\"${CMAKE_INSTALL_PREFIX}/share/icons/hicolor\")"
		)
endif()

if (UPDATE_DESKTOP)
	install(CODE "
		execute_process(COMMAND ${UPDATE_DESKTOP} -q)"
		)
endif()
