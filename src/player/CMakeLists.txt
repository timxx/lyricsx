add_library(player SHARED
	mediaplayer.cpp
	mediaplayer.h
	mprisplayer.cpp
	mprisplayer.h
	player.cpp
	player.h
	playerfactory.cpp
	playerfactory.h
	player_export.h
	)

generate_public_header(player
	player.h
	playerfactory.h
	)

set_target_properties(player
	PROPERTIES VERSION 1.0.0
	SOVERSION 1
	)

target_link_libraries(player
	Qt5::Core
	Qt5::Multimedia
	)
