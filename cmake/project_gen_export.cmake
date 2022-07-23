macro(project_gen_export)
	set(EXPORT_FOLDER_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
	generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${EXPORT_FOLDER_DIR}/export.hxx BASE_NAME ${PROJECT_NAME})

	# https://stackoverflow.com/questions/25676277/cmake-target-include-directories-prints-an-error-when-i-try-to-add-the-source
	target_include_directories(${PROJECT_NAME} PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<INSTALL_INTERFACE:include>
	)

	install(FILES ${EXPORT_FOLDER_DIR}/export.hxx DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_ROOT_NAME}/${PROJECT_NAME})
endmacro()
