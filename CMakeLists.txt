cmake_minimum_required(VERSION 3.15)

project(cpp_sem1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(ENABLE_TESTS "Build unit tests" OFF)

add_executable(cpp_sem1
        #src/chess.cpp
        src/test.cpp
        lib/gui_engine/main.cpp
        lib/gui_engine/css_parser.cppm
)

add_custom_command(TARGET cpp_sem1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_subdirectory(src/backend)
add_subdirectory(src/frontend)
add_subdirectory(lib/graph_lib)

target_include_directories(cpp_sem1 PRIVATE
        ${fltk_SOURCE_DIR}
        lib/graph_lib
        src/frontend
        src/backend
)

target_link_libraries(cpp_sem1 PRIVATE
        fltk
        fltk_images
        fltk_z
        frontend
        backend
        graph_lib
)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()