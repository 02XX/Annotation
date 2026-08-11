option(BUILD_DOCS "Build documentation via Doxygen" OFF)

if(BUILD_DOCS)
  find_package(Doxygen QUIET)
  if(Doxygen_FOUND)
    doxygen_add_docs(docs
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/src
      COMMENT "Generate API documentation"
    )
  else()
    message(WARNING "Doxygen not found — BUILD_DOCS=ON has no effect")
  endif()
endif()
