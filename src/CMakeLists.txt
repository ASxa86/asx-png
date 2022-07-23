project(asx-png)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
	${PROJECT_INCLUDE_DIR}/asx-png/ReadFile.h
)

target_sources(${PROJECT_NAME} PRIVATE
	ReadFile.cpp
)

project_install_target()
project_compile_options()
project_gen_export()
