# Add the executable
add_executable(gsim4VolumeFinder gsim4VolumeFinder.cc)

# Find system libraries
find_library(READLINE_LIBRARY readline)
find_library(NCURSES_LIBRARY ncurses)

# Link libraries
target_link_libraries(gsim4VolumeFinder
  GsimSpectrum
  GsimE14Spectrum
  GsimE14Step2Spectrum
  GsimE391Spectrum
  GsimE14Detector
  GsimE14Step2Detector
  GsimE391Detector
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
install(TARGETS gsim4VolumeFinder DESTINATION bin)
