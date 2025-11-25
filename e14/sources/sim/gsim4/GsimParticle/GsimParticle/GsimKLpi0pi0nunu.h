/**
 *  @file
 *  @brief  GsimKLpi0pi0nunu
 *  $Id$
 *  $Log$
 */
#ifndef GsimKLpi0pi0nunu_h
#define GsimKLpi0pi0nunu_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


/**
 *  @class GsimKLpi0pi0nunu
 *  @brief This class provides the KL0->pi0 pi0 nu_e anti_nu_e, KL0->pi0 pi0 nu_mu anti_nu_mu, and KL0->pi0 pi0 nu_tau anti_nu_tau decays.
 */

class GsimKLpi0pi0nunu : public G4ParticleDefinition
{
 private:
   /**
    *  @brief The instance of this.
    */
   static GsimKLpi0pi0nunu* theInstance;
   /**
    *  @function GsimKLpi0pi0nunu
    *  @brief Constructor for GsimKLpi0pi0nunu
    */
   GsimKLpi0pi0nunu(){}
   /**
    *  @function ~GsimKLpi0pi0nunu
    *  @brief Destructor for GsimKLpi0pi0nunu
    */
   ~GsimKLpi0pi0nunu(){}

 public:
   /**
    *  @function Definition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    */
   static GsimKLpi0pi0nunu* Definition();
   /**
    *  @function KLpi0pi0nunuDefinition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0pi0nunu* KLpi0pi0nunuDefinition();
   /**
    *  @function KLpi0pi0nunu
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0pi0nunu* KLpi0pi0nunu();

 private:
};

#endif // GsimKLpi0pi0nunu_h
