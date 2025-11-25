/**
 *  @file
 *  @brief  GsimKLpienugammaDecayChannel
 *  $Id$ 
 *  $Log$ 
 */
#ifndef GsimKLpienugammaDecayChannel_h
#define GsimKLpienugammaDecayChannel_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "G4VDecayChannel.hh"

//forward declaration


class GsimPiENuGammaDecayChannel :public G4VDecayChannel
{
 public:
  GsimPiENuGammaDecayChannel(const G4String& theParentName,
			G4double        theBR,
			const G4String& theElectronName,
			const G4String& thePionName,
			const G4String& theNuName,
			const G4String& theGammaName
			);
  virtual ~GsimPiENuGammaDecayChannel();
  virtual G4DecayProducts *DecayIt(G4double);

 private:
  double me;  // lepton mass
  double mkl; // kl mass
  double mp1; // charged pi mass
  double pi;  // pi = 3.141..

  double Fp; // decay constant of pion
  double Fk; // decay constant of kaon
  
  double meta; // mass of eta
  double mrho; // mass of rho

  double V[4]; // vector form factor
  double A[4]; // axial form factor

  void init_formfactor();
  double f_plus(double t);

  double get_PDF(const TLorentzVector& Pe, const TLorentzVector& Pp, 
		  	const TLorentzVector& Pn, const TLorentzVector& Pg);

};



#endif // GsimKLpienugammaDecayChannel_h
