add_library(lrc SHARED
	lrc.cpp
	lrc.h
	lrc_export.h
	lrcreader.cpp
	lrcreader.h
	)

generate_public_header(lrc
	lrc.h
	lrcreader.h
	)

set_target_properties(lrc
	PROPERTIES VERSION 1.0.0
	SOVERSION 1
	)

target_link_libraries(lrc
	Qt5::Core
	)
