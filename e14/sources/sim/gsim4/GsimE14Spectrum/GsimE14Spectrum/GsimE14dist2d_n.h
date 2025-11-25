#ifndef GsimE14dist2d_n_h
#define GsimE14dist2d_n_h

#include "GsimKernel/GsimSpectrum.h"
#include <string>

class G4Event;

class GsimE14dist2d_n : public GsimSpectrum {
public:
  GsimE14dist2d_n(std::string name);
  
protected:
  virtual void generateSpecial(const G4Event* anEvent);
  
private:
  inline double mod(double x, double y);
  double lagrange(double* x, double* y, double xp0, int n);
  double lagrange(double x[], double y[], double xp0, int n, int ord);
  void sampl2d(double& xpart, double& ypart, double& zpart, double& px, double& py, double& pz);
};

#endif // GsimE14dist2d_n_h
