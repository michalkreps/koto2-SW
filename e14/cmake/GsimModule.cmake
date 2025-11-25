function(add_gsim_module module_name)
  # Glob the source files
  file(GLOB_RECURSE SOURCES "src/*.cc")

  # Glob the header files
  file(GLOB HEADERS "${module_name}/*.h")

  # Read the REQUIREMENTS file
  set(REQUIREMENTS_FILE "REQUIREMENTS")
  set(LINK_LIBRARIES "")
  if(EXISTS ${REQUIREMENTS_FILE})
    file(STRINGS ${REQUIREMENTS_FILE} LIBRARIES)
    foreach(lib ${LIBRARIES})
      if(NOT ${lib} MATCHES "^#.*")
        list(APPEND LINK_LIBRARIES ${lib})
      endif()
    endforeach()
  endif()

  # Add the library
  add_library(${module_name} SHARED ${SOURCES})

  # Link libraries
  target_link_libraries(${module_name} PUBLIC ${LINK_LIBRARIES} ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${CLHEP_LIBRARIES})

  # Include directories
  target_include_directories(${module_name} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  )

  # Install the library
  install(TARGETS ${module_name}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )

  # Install the headers
  install(FILES ${HEADERS}
    DESTINATION include/${module_name}
  )

endfunction()
