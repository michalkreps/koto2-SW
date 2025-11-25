/**
 *  @file
 *  @brief  GsimKLpi02gamma
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKLpi02gamma_h
#define GsimKLpi02gamma_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


/**
 *  @class GsimKLpi02gamma
 */

class GsimKLpi02gamma : public G4ParticleDefinition
{
 private:
   static GsimKLpi02gamma* theInstance;
   GsimKLpi02gamma(){}
   ~GsimKLpi02gamma(){}

 public:
   static GsimKLpi02gamma* Definition();
   static GsimKLpi02gamma* KLpi02gammaDefinition();
   static GsimKLpi02gamma* KLpi02gamma();
};

#endif // GsimKLpi02gamma_h
