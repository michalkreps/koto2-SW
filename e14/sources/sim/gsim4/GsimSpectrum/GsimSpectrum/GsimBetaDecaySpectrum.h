/**
 *  @file
 *  @brief  GsimBetaDecaySpectrum
 *  $Id$
 *  $Log$
 */
#ifndef GsimBetaDecaySpectrum_h
#define GsimBetaDecaySpectrum_h

#include "GsimKernel/GsimSpectrum.h"

#include <string>

class GsimBetaDecay;
class G4Event;

class GsimBetaDecaySpectrum : public GsimSpectrum
{
public:
  GsimBetaDecaySpectrum(std::string name);
  virtual ~GsimBetaDecaySpectrum();

  
  void setBetaDecay(int A0,int Z0,double Q0);
  void setBetaDecay(int A0,int Z0,double Q0,int A1,int Z1,double Q1);
  void setSourceR(double r);
  void setSourceTheta(double theta);

 protected:
  GsimBetaDecaySpectrum();
  virtual void generateSpecial(const G4Event* anEvent);
  virtual void update();

  int    m_nCascade;//1 or 2
  /// beta decay Q value 
  double m_A[2];//A
  double m_Z[2];//positive for beta+, negatvie for beta-
  double m_Q[2];//Z for daughter nuclei
  GsimBetaDecay* m_betaDecay[2];

  /// beta decay source radius in X-Y plan
  double m_sourceR;

  /// beta decay direction theta
  double m_sourceTheta;
};

inline void GsimBetaDecaySpectrum::setBetaDecay(int A0,int Z0,double Q0)
{
  m_nCascade=1;
  m_A[0]=A0;
  m_Z[0]=Z0;
  m_Q[0]=Q0;
  update();
}

inline void GsimBetaDecaySpectrum::setBetaDecay(int A0,int Z0,double Q0,int A1,int Z1,double Q1)
{
  m_nCascade=2;
  m_A[0]=A0;
  m_Z[0]=Z0;
  m_Q[0]=Q0;
  m_A[1]=A1;
  m_Z[1]=Z1;
  m_Q[1]=Q1;
  update();
}

inline void GsimBetaDecaySpectrum::setSourceR(double r)
{
  m_sourceR=r;
}

inline void GsimBetaDecaySpectrum::setSourceTheta(double theta)
{
  m_sourceTheta=theta;
}

#endif // GsimBetaDecaySpectrum_h





