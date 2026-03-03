cmake_minimum_required(VERSION 3.10)
project(e14 CXX C)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the installation prefix
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install)

# Find ROOT
find_package(ROOT REQUIRED COMPONENTS EG)
include(${ROOT_USE_FILE})

# Find Geant4 
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

# Find CLHEP
find_package(CLHEP REQUIRED)

# Add custom cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(GsimModule)

# Define global include directories
file(GLOB GSIM4_MODULE_DIRS "${PROJECT_SOURCE_DIR}/sources/sim/gsim4/Gsim*")
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${GSIM4_MODULE_DIRS}
  ${Geant4_INCLUDE_DIRS}
  ${ROOT_INCLUDE_DIRS}
  ${CLHEP_INCLUDE_DIRS}
)

# Define global library directories
link_directories(
  ${PROJECT_BINARY_DIR}/lib
  ${Geant4_LIBRARY_DIRS}
  ${ROOT_LIBRARY_DIRS}
  ${CLHEP_LIBRARIES}
)

# Add subdirectories
add_subdirectory(sources)
add_subdirectory(examples)
