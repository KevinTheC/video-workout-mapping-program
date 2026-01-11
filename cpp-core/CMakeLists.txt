cmake_minimum_required(VERSION 3.12)

project(cpp_core)

file(GLOB CORE_SOURCES
    src/*.cpp
)

add_library(
    cpp_core
    STATIC
    ${CORE_SOURCES}
)

target_include_directories(
    cpp_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
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