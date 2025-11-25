/**
 *  @file
 *  @brief  GsimKLpi02gammaDecayChannel
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKL2pi0gammaDecayChannel_h
#define GsimKL2pi0gammaDecayChannel_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4VDecayChannel.hh"

//forward declaration


class Gsim2Pi0GDecayChannel :public G4VDecayChannel
{
 public:
  Gsim2Pi0GDecayChannel(const G4String& theParentName,
			G4double        theBR,
			const G4String& thePionName1,
			const G4String& thePionName2,
			const G4String& theGammaName
			);
  virtual ~Gsim2Pi0GDecayChannel();
  virtual G4DecayProducts *DecayIt(G4double);

 private:
  double m_mKL;
  double m_mp0;
};



#endif // GsimKL2pi0gammaDecayChannel_h
