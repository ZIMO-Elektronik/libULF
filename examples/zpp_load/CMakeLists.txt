file(GLOB_RECURSE SRC *.cpp)
add_executable(ZppLoad ${SRC})

include(${PROJECT_SOURCE_DIR}/cmake/get_test_data.cmake)
get_test_data(ZppLoad)

sanitize(address,undefined)

target_common_warnings(ZppLoad PRIVATE)
target_common_errors(ZppLoad PRIVATE)

target_link_libraries(ZppLoad PRIVATE KLUG)
