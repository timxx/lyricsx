add_library(utils SHARED
	log.cpp
	log.h
	utils_export.h
	)

generate_public_header(utils
	log.h
	)

set_target_properties(utils
	PROPERTIES VERSION 1.0.0
	SOVERSION 1
	)
