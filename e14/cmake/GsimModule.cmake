function(add_gsim_module module_name)
  # Glob the source files
  file(GLOB_RECURSE SOURCES "src/*.cc")

  # Glob the header files
  file(GLOB HEADERS "${module_name}/*.h")

  # Generate ROOT dictionaries if LinkDef files exist
  file(GLOB LINKDEF_FILES "${module_name}/*LinkDef.h")
  if(LINKDEF_FILES)
    set(DICT_SOURCES "")
    foreach(LINKDEF ${LINKDEF_FILES})
      get_filename_component(LINKDEF_BASE ${LINKDEF} NAME_WE)
      string(REPLACE "LinkDef" "" CLASS_NAME ${LINKDEF_BASE})
      set(DICT_NAME "${module_name}_${CLASS_NAME}_dict")
      set(HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${module_name}/${CLASS_NAME}.h")
      if(EXISTS ${HEADER_FILE})
        ROOT_GENERATE_DICTIONARY(${DICT_NAME} ${HEADER_FILE} LINKDEF ${LINKDEF})
        list(APPEND DICT_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${DICT_NAME}.cxx)
      endif()
    endforeach()
    list(APPEND SOURCES ${DICT_SOURCES})
  endif()

  # Read the REQUIREMENTS file
  set(REQUIREMENTS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/REQUIREMENTS")
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
