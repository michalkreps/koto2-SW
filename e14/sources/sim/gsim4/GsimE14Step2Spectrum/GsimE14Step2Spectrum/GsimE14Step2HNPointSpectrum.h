/**
 *  @file
 *  @brief  GsimE14Step2HNPointSpectrum
 *  $Id: 
 *  $Log:
 */
#ifndef GsimE14Step2HNPointSpectrum_h
#define GsimE14Step2HNPointSpectrum_h

#include "GsimE14Spectrum/GsimE14Spectrum.h"
#include <string>
#include <vector>
#include <map>

class G4Event;
class TF1;

class GsimE14Step2HNPointSpectrum : public GsimE14Spectrum
{
public:
  GsimE14Step2HNPointSpectrum(std::string name);
  virtual ~GsimE14Step2HNPointSpectrum();

 protected:
  GsimE14Step2HNPointSpectrum();
  virtual void   generateSpecial(const G4Event* anEvent);

  static double pKLDistExit( double Pk );
  static double pKLDist( double *x,double *par );
};


#endif // GsimE14Step2HNPointSpectrum_h





