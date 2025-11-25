#include "E14Fsim/E14FsimBHPVModule.h"
#include "E14Fsim/E14FsimPersistencyManager.h"
#include "E14Fsim/E14FsimVeto.h"
#include "E14Fsim/E14FsimFunction.h"

#include "GsimData/GsimGenParticleData.h"
#include "GsimData/GsimDetectorEventData.h"

#include "TMath.h"
#include <cmath>


E14FsimBHPVModule::E14FsimBHPVModule()
  : E14FsimAnalysisModule()
{
  m_detID=9;
}

E14FsimBHPVModule::~E14FsimBHPVModule()
{
  ;
}


void E14FsimBHPVModule::processEvent()
{
  double BHCVineff = 0.005;
  int VetoID = m_detID+1;

  GsimDetectorEventData* detData
    =E14FsimPersistencyManager::getPersistencyManager()->getDetectorEventData(m_detID);
  TClonesArray* detHitArray = detData->hits;
  int nHit =detHitArray->GetEntries();
  
  for (int j=0; j<nHit; j++){
    GsimDetectorHitData* aHit = (GsimDetectorHitData*)(detHitArray)->At(j);
    
    double Mome = aHit->p.Mag();
    int PID = aHit->pid;
    
    E14FsimVeto* veto = new E14FsimVeto();
    if(PID == 22){                    // for gamma
      double ineffi=
	E14FsimFunction::getFunction()->catcherIneff(Mome);
      veto->setIneffi(ineffi);
    }
    else if (PID == 11 || PID == -11 || PID == 13 || PID == -13 ){ // for e and mu
      veto->setIneffi(BHCVineff);
    }
    else if (PID ==  211 || PID ==  -211 ){
      veto->setIneffi(BHCVineff);
    } else {
      veto->setIneffi(1.);
    }
    
    veto->setDetID(VetoID);
    veto->setPID(PID);
    veto->setMome(aHit->p.x(),
		  aHit->p.y(),
		  aHit->p.z());
    veto->setPos(aHit->r.x(),
		 aHit->r.y(),
		 aHit->r.z());
    veto->setTime(aHit->time);
    s_vetoList.push_back(veto);
  }
}

