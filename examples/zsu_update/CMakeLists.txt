file(GLOB_RECURSE SRC *.cpp)
add_executable(ZsuUpdate ${SRC})

include(${PROJECT_SOURCE_DIR}/cmake/get_test_data.cmake)
get_test_data(ZsuUpdate)

sanitize(address,undefined)

target_common_warnings(ZsuUpdate PRIVATE)
target_common_errors(ZsuUpdate PRIVATE)

target_link_libraries(ZsuUpdate PRIVATE KLUG)
