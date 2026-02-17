
include(${CMAKE_CURRENT_LIST_DIR}/app_sources.cmake)

add_executable(craps-stats-tracker ${CST_APP_SOURCES} ${CST_APP_HEADERS})

target_link_libraries(craps-stats-tracker PRIVATE
    cst::cst
    cst::gui
    spdlog::spdlog
)

set_target_properties(craps-stats-tracker PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

install(TARGETS craps-stats-tracker
    RUNTIME DESTINATION bin
)
