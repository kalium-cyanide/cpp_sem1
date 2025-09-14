cmake_minimum_required(VERSION 4.0.0)
project(cpp_sem1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(FetchContent)

FetchContent_Declare(
  fltk
  GIT_REPOSITORY https://github.com/fltk/fltk.git
  GIT_TAG        release-1.4.4
)

FetchContent_MakeAvailable(fltk)

add_executable(cpp_sem1 src/chess.cpp)

target_include_directories(cpp_sem1 PRIVATE ${fltk_SOURCE_DIR})
target_link_libraries(cpp_sem1 PRIVATE fltk)