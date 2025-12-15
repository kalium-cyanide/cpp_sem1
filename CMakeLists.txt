cmake_minimum_required(VERSION 3.15)
include(FetchContent)

project(cpp_sem1 VERSION 0.1.0 LANGUAGES C CXX)


FetchContent_Declare(
        fltk
        GIT_REPOSITORY https://github.com/fltk/fltk.git
        GIT_TAG release-1.4.0
)
FetchContent_Declare(
        lua
        GIT_REPOSITORY https://github.com/marovira/lua
        GIT_TAG 5.4.7
)
FetchContent_Declare(
        yoga
        GIT_REPOSITORY https://github.com/facebook/yoga
        GIT_TAG v3.2.1
)
FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml
        GIT_TAG v1.15
)
FetchContent_Declare(
        tinyxml2
        GIT_REPOSITORY https://github.com/leethomason/tinyxml2
        GIT_TAG 11.0.0
)
FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2
        GIT_TAG v3.5.0
)

FetchContent_MakeAvailable(fltk yoga pugixml tinyxml2 lua sol2)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(ENABLE_TESTS "Build unit tests" OFF)


add_executable(cpp_sem1
        main.cpp
)


add_subdirectory(src/backend)
add_subdirectory(src/frontend)
add_subdirectory(lib/gui_engine)

target_include_directories(cpp_sem1 PRIVATE
        ${fltk_SOURCE_DIR}
        lib/gui_engine
        src/frontend
        src/backend
)

target_link_libraries(cpp_sem1 PRIVATE
        frontend
        backend
)

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()