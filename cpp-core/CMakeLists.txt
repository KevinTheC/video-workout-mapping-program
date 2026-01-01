cmake_minimum_required(VERSION 3.10)

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
#CREATE TESTS
add_executable(test_core src/test.cpp)
target_link_libraries(test_core PRIVATE cpp_core)
enable_testing()
add_test(NAME core_test
         COMMAND $<TARGET_FILE:test_core>)