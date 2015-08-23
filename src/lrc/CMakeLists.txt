add_library(lrc SHARED
	lrc.cpp
	lrc.h
	lrc_export.h
	lrcreader.cpp
	lrcreader.h
	lrcwriter.cpp
	lrcwriter.h
	)

generate_public_header(lrc
	lrc.h
	lrcreader.h
	lrcwriter.h
	)

set_target_properties(lrc
	PROPERTIES VERSION 1.0.0
	SOVERSION 1
	)

target_link_libraries(lrc
	utils
	Qt5::Core
	)
