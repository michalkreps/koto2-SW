/**
 *  @file
 *  @brief  GsimE14CoreLikeK
 *  $Id: 
 *  $Log:
 */
#ifndef GsimE14CoreLikeK_h
#define GsimE14CoreLikeK_h

#include "GsimE14Spectrum/GsimE14SatoSpectrum.h"

class G4Event;

class GsimE14CoreLikeK : public GsimE14SatoSpectrum
{
public:
  GsimE14CoreLikeK(std::string name);
  virtual ~GsimE14CoreLikeK();

 protected:
  GsimE14CoreLikeK();
  virtual void   generateSpecial(const G4Event* anEvent);
};


#endif // GsimE14CoreLikeK_h





