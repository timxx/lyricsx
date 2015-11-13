set(player_SRC
	mediaplayer.cpp
	mediaplayer.h
	player.cpp
	player.h
	playerfactory.cpp
	playerfactory.h
	player_export.h
	)

if (LRCX_HAVE_DBUS)
	set(player_SRC
		${player_SRC}
		mprisplayer.cpp
		mprisplayer.h
		mprisplayerhelper.cpp
		mprisplayerhelper.h
		)
endif()

add_library(player SHARED
	${player_SRC}
	)

set(player_PUB
	player.h
	playerfactory.h
	)

if (LRCX_HAVE_DBUS)
	set(player_PUB
		${player_PUB}
		mprisplayerhelper.h
		)
endif()


generate_public_header(player
	${player_PUB}
	)

set_target_properties(player
	PROPERTIES VERSION 1.0.0
	SOVERSION 1
	)

set(player_LIB
	Qt5::Core
	Qt5::Multimedia
	)

if (LRCX_HAVE_DBUS)
	set(player_LIB
		${player_LIB}
		Qt5::DBus
		)
endif()

target_link_libraries(player
	${player_LIB}
	)
