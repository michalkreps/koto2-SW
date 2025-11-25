/**
 *  @file
 *  @brief  GsimKLpi02gamma
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKL2pi0gamma_h
#define GsimKL2pi0gamma_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


class GsimKL2pi0gamma : public G4ParticleDefinition
{
 private:
   static GsimKL2pi0gamma* theInstance;
   GsimKL2pi0gamma(){}
   virtual ~GsimKL2pi0gamma(){}

 public:
   static GsimKL2pi0gamma* Definition();
   static GsimKL2pi0gamma* KL2pi0gammaDefinition();
   static GsimKL2pi0gamma* KL2pi0gamma();
};

#endif // GsimKL2pi0gamma_h
