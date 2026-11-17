file(GLOB_RECURSE SRC *.cpp)
add_executable(MxZppLoad ${SRC})

include(${PROJECT_SOURCE_DIR}/cmake/get_test_data.cmake)
get_test_data(MxZppLoad)

sanitize(address,undefined)

target_common_warnings(MxZppLoad PRIVATE)
target_common_errors(MxZppLoad PRIVATE)

target_include_directories(MxZppLoad PRIVATE ../../helper/)

target_link_libraries(MxZppLoad PRIVATE ULF)
