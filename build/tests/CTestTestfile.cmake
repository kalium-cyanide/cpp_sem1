# CMake generated Testfile for 
# Source directory: /home/artem/cpp_sem1/tests
# Build directory: /home/artem/cpp_sem1/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(unit_tests "/home/artem/cpp_sem1/build/tests/unit_tests")
set_tests_properties(unit_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/artem/cpp_sem1/tests/CMakeLists.txt;22;add_test;/home/artem/cpp_sem1/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
