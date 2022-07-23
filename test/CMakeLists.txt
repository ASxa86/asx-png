project(test-asx-gl)

add_executable(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE -DTEST_IMAGE="${CMAKE_CURRENT_SOURCE_DIR}/test.png")

target_sources(${PROJECT_NAME} PRIVATE
	ReadFile.test.cpp
)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
	asx-png
	GTest::gtest_main
	GTest::gtest
)

project_install_target()
project_compile_options()
