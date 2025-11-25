# Add the executables
add_executable(kpigg kpigg.cc)
add_executable(phaseSpace phaseSpace.cc)
add_executable(compare compare.cc)

# Link the executables to the library
target_link_libraries(kpigg GsimTools)
target_link_libraries(phaseSpace GsimTools)
target_link_libraries(compare GsimTools)

# Install the executables
install(TARGETS kpigg phaseSpace compare DESTINATION bin)
