find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB_RECURSE DXF_SOURCES CONFIGURE_DEPENDS src/IO/DXF/*.cpp)
file(GLOB_RECURSE DXF_HEADERS CONFIGURE_DEPENDS include/IO/DXF/*.hpp)
set(DXF_LOADER_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/IO/DXF/GDXFLoader.cpp")
set(DXF_LOADER_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/IO/DXF/GDXFLoader.hpp")
list(REMOVE_ITEM DXF_SOURCES "${DXF_LOADER_SOURCE}")
list(REMOVE_ITEM DXF_HEADERS "${DXF_LOADER_HEADER}")

# DXF is a standalone C++17 library. It intentionally has no Qt dependency.
add_library(TotcadDXF ${DXF_SOURCES} ${DXF_HEADERS})
target_include_directories(TotcadDXF PUBLIC include)

file(GLOB_RECURSE ANNOTATION_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE ANNOTATION_HEADERS CONFIGURE_DEPENDS include/*.hpp)
set(ANNOTATION_ENTRY_POINT "${CMAKE_CURRENT_SOURCE_DIR}/src/Entry.cpp")
list(REMOVE_ITEM ANNOTATION_SOURCES "${ANNOTATION_ENTRY_POINT}")
list(REMOVE_ITEM ANNOTATION_SOURCES ${DXF_SOURCES})
list(REMOVE_ITEM ANNOTATION_HEADERS ${DXF_HEADERS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(
    AnnotationCore
    ${ANNOTATION_SOURCES}
    ${ANNOTATION_HEADERS}
    resources/resources.qrc
)

target_link_libraries(
    AnnotationCore
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
    PRIVATE
        TotcadDXF
)

target_include_directories(AnnotationCore PUBLIC include)

add_executable(AnnotationApp "${ANNOTATION_ENTRY_POINT}")
target_link_libraries(AnnotationApp PRIVATE AnnotationCore)

set_target_properties(
    AnnotationApp
    PROPERTIES
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)
# ---- Tests ----
option(BUILD_TESTS "Build the test suite" ON)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
