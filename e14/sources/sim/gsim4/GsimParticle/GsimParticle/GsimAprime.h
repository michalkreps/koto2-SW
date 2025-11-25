#ifndef G4Aprime_h
#define G4Aprime_h 1

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DecayTable.hh"


class GsimAprime : public G4ParticleDefinition
{
 public:
  GsimAprime();
  virtual ~GsimAprime();
  
  static GsimAprime* Initialize(double Amass);
  static GsimAprime* Definition();
  static GsimAprime* AprimeDefinition();
  static GsimAprime* Aprime();
  
  
 private:
  static GsimAprime* theInstance;
  static double s_mass;
};

#endif



