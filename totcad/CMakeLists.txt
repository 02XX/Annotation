find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB_RECURSE ANNOTATION_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE ANNOTATION_HEADERS CONFIGURE_DEPENDS include/*.hpp)
set(ANNOTATION_ENTRY_POINT "${CMAKE_CURRENT_SOURCE_DIR}/src/Entry.cpp")
list(REMOVE_ITEM ANNOTATION_SOURCES "${ANNOTATION_ENTRY_POINT}")

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
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
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

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
