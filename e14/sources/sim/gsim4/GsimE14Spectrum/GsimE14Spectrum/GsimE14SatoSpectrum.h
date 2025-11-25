/**
 *  @file
 *  @brief  GsimE14SatoSpectrum
 *  $Id: 
 *  $Log:
 */
#ifndef GsimE14SatoSpectrum_h
#define GsimE14SatoSpectrum_h

#include "GsimE14Spectrum/GsimE14Spectrum.h"
#include <string>
#include <vector>
#include <map>

class G4Event;
class TF1;

class GsimE14SatoSpectrum : public GsimE14Spectrum
{
public:
  GsimE14SatoSpectrum(std::string name);
  virtual ~GsimE14SatoSpectrum();

 protected:
  GsimE14SatoSpectrum();
  virtual void   generateSpecial(const G4Event* anEvent);

  static double pKLDistExit( double Pk );
  static double pKLDist( double *x,double *par );
};


#endif // GsimE14SatoSpectrum_h





