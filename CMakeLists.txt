cmake_minimum_required(VERSION 3.25)

project(
  Annotation
  VERSION 0.1.0
  DESCRIPTION "Annotation — a modern C++ annotation tool."
  LANGUAGES C CXX
)



# ---- C++ Standard ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Export compile commands ----
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- CMake modules ----
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/Doxygen.cmake)

# ---- Engine source ----
add_subdirectory(totcad)

