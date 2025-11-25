/**
 *  @file
 *  @brief  GsimKLpipigamma
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKLpipigamma_h
#define GsimKLpipigamma_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


class GsimKLpipigamma : public G4ParticleDefinition
{
 private:
   static GsimKLpipigamma* theInstance;
   GsimKLpipigamma(){}
   virtual ~GsimKLpipigamma(){}

 public:
   static GsimKLpipigamma* Definition();
   static GsimKLpipigamma* KLpipigammaDefinition();
   static GsimKLpipigamma* KLpipigamma();
};

#endif // GsimKLpipigamma_h
