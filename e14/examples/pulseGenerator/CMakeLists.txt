# Add the executables
add_executable(pulseView pulseView.cc)
add_executable(pulseGen pulseGen.cc)

# Link the executables to the library
target_link_libraries(pulseView PulseGenerator)
target_link_libraries(pulseGen PulseGenerator)

# Install the executables
install(TARGETS pulseView pulseGen DESTINATION bin)
