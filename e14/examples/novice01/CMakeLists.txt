# Add the library
add_library(novice01 SHARED src/fun.cc)

# Include the local headers
target_include_directories(novice01 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

# Add the executables
add_executable(n01 n01.cc)
add_executable(n02 n02.cc)

# Link the executables to the library
target_link_libraries(n01 novice01)
target_link_libraries(n02 novice01)

# Install the library
install(TARGETS novice01 DESTINATION lib)

# Install the executables
install(TARGETS n01 n02 DESTINATION bin)
