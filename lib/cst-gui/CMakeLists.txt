include(${CMAKE_CURRENT_LIST_DIR}/cst_gui_sources.cmake)

add_library(cst-gui STATIC ${CST_GUI_SOURCES} ${CST_GUI_HEADERS})

target_include_directories(cst-gui PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Link to core cst (GUI depends on it)
target_link_libraries(cst-gui PUBLIC
    cst::cst
)

add_library(cst::gui ALIAS cst-gui)

install(TARGETS cst-gui
    LIBRARY DESTINATION lib
)
