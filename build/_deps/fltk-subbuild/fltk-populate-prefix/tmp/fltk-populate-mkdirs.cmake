# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/artem/cpp_sem1/build/_deps/fltk-src")
  file(MAKE_DIRECTORY "/home/artem/cpp_sem1/build/_deps/fltk-src")
endif()
file(MAKE_DIRECTORY
  "/home/artem/cpp_sem1/build/_deps/fltk-build"
  "/home/artem/cpp_sem1/build/_deps/fltk-subbuild/fltk-populate-prefix"
  "/home/artem/cpp_sem1/build/_deps/fltk-subbuild/fltk-populate-prefix/tmp"
  "/home/artem/cpp_sem1/build/_deps/fltk-subbuild/fltk-populate-prefix/src/fltk-populate-stamp"
  "/home/artem/cpp_sem1/build/_deps/fltk-subbuild/fltk-populate-prefix/src"
  "/home/artem/cpp_sem1/build/_deps/fltk-subbuild/fltk-populate-prefix/src/fltk-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/artem/cpp_sem1/build/_deps/fltk-subbuild/fltk-populate-prefix/src/fltk-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/artem/cpp_sem1/build/_deps/fltk-subbuild/fltk-populate-prefix/src/fltk-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
