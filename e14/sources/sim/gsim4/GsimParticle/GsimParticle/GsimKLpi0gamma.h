/**
 *  @file
 *  @brief  GsimKLpi0pi0
 *  $Id: GsimKLpi0pi0.h,v 1.3 2006/12/06 19:25:52 nanjo Exp $
 *  $Log: GsimKLpi0pi0.h,v $
 *  Revision 1.3  2006/12/06 19:25:52  nanjo
 *  CVS variables.
 *
 */
#ifndef GsimKLpi0gamma_h
#define GsimKLpi0gamma_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

//forward declaration


/**
 *  @class GsimKLpi0gamma
 *  @brief This class provides the KL0->pi0pi0 decay.
 */

class GsimKLpi0gamma : public G4ParticleDefinition
{
 private:
   /**
    *  @brief The instance of this.
    */
   static GsimKLpi0gamma* theInstance;
   /**
    *  @function GsimKLpi0gamma
    *  @brief Constructor for GsimKLpi0gamma
    */
   GsimKLpi0gamma(){}
   /**
    *  @function ~GsimKLpi0gamma
    *  @brief Destructor for GsimKLpi0gamma
    */
   ~GsimKLpi0gamma(){}

 public:
   /**
    *  @function Definition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    */
   static GsimKLpi0gamma* Definition();
   /**
    *  @function KLpi0gammaDefinition
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0gamma* KLpi0gammaDefinition();
   /**
    *  @function KLpi0gamma
    *  @brief Returns the definition of the decay.
    *  @return The definition of the decay.
    *
    *  This is the same as Definition().
    */
   static GsimKLpi0gamma* KLpi0gamma();
};

#endif // GsimKLpi0gamma_h
