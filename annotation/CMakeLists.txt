find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

file(GLOB_RECURSE ANNOTATION_SOURCES CONFIGURE_DEPENDS src/*.cpp)
set(ANNOTATION_ENTRY_POINT "${CMAKE_CURRENT_SOURCE_DIR}/src/Entry.cpp")
list(REMOVE_ITEM ANNOTATION_SOURCES "${ANNOTATION_ENTRY_POINT}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(AnnotationCore ${ANNOTATION_SOURCES})

target_link_libraries(
    AnnotationCore
    PUBLIC Qt5::Core Qt5::Widgets
)

target_include_directories(AnnotationCore PUBLIC include)

add_executable(AnnotationApp "${ANNOTATION_ENTRY_POINT}")
target_link_libraries(AnnotationApp PRIVATE AnnotationCore)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
