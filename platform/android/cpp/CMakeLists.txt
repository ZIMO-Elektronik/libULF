cmake_minimum_required(VERSION 3.22)

project(android_wrapper)

add_subdirectory(../../.. ${CMAKE_CURRENT_BINARY_DIR}/ulf)

add_library(android_wrapper SHARED src/jni_context.cpp src/jni_libulf.cpp)

target_include_directories(android_wrapper PRIVATE . ../../../inc/libulf/)

target_link_libraries(android_wrapper PRIVATE ULF)
