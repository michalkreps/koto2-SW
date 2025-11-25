#ifndef E14FsimVetoModule_h
#define E14FsimVetoModule_h
#include "E14Fsim/E14FsimAnalysisModule.h"
#include <list>

class E14FsimVetoModule : public E14FsimAnalysisModule
{
 public:
  E14FsimVetoModule();
  virtual ~E14FsimVetoModule();

  virtual void beginOfRun();
  virtual void preClustering();
  virtual void processOneClusterPattern(std::list<int>& pattern);
  virtual void endOfEvent();
  
protected:
  size_t  m_hitVeto;

  //Veto
  int     gsimEventNumber;
  int     mcEventID;
  int     nHitVeto;
  int     VetoPID[1000];//[nHitVeto]
  int     VetoDetID[1000];//[nHitVeto]
  double  VetoPos[1000][3];
  double  VetoMome[1000][3];//[nHitVeto]
  double  VetoTime[1000];//[nHitVeto]
  double  VetoIneffi[1000];//[nHitVeto]
  double  DetVetoWeight[10];
};
#endif
