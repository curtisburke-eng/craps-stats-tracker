include(${CMAKE_CURRENT_LIST_DIR}/cst_tests_sources.cmake)

add_executable(cst-tests ${CST_TEST_SOURCES} ${CST_TEST_HEADERS})

target_compile_definitions(cst-tests PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

target_include_directories(cst-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(GTest REQUIRED)

target_link_libraries(cst-tests PRIVATE
    cst::cst
    spdlog::spdlog
    GTest::GTest
)

set_target_properties(cst-tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(CTest)
add_test(NAME cst-tests COMMAND cst-tests)

install(TARGETS cst-tests
    RUNTIME DESTINATION bin
)