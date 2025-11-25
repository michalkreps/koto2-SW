/**
 *  @file
 *  @brief  GsimKPiPiNuNuDecayChannel
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKPiPiNuNuDecayChannel_h
#define GsimKPiPiNuNuDecayChannel_h

//includes
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"

#include "G4VDecayChannel.hh"
#include "G4ios.hh"
#include "globals.hh"

//forward declaration


/**
 *  @class GsimKPiPiNuNuDecayChannel
 */
class GsimKPiPiNuNuDecayChannel : public G4VDecayChannel
{
public:
  /**
   *  @function GsimKPiPiNuNuDecayChannel
   *  @brief Constructor for GsimKPiPiNuNuDecayChannel
   */
  GsimKPiPiNuNuDecayChannel(const G4String& theParentName,
			    G4double        theBR,
			    const G4String& thPiKname0,
			    const G4String& thPiKname1,
			    const G4String& thNuKname0,
			    const G4String& thNuKname1
			    );
  /**
   *  @function ~GsimKPiPiNuNuDecayChannel
   *  @brief Destructor for GsimKPiPiNuNuDecayChannel
   */
  virtual ~GsimKPiPiNuNuDecayChannel();

  /**
   *  @function DecayIt
   *  @brief Returns the decay products.
   *  @return The decay products.
   */  
  virtual G4DecayProducts *DecayIt(G4double);
  
protected:
  
  TGenPhaseSpace* m_event;
  Double_t m_wmax;
  Double_t m_F1;
  Double_t m_F2;
  Double_t m_F3;
  Double_t m_F4;
  Double_t m_F6;
  Double_t m_F7;

  void generate();
  Double_t kx4gen(const TLorentzVector *pPi0,
		  const TLorentzVector *pPi1,
		  const TLorentzVector *pNu0,
		  const TLorentzVector *pNu1);

};

#endif // GsimKPiPiNuNuDecayChannel_h
