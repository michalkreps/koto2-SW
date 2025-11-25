/**
 *  @file
 *  @brief  GsimKLmumugammaDecayChannel
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKLmumugammaDecayChannel_h
#define GsimKLmumugammaDecayChannel_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "G4VDecayChannel.hh"

//forward declaration


class GsimMMGDecayChannel :public G4VDecayChannel
{
 public:
  GsimMMGDecayChannel(const G4String& theParentName,
			G4double        theBR,
			const G4String& theMumName1,
			const G4String& theMupName2,
			const G4String& theGammaName
			);
  virtual ~GsimMMGDecayChannel();
  virtual G4DecayProducts *DecayIt(G4double);

 private:
  double m_eps; // mixing parameter (KL=K2+K1*eps)
  double m_ml;  // lepton mass
  double m_mkl; // kl mass
  double m_mp1; // charged pi mass
  double m_mn;  // nucleon mass
  double m_pi;  // pi = 3.141..
  
  double calc_F1(double t);
  double calc_F2(double t);

};



#endif // GsimKLmumugammaDecayChannel_h
