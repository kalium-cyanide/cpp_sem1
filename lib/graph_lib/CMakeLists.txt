cmake_minimum_required(VERSION 3.15)
include(FetchContent)

find_package(FLTK)

FetchContent_Declare(
        fltk
        GIT_REPOSITORY https://github.com/fltk/fltk.git
        GIT_TAG release-1.4.0
)
FetchContent_MakeAvailable(fltk)

add_library(graph_lib
        Graph.cpp
        GUI.cpp
        Window.cpp
        ext/graph.cpp
)

target_include_directories(graph_lib PRIVATE
        ${fltk_SOURCE_DIR}
)

target_link_libraries(graph_lib PRIVATE
        fltk
        fltk_images
        fltk_z
)

target_include_directories(graph_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
