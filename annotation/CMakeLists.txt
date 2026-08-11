find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(AnnotationApp ${SOURCES})

target_link_libraries(
    AnnotationApp
    PRIVATE Qt5::Core Qt5::Widgets 
)

target_include_directories(AnnotationApp PRIVATE include)

option(BUILD_TESTS "Build the test suite" ON)
if(BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
  enable_testing()
  add_subdirectory(tests)
endif()
