include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
        test_figures.cpp
        test_pgn_parser.cpp
)

target_include_directories(unit_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src/backend
)
target_link_libraries(unit_tests
        backend
        gtest_main
)

add_test(NAME unit_tests COMMAND unit_tests)

target_compile_definitions(unit_tests PRIVATE
        PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)