/**
 *  @file
 *  @brief  GsimKLpi02gamma
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKLeegamma_h
#define GsimKLeegamma_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


class GsimKLeegamma : public G4ParticleDefinition
{
 private:
   static GsimKLeegamma* theInstance;
   GsimKLeegamma(){}
   virtual ~GsimKLeegamma(){}

 public:
   static GsimKLeegamma* Definition();
   static GsimKLeegamma* KLeegammaDefinition();
   static GsimKLeegamma* KLeegamma();
};

#endif // GsimKLeegamma_h
