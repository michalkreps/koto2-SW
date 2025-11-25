/**
 *  @file
 *  @brief  GsimKLpipi0enu
 *  $Id: GsimKLpipi0enu.h$
 *  $Log: GsimKLpipi0enu.h$
 *  N. Shimizu
 */
#ifndef GsimKLpipi0enu_h
#define GsimKLpipi0enu_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


/**
 *  @class GsimKLpipi0enu
 *  @brief This class provides the KL0->pi- e+ e_nu gamma and KL0->pi+ e- anti_e_nu gamma decays.
 */

class GsimKLpipi0enu : public G4ParticleDefinition
{
 private:
   /**
    *  @brief The instance of this.
    */
   static GsimKLpipi0enu* theInstance;
   /**
    *  @function GsimKLpipi0enu
    *  @brief Constructor for GsimKLpipi0enu
    */
   GsimKLpipi0enu(){}
   /**
    *  @function ~GsimKLpipi0enu
    *  @brief Destructor for GsimKLpipi0enu
    */
   ~GsimKLpipi0enu(){}

 public:
   /**
    *  @function Definition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    */
   static GsimKLpipi0enu* Definition();
   /**
    *  @function KLpipi0enuDefinition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpipi0enu* KLpipi0enuDefinition();
   /**
    *  @function KLpipi0enu
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpipi0enu* KLpipi0enu();
};

#endif // GsimKLpipi0enu_h
