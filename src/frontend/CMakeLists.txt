
cmake_minimum_required(VERSION 3.15)

add_library(frontend
        gui.cpp
)

target_link_libraries(frontend PRIVATE
        gui_engine
        fltk::images
        fltk::fltk
        fltk::z
        lua::lua
        sol2
        backend
)

target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})