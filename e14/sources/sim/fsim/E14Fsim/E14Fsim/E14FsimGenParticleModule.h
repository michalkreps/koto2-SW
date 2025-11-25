#ifndef E14FsimGenParticleModule_h
#define E14FsimGenParticleModule_h

#include "E14Fsim/E14FsimAnalysisModule.h"

class E14FsimGenParticleModule : public E14FsimAnalysisModule
{
 public:
  E14FsimGenParticleModule();
  virtual ~E14FsimGenParticleModule();

  virtual void initialize();
  virtual void beginOfRun();

  virtual void processEvent();

  virtual void endOfRun();
  virtual void terminate();

 protected:
  double KDecayMome[3];
  double KDecayPos[3];
  double KDecayTime;

  //Pi0 information
  int    Pi0number;
  double Pi0Mome[100][3];
  double Pi0Pos[100][3];
  double Pi0Time[100];

  //decayDaughter
  int    DecayDaughterNumber;
  int    DecayDaughterPID[10];
  double DecayDaughterMass[10];
  double DecayDaughterEkin[10];
  double DecayDaughterP[10][3];
};
#endif
