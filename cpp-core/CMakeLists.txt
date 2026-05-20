cmake_minimum_required(VERSION 3.12)

include(FetchContent)

project(cpp_core)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

#globbing doesn't change just because source files might change, so we need to redo this glob everytime a new file is made.
#common recommendation is to just list source files, but you can also just modify this file to update source file list
file(GLOB CORE_SOURCES
    src/*.cpp
)

add_library(
    cpp_core
    STATIC
    ${CORE_SOURCES}
)

target_link_libraries(cpp_core PUBLIC glm::glm)

target_include_directories(
    cpp_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
enable_testing()
# CREATE TESTS
file(GLOB TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

foreach(test_src ${TEST_SOURCES})
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE cpp_core)

    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endforeach()