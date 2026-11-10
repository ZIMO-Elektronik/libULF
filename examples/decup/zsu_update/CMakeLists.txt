file(GLOB_RECURSE SRC *.cpp)
add_executable(MxZsuUpdate ${SRC})

include(${PROJECT_SOURCE_DIR}/cmake/get_test_data.cmake)
get_test_data(MxZsuUpdate)

sanitize(address,undefined)

target_common_warnings(ZsuUpdate PRIVATE)
target_common_errors(ZsuUpdate PRIVATE)

target_link_libraries(MxZsuUpdate PRIVATE ULF)
