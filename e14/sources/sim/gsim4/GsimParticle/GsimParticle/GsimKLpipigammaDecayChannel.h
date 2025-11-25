/**
 *  @file
 *  @brief  GsimKLpipigammaDecayChannel
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKLpipigammaDecayChannel_h
#define GsimKLpipigammaDecayChannel_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4VDecayChannel.hh"

//forward declaration


class GsimPiPiGDecayChannel :public G4VDecayChannel
{
 public:
  GsimPiPiGDecayChannel(const G4String& theParentName,
			G4double        theBR,
			const G4String& thePionName1,
			const G4String& thePionName2,
			const G4String& theGammaName
			);
  virtual ~GsimPiPiGDecayChannel();
  virtual G4DecayProducts *DecayIt(G4double);

 private:
  double m_mkl; // kl mass
  double m_mp1; // charged pi mass
  double m_pi;  // pi = 3.141..

};



#endif // GsimKLpipigammaDecayChannel_h
