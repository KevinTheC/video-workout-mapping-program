cmake_minimum_required(VERSION 3.10.2)

project("android-app")

#builds the first directory's source files into the second directory
add_subdirectory(
        ../../../../../cpp-core
        cpp-core-build
)

#create a library target named the first parameter, and a list of sources
add_library(
        android_wrapper_cpp
        SHARED
        cpp_core_wrapper.cpp
)

#uses the name defined in add_library and defines the include
target_include_directories(
        android_wrapper_cpp
        PRIVATE ../../../../../cpp-core/include
)

find_library(
        log-lib
        log
)
#uses the name defined in add_library and defines the libraries linked against
target_link_libraries(
        android_wrapper_cpp
        cpp_core
        ${log-lib}
)