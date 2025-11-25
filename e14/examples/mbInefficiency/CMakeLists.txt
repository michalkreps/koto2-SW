# Add the executables
add_executable(drawMBIneffi drawMBIneffi.cc)
add_executable(drawMBOnlyIneffi drawMBOnlyIneffi.cc)
add_executable(drawIBIneffi drawIBIneffi.cc)
add_executable(drawZSBIneffi drawZSBIneffi.cc)

# Link the executables to the libraries
target_link_libraries(drawMBIneffi E14Fsim GsimData)
target_link_libraries(drawMBOnlyIneffi E14Fsim GsimData)
target_link_libraries(drawIBIneffi E14Fsim GsimData)
target_link_libraries(drawZSBIneffi E14Fsim GsimData)

# Install the executables
install(TARGETS drawMBIneffi drawMBOnlyIneffi drawIBIneffi drawZSBIneffi DESTINATION bin)
