/**
 *  @file
 *  @brief  GsimE14Step2IsoePointSpectrum
 *  $Id: 
 *  $Log:
 */
#ifndef GsimE14Step2IsoePointSpectrum_h
#define GsimE14Step2IsoePointSpectrum_h

#include "GsimE14Spectrum/GsimE14Spectrum.h"
#include <string>
#include <vector>
#include <map>

class G4Event;
class TF1;

class GsimE14Step2IsoePointSpectrum : public GsimE14Spectrum
{
public:
  GsimE14Step2IsoePointSpectrum(std::string name);
  virtual ~GsimE14Step2IsoePointSpectrum();

 protected:
  GsimE14Step2IsoePointSpectrum();
  virtual void   generateSpecial(const G4Event* anEvent);

  static double pKLDistExit( double Pk );
  static double pKLDist( double *x,double *par );
};


#endif // GsimE14Step2IsoePointSpectrum_h





