cmake_minimum_required(VERSION 3.25)

project(
  Annotation
  VERSION 0.1.0
  DESCRIPTION "Annotation — a modern C++ annotation tool."
  LANGUAGES C CXX
)



# ---- C++ Standard ----
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The project is developed with Visual Studio 2015 and contains UTF-8
# source files with Chinese UI text. Keep MSVC's normal exception flags and
# add UTF-8 as an extra compile option instead of replacing CMAKE_CXX_FLAGS.
if(MSVC)
  add_compile_options(/utf-8)
endif()

# ---- Export compile commands ----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- CMake modules ----
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/Doxygen.cmake)

# ---- Tests ----
option(BUILD_TESTS "Build the test suite" ON)
if(BUILD_TESTS)
  enable_testing()
endif()

# ---- Engine source ----
add_subdirectory(totcad)

