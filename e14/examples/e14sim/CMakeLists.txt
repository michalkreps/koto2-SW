# Add the executable
add_executable(e14sim e14sim.cc)

# Find system libraries
find_library(READLINE_LIBRARY readline)
find_library(NCURSES_LIBRARY ncurses)

# Link libraries
target_link_libraries(e14sim
  GsimSpectrum
  GsimE14Spectrum
  GsimE14Step2Spectrum
  GsimE14Detector
  GsimE14Step2Detector
  GsimUserDetector
  GsimDetector
  GsimTools
  GsimData
  GsimGun
  GsimParticle
  GsimPersistency
  GsimPhysicsList
  GsimMaterial
  GsimKernel
  GsimUIbash
  ${READLINE_LIBRARY}
  ${NCURSES_LIBRARY}
)

# Install the executable
install(TARGETS e14sim DESTINATION bin)
