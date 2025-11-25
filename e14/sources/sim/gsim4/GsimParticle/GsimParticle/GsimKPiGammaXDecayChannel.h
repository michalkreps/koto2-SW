/**
 *  @file
 *  @brief  GsimKPiGammaXDecayChannel
 *  $Id: GsimKPiGammaXDecayChannel.h,v $
 *  $Log: GsimKPiGammaXDecayChannel.h,v $
 */
#ifndef GsimKPiGammaXDecayChannel_h
#define GsimKPiGammaXDecayChannel_h

//includes
#include "G4VDecayChannel.hh"
#include "G4ios.hh"
#include "globals.hh"

//forward declaration


/**
 *  @class GsimKPiGammaXDecayChannel
 *  @brief This class provides the pi gamma A decay channel.
 */
class GsimKPiGammaXDecayChannel : public G4VDecayChannel
{
public:
  /**
   *  @function GsimKPiGammaXDecayChannel
   *  @brief Constructor for GsimKPiGammaXDecayChannel
   *  @param theParentName The name of the parent.
   *  @param theBR The branching ratio.
   *  @param thePionName1 The first daughter of the decay.
   *  @param thePionName2 The second daughter of the decay.
   *  @param thePionName3 The third daughter of the decay.
   */
  GsimKPiGammaXDecayChannel(const G4String& theParentName,
			     G4double        theBR,
			     const G4String& thePionName1,
			     const G4String& thePionName2,
			     const G4String& thePionName3
			     );
  //  thePionName1 should be "pi0"
  //  thePionName2,3 should be "gamma","Aprime,
  
  
  /**
   *  @function ~GsimKPiGammaXDecayChannel
   *  @brief Destructor for GsimKPiGammaXDecayChannel
   */
  virtual ~GsimKPiGammaXDecayChannel();

  /**
   *  @function DecayIt
   *  @brief Returns the decay products.
   *  @return The decay products.
   */  
  virtual G4DecayProducts *DecayIt(G4double);
  
  void SetDalitzParameterLambda(G4double aLambda);
protected:

  /**
   *  @function GetKPi0EnergyWeight
   *  @brief  The m^2(gamma,A) function.
   *  @param  m^2(gamma,A)
   *  @return The weight.
   */
  double getM2Weight( double m2 );

  /**
   *  @brief The mass of the pi0.
   */
  double m_mp;
  /**
   *  @brief The mass of the KL0.
   */
  double m_mk;
  
  double m_wmax;
  double m_mmsqmax;
};

#endif // GsimKPiGammaXDecayChannel_h
