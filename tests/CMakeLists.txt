include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(Tests ${SRC})

include(${PROJECT_SOURCE_DIR}/cmake/get_test_data.cmake)
get_test_data(Tests)

target_include_directories(Tests PRIVATE ${PROJECT_SOURCE_DIR}/src
                                         ${PROJECT_SOURCE_DIR}/tests)

cpmaddpackage(URI "gh:google/googletest#main" OPTIONS "INSTALL_GTEST OFF")

target_link_libraries(Tests PRIVATE ${TARGET} GTest::gtest_main GTest::gmock)

gtest_discover_tests(Tests WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build/tests/)
