/**
 *  @file
 *  @brief  GsimKLpi0gammaX
 *  $Id: GsimKLpi0gammaX.h,v 1.3 2006/12/06 19:25:52 nanjo Exp $
 *  $Log: GsimKLpi0gammaX.h,v $
 *  Revision 1.3  2006/12/06 19:25:52  nanjo
 *  CVS variables.
 *
 */
#ifndef GsimKLpi0gammaX_h
#define GsimKLpi0gammaX_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


/**
 *  @class GsimKLpi0gammaX
 *  @brief This class provides the KL0->pi0 nu_e anti_nu_e, KL0->pi0 nu_mu anti_nu_mu, and KL0->pi0 nu_tau anti_nu_tau decays.
 */

class GsimKLpi0gammaX : public G4ParticleDefinition
{
 private:
   /**
    *  @brief The instance of this.
    */
   static GsimKLpi0gammaX* theInstance;
   /**
    *  @function GsimKLpi0gammaX
    *  @brief Constructor for GsimKLpi0gammaX
    */
   GsimKLpi0gammaX(){}
   /**
    *  @function ~GsimKLpi0gammaX
    *  @brief Destructor for GsimKLpi0gammaX
    */
   ~GsimKLpi0gammaX(){}

 public:
   /**
    *  @function Definition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    */
   static GsimKLpi0gammaX* Definition();
   /**
    *  @function KLpi0gammaXDefinition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0gammaX* KLpi0gammaXDefinition();
   /**
    *  @function KLpi0gammaX
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0gammaX* KLpi0gammaX();
   
 private:
};

#endif // GsimKLpi0gammaX_h
