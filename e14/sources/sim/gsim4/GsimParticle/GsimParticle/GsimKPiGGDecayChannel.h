/**
 *  @file
 *  @brief  GsimKPiGGDecayChannel
 *  $Id$
 *  $Log$
 */
#ifndef GsimKPiGGDecayChannel_h
#define GsimKPiGGDecayChannel_h

//includes
#include "G4VDecayChannel.hh"
#include "G4ios.hh"
#include "globals.hh"

//forward declaration
class GsimKPiGGDecay;

/**
 *  @class GsimKPiGGDecayChannel
 *  @brief This class provides the KLpi0gg decay channels.
 */
class GsimKPiGGDecayChannel :public G4VDecayChannel
{
  public:
  /**
   *  @function GsimKPiGGDecayChannel
   *  @brief Constructor for GsimKPiGGDecayChannel
   *  @param theParentName The name of the parent.
   *  @param theBR The branching ratio.
   *  @param thePionName1 The first daughter of the decay.
   *  @param thePionName2 The second daughter of the decay.
   *  @param thePionName3 The third daughter of the decay.
   *
   *  "thePionName3" should be "pi0".
   */
  GsimKPiGGDecayChannel(const G4String& theParentName,
		       G4double        theBR,
                       const G4String& thePionName1,
                       const G4String& thePionName2,
                       const G4String& thePionName3
                       );
  //  thePionName3 should be "pi0"

  /**
   *  @function ~GsimKPiGGDecayChannel
   *  @brief Destructor for GsimKPiGGDecayChannel
   */
  virtual ~GsimKPiGGDecayChannel();

  /**
   *  @function DecayIt
   *  @brief Returns the decay products.
   *  @return The decay products.
   */
  virtual G4DecayProducts *DecayIt(G4double);


protected:
  GsimKPiGGDecay* m_kpggd;
};  


#endif

