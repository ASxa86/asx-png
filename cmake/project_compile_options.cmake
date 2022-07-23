function(project_compile_options)
	if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
		target_compile_options(${PROJECT_NAME} PRIVATE /wd4251)
		target_compile_options(${PROJECT_NAME} PRIVATE /permissive-)

	elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
		target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)

	elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
		target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
	endif()

	if(BUILD_WARNINGS_AS_ERRORS)
		if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
			target_compile_options(${PROJECT_NAME} PRIVATE /WX)

		elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
			target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
			target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

		elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
			target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
			target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
		endif()
	endif()
endfunction()
