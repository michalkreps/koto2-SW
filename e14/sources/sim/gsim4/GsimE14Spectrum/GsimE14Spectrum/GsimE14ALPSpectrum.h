/**
 *  @file
 *  @brief  GsimE14ALPSpectrum
 *  $Id: $
 *  $Log: $
 */
#ifndef GsimE14ALPSpectrum_h
#define GsimE14ALPSpectrum_h

#include "GsimKernel/GsimSpectrum.h"
#include <string>
#include <vector>
#include <map>

#include "CLHEP/Vector/ThreeVector.h"

#include "TFile.h"
#include "TTree.h"

#ifndef M_PIl
# define M_PIl          3.1415926535897932384626433832795029L  /* pi */
#endif

class G4Event;

class GsimE14ALPSpectrum : public GsimSpectrum
{
 public:
  GsimE14ALPSpectrum(std::string name);
  virtual ~GsimE14ALPSpectrum();

 protected:
  GsimE14ALPSpectrum();
  virtual void   generateSpecial(const G4Event* anEvent);

  TFile* tf;
  TTree* tr;
  int m_cnt;
  int m_all;
};

#endif // GsimE14ALPSpectrum_h
