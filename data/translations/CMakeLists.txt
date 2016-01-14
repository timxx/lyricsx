set(_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TS_FILES
	${_DIR}/zh_CN.ts
	${_DIR}/qt_zh_CN.ts
	)

translate_ts_files(QM_FILES ${LRCX_TRANSLATIONS_OUTPUT_DIR} ${TS_FILES})

install_files("${LRCX_TRANSLAIONS_DIR}" ${QM_FILES})
