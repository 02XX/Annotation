find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

file(
  GLOB_RECURSE UNIT_TEST_SOURCES
  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(AnnotationUnitTests ${UNIT_TEST_SOURCES})
target_link_libraries(
  AnnotationUnitTests
  PRIVATE AnnotationCore GTest::gtest_main
)

gtest_discover_tests(AnnotationUnitTests)
