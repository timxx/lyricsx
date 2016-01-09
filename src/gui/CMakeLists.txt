if (WIN32)
	set(_TYPE WIN32)
	set(_WINMAINLIB Qt5::WinMain)
	set(_RC_FILE lyricsx.rc)
endif()

add_executable(lyricsx ${_TYPE}
	icons.qrc
	${_RC_FILE}
	application.cpp
	application.h
	lrceditor.cpp
	lrceditor.h
	lrchighlighter.cpp
	lrchighlighter.h
	main.cpp
	mainwindow.cpp
	mainwindow.h
	mainwindow.ui
	musicselector.cpp
	musicselector.h
	musicselector.ui
	playerwidget.cpp
	playerwidget.h
	playerwidget.ui
	preferences.cpp
	preferences.h
	preferences.ui
	encodingchooser.cpp
	encodingchooser.h
	encodingchooser.ui
	aboutdialog.cpp
	aboutdialog.h
	aboutdialog.ui
	i18n.cpp
	i18n.h
	)

target_link_libraries(lyricsx
	lrc
	player
	Qt5::Core
	Qt5::Widgets
	${_WINMAINLIB}
	)
	
target_enable_visual_styles(lyricsx)

install_bin(lyricsx)
