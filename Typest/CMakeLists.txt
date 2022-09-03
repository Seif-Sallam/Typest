cmake_minimum_required(VERSION 3.16)

FILE(GLOB_RECURSE SOURCES ./src/Typest/*.cpp)

add_executable(
	Typest
	${SOURCES}
)

target_include_directories(
	Typest
	PRIVATE
	./include/
)

target_link_libraries(Typest
	PRIVATE
	sfml-graphics
    sfml-window
    sfml-system
	opengl32.lib
	fmt
)

target_compile_definitions(Typest
	PRIVATE
	RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc"
)