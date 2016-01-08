file(GLOB TS_FILES "*.ts")

translate_ts_files(QM_FILES ${LRCX_TRANSLATIONS_OUTPUT_DIR} ${TS_FILES})

install_files("${LRCX_TRANSLAIONS_DIR}" ${QM_FILES})
