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

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Typest
        PRIVATE
        RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc/"
    )
else()
    target_compile_definitions(Typest
        PRIVATE
        RESOURCES_DIR="./rsc/"
    )

    set_target_properties(Typest PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    if (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rsc/)
        file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rsc/)
    endif()

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/rsc/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rsc/)
endif()
