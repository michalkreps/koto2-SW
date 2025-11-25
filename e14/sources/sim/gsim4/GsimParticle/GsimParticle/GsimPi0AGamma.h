#ifndef GsimPi0AGamma_h_h
#define GsimPi0AGamma_h 1

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DecayTable.hh"


class GsimPi0AGamma : public G4ParticleDefinition
{
 private:
  static GsimPi0AGamma* theInstance;
  
 private:
  
 public:
  ~GsimPi0AGamma(){}
  GsimPi0AGamma() {}
  
  static GsimPi0AGamma* Definition();
  static GsimPi0AGamma* Pi0AprimeGammaDefinition();
  static GsimPi0AGamma* Pi0AprimeGamma();
};

#endif



