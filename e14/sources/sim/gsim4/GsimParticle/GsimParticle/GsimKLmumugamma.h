/**
 *  @file
 *  @brief  GsimKLpi02gamma
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKLmumugamma_h
#define GsimKLmumugamma_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


class GsimKLmumugamma : public G4ParticleDefinition
{
 private:
   static GsimKLmumugamma* theInstance;
   GsimKLmumugamma(){}
   virtual ~GsimKLmumugamma(){}

 public:
   static GsimKLmumugamma* Definition();
   static GsimKLmumugamma* KLmumugammaDefinition();
   static GsimKLmumugamma* KLmumugamma();
};

#endif // GsimKLmumugamma_h
