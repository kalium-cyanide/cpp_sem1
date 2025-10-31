cmake_minimum_required(VERSION 3.15)

add_library(frontend
        FltkFrontend.hpp
        PlayerWidget.cpp
        board_widget.cpp
        board_widget.hpp
        cell_widget.cpp
        cell_widget.hpp
)

target_include_directories(frontend PRIVATE
        ../src/backend
        ../../lib/graph_lib
)

target_link_libraries(frontend PRIVATE
        backend
        graph_lib
)

target_include_directories(frontend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})