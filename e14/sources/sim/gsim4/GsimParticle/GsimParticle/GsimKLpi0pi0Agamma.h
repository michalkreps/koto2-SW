/**
 *  @file
 *  @brief  GsimKLpi0pi0Agamma
 *  $Id:
 *  $Log:
 *
 */
#ifndef GsimKLpi0pi0Agamma_h
#define GsimKLpi0pi0Agamma_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


/**
 *  @class GsimKLpi0pi0Agamma
 *  @brief This class provides the KL0->pi0pi0(pi0->Agamma) decay.
 */

class GsimKLpi0pi0Agamma : public G4ParticleDefinition
{
 private:
   /**
    *  @brief The instance of this.
    */
   static GsimKLpi0pi0Agamma* theInstance;
   /**
    *  @function GsimKLpi0pi0Agamma
    *  @brief Constructor for GsimKLpi0pi0Agamma
    */
   GsimKLpi0pi0Agamma(){}
   /**
    *  @function ~GsimKLpi0pi0Agamma
    *  @brief Destructor for GsimKLpi0pi0Agamma
    */
   virtual ~GsimKLpi0pi0Agamma(){}

 public:
   /**
    *  @function Definition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    */
   static GsimKLpi0pi0Agamma* Definition();
   /**
    *  @function KLpi0pi0AgammaDefinition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0pi0Agamma* KLpi0pi0AgammaDefinition();
   /**
    *  @function KLpi0pi0Agamma
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0pi0Agamma* KLpi0pi0Agamma();
};

#endif // GsimKLpi0pi0Agamma_h
