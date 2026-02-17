include(${CMAKE_CURRENT_LIST_DIR}/cst_sources.cmake)

add_library(cst STATIC ${CST_SOURCES} ${CST_HEADERS})

target_include_directories(cst PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

target_compile_definitions(cst PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

target_compile_features(cst PRIVATE cxx_std_20)

add_library(cst::cst ALIAS cst)

target_link_libraries(cst PUBLIC
    spdlog::spdlog
)

install(TARGETS cst
    LIBRARY DESTINATION lib
)