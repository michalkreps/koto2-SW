/**
 *  @file
 *  @brief  GsimKLpipi0enuDecayChannel
 *  $Id$ 
 *  $Log$
 *  N. Shimizu
 */
#ifndef GsimKLpipi0enuDecayChannel_h
#define GsimKLpipi0enuDecayChannel_h

//includes
#include "globals.hh"
#include "G4ios.hh"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "G4VDecayChannel.hh"

//forward declaration


class GsimPiPi0ENuDecayChannel :public G4VDecayChannel
{
 public:
  GsimPiPi0ENuDecayChannel(const G4String& theParentName,
			G4double        theBR,
			const G4String& thePionName,
			const G4String& thePion0Name,
			const G4String& theEName,
			const G4String& theNuName
			);
  virtual ~GsimPiPi0ENuDecayChannel();
  virtual G4DecayProducts *DecayIt(G4double);

 private:
  double m_ml;  // lepton mass
  double m_mkl; // kl mass
  double m_mp1; // charged pi mass
  double m_mp0; // neutral pi mass
  double m_pi;  // pi = 3.141..
  
  double m_max_prob; // monitor of prob. for debug
  int    m_i;        // counter of rand for debug
  

  double probJ5(double spi, double se, double th_pi, double th_l, double phi);
  void FormF(double* f, double spi, double se, double th_pi);





};



#endif // GsimKLpipi0enuDecayChannel_h
