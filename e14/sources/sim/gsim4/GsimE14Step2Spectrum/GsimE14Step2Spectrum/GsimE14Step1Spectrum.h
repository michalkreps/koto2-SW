/**
 *  @file
 *  @brief  GsimE14Step1Spectrum
 *  $Id: 
 *  $Log:
 */
#ifndef GsimE14Step1Spectrum_h
#define GsimE14Step1Spectrum_h

#include "GsimE14Spectrum/GsimE14Spectrum.h"
#include <string>
#include <vector>
#include <map>

class G4Event;
class TF1;

class GsimE14Step1Spectrum : public GsimE14Spectrum
{
public:
  GsimE14Step1Spectrum(std::string name);
  virtual ~GsimE14Step1Spectrum();

 protected:
  GsimE14Step1Spectrum();
  virtual void   generateSpecial(const G4Event* anEvent);

  static double pKLDistExit( double Pk );
  static double pKLDist( double *x,double *par );
};


#endif // GsimE14Step1Spectrum_h





