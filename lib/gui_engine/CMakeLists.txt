cmake_minimum_required(VERSION 3.31.6)
include(FetchContent)


project(gui_engine CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gui_engine
        xml_parser.cpp
        css_parser.cpp
        layout_engine.cpp
        style_engine.cpp
        fltk_renderer.cpp
        browser_window.cpp
        html_widget.cpp
        html_widget.hpp
        preprocessor.cpp
        event_engine.cpp
        event_engine.hpp
)

target_link_libraries(gui_engine PRIVATE
        fltk
        fltk::z
        fltk::forms
        fltk::images
        yogacore
        pugixml
        tinyxml2
        lua::lua
        sol2
)

target_include_directories(gui_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})