add_custom_target(api_docs ALL)
add_custom_command(
	TARGET api_docs
	COMMAND doxygen -q
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

