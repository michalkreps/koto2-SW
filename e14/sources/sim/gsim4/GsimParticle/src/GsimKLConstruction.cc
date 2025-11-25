/**
 *  @file
 *  @brief  GsimKLConstruction
 *  $Id: GsimKLConstruction.cc,v 1.5 2006/12/06 19:25:52 nanjo Exp $
 *  $Log: GsimKLConstruction.cc,v $
 *  Revision 1.5  2006/12/06 19:25:52  nanjo
 *  CVS variables.
 *
 */

#include "GsimPersistency/GsimMessage.h"
#include "GsimParticle/GsimKLConstruction.h"
#include "GsimParticle/GsimKLpi0nunu.h"
#include "GsimParticle/GsimKLpienu.h"
#include "GsimParticle/GsimKLpimunu.h"
#include "GsimParticle/GsimKL3pi0.h"
#include "GsimParticle/GsimKLpipipi0.h"
#include "GsimParticle/GsimKLpipi.h"
#include "GsimParticle/GsimKLpi0pi0.h"
#include "GsimParticle/GsimKLpi0gamma.h"
#include "GsimParticle/GsimKLpienugamma.h"
#include "GsimParticle/GsimKLpimunugamma.h"
#include "GsimParticle/GsimKL2gamma.h"
#include "GsimParticle/GsimKLbeta.h"
#include "GsimParticle/GsimKLpi02gamma.h"
#include "GsimParticle/GsimKLpi0pi0nunu.h"
#include "GsimParticle/GsimKLpi0pi0Agamma.h"
#include "GsimParticle/GsimKL2pi0gamma.h"
#include "GsimParticle/GsimKLeegamma.h"
#include "GsimParticle/GsimKLmumugamma.h"
#include "GsimParticle/GsimKLpipigamma.h"
#include "GsimParticle/GsimKLpi0gammaX.h"
#include "GsimParticle/GsimKLpipi0enu.h"

#include "GsimParticle/GsimKL2pi0gammaDecayChannel.h"
#include "GsimParticle/GsimKL3DecayChannel.h"
#include "GsimParticle/GsimKLBetaDecayChannel.h"
#include "GsimParticle/GsimKLeegammaDecayChannel.h"
#include "GsimParticle/GsimKLmumugammaDecayChannel.h"
#include "GsimParticle/GsimKLpienugammaDecayChannel.h"
#include "GsimParticle/GsimKLpipigammaDecayChannel.h"
#include "GsimParticle/GsimKPi3DecayChannel.h"
#include "GsimParticle/GsimKPiGGDecayChannel.h"
#include "GsimParticle/GsimKPiNuNuDecayChannel.h"
#include "GsimParticle/GsimKPiPiNuNuDecayChannel.h"
#include "GsimParticle/GsimKLpipi0enuDecayChannel.h"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"

GsimKLConstruction* GsimKLConstruction::s_KLConstruction = 0;

GsimKLConstruction* GsimKLConstruction::getKLConstruction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  if(!s_KLConstruction) {
    static GsimKLConstruction theKLConstruction;
    s_KLConstruction = &theKLConstruction;
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return s_KLConstruction;
}

GsimKLConstruction::GsimKLConstruction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_decayTableMode=0;
  m_originalKaon0LDecayTable=0;
  m_customKaon0LDecayTable=0;

  G4ParticleTable* pt = G4ParticleTable::GetParticleTable();
  if(!pt) {
    //Warning
    GsimMessage::getInstance()
      ->report("warning","No particle table for GsimKLConstruction.");
  }
  G4ParticleDefinition* pd=0;
  if(pt) pd=pt->FindParticle("kaon0L");
  if(!pd) {
    //Warning
    GsimMessage::getInstance()
      ->report("warning","No kaon0L for GsimKLConstruction.");
  }
  G4DecayTable* dt=0;
  if(pd) dt=pd->GetDecayTable();
  if(!dt) {
    //Warning
    GsimMessage::getInstance()
      ->report("warning","No DecayTable of kaon0L.");
  } else {
    //
    m_decayTableMode=0;
    m_originalKaon0LDecayTable=dt;
    //dt->DumpInfo();
  }
  
  m_KLlist.push_back(GsimKLpi0nunu::KLpi0nunuDefinition());
  m_KLlist.push_back(GsimKLpienu::KLpienuDefinition());
  m_KLlist.push_back(GsimKLpimunu::KLpimunuDefinition());
  m_KLlist.push_back(GsimKL3pi0::KL3pi0Definition());
  m_KLlist.push_back(GsimKLpipipi0::KLpipipi0Definition());
  m_KLlist.push_back(GsimKLpipi::KLpipiDefinition());
  m_KLlist.push_back(GsimKLpi0pi0::KLpi0pi0Definition());
  m_KLlist.push_back(GsimKLpi0gamma::KLpi0gammaDefinition());
  m_KLlist.push_back(GsimKLpienugamma::KLpienugammaDefinition());
  m_KLlist.push_back(GsimKLpimunugamma::KLpimunugammaDefinition());
  m_KLlist.push_back(GsimKL2gamma::KL2gammaDefinition());
  m_KLlist.push_back(GsimKLbeta::KLbetaDefinition());
  m_KLlist.push_back(GsimKLpi02gamma::KLpi02gammaDefinition());
  m_KLlist.push_back(GsimKLpi0pi0nunu::KLpi0pi0nunuDefinition());
  m_KLlist.push_back(GsimKLpi0pi0Agamma::KLpi0pi0AgammaDefinition());
  m_KLlist.push_back(GsimKL2pi0gamma::KL2pi0gammaDefinition());
  m_KLlist.push_back(GsimKLeegamma::KLeegammaDefinition());
  m_KLlist.push_back(GsimKLmumugamma::KLmumugammaDefinition());
  m_KLlist.push_back(GsimKLpipigamma::KLpipigammaDefinition());
  m_KLlist.push_back(GsimKLpi0gammaX::KLpi0gammaXDefinition());
  m_KLlist.push_back(GsimKLpipi0enu::KLpipi0enuDefinition());

  generateCustomDecayTable();

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimKLConstruction::~GsimKLConstruction()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  //If the table is assigned to the particle, it is automatically deleted.  
  //Avoid double delete for checking the assignment.
  if(m_decayTableMode!=0 && m_originalKaon0LDecayTable) {
    delete m_originalKaon0LDecayTable;
  }
  if(m_decayTableMode!=1 && m_customKaon0LDecayTable) {
    delete m_customKaon0LDecayTable;
  }

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


bool GsimKLConstruction::isGsimKL(G4ParticleDefinition* particleDefinition)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  for(std::list<G4ParticleDefinition*>::iterator it=m_KLlist.begin();
      it!=m_KLlist.end();it++) {
    
    if(particleDefinition==(*it)) {
      return true;
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return false;
}


void GsimKLConstruction::generateCustomDecayTable()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_customKaon0LDecayTable = new G4DecayTable();
  G4DecayTable* table = m_customKaon0LDecayTable;
  G4VDecayChannel** mode = 0;
  {
    //GsimKLpi0nunu
    mode = new G4VDecayChannel*[3];
    // kaon0L -> pi0 + nu_e + anti_nu_e
    mode[0] = new GsimKPiNuNuDecayChannel("kaon0L",1./3.,"pi0","nu_e","anti_nu_e");
    // kaon0L -> pi0 + nu_mu + anti_nu_mu
    mode[1] = new GsimKPiNuNuDecayChannel("kaon0L",1./3.,"pi0","nu_mu","anti_nu_mu");
    // kaon0L -> pi0 + nu_tau + anti_nu_tau
    mode[2] = new GsimKPiNuNuDecayChannel("kaon0L",1./3.,"pi0","nu_tau","anti_nu_tau");
    
    for (G4int index=0; index <3; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpienu
    mode = new G4VDecayChannel*[2];
    // kaon0L -> pi+ + e+ nu_e
    mode[0] = new GsimKL3DecayChannel("kaon0L",0,"pi-","e+","nu_e");
    // kaon0L -> pi- + e- + anti_nu_e
    mode[1] = new GsimKL3DecayChannel("kaon0L",0,"pi+","e-","anti_nu_e");
    for (G4int index=0; index <2; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpimunu
    mode = new G4VDecayChannel*[2];
    // kaon0L -> pi- + mu+ + nu_mu
    mode[0] = new GsimKL3DecayChannel("kaon0L",0,"pi-","mu+","nu_mu");
    // kaon0L -> pi- +  mu- + anti_nu_mu
    mode[1] = new GsimKL3DecayChannel("kaon0L",0,"pi+","mu-","anti_nu_mu");
    
    for (G4int index=0; index <2; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKL3pi0
    mode = new G4VDecayChannel*[1];
    // kaon0L -> pi0 + pi0 + pi0
    mode[0] = new GsimKPi3DecayChannel("kaon0L",0.,"pi0","pi0","pi0");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpipipi0
    mode = new G4VDecayChannel*[1];
    // kaon0L -> pi+ + pi- + pi0
    mode[0] = new GsimKPi3DecayChannel("kaon0L",0.,"pi+","pi-","pi0");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpipi
    mode = new G4VDecayChannel*[1];
    // kaon0L -> pi+ + pi-
    mode[0] = new G4PhaseSpaceDecayChannel("kaon0L",0.,2,"pi+","pi-");
    
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpi0pi0
    mode = new G4VDecayChannel*[1];
    // kaon0L -> pi0 + pi0
    mode[0] = new G4PhaseSpaceDecayChannel("kaon0L",0.,2,"pi0","pi0");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpi0gamma
    mode = new G4VDecayChannel*[1];
    // kaon0L -> pi0 + gamma
    mode[0] = new G4PhaseSpaceDecayChannel("kaon0L",0.,2,"pi0","gamma");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpienugamma
    mode = new G4VDecayChannel*[2];
    // kaon0L -> pi- + e+ + nu_e + gamma
    mode[0] = new GsimPiENuGammaDecayChannel("kaon0L",0.,"pi-","e+","nu_e","gamma");
    // kaon0L -> pi+ + e- + anti_nu_e + gamma
    mode[1] = new GsimPiENuGammaDecayChannel("kaon0L",0.,"pi+","e-","anti_nu_e","gamma");
    
    for (G4int index=0; index <2; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpimunugamma
    mode = new G4VDecayChannel*[2];
    // kaon0L -> pi- + mu+ + nu_mu + gamma
    mode[0] = new G4PhaseSpaceDecayChannel("kaon0L",0.,4,"pi+","mu+","nu_mu","gamma");
    // kaon0L -> pi+ + mu- + anti_nu_mu + gamma
    mode[1] = new G4PhaseSpaceDecayChannel("kaon0L",0.,4,"pi-","mu-","anti_nu_mu","gamma");
    
    for (G4int index=0; index <2; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKL2gamma
    mode = new G4VDecayChannel*[1];
    // kaon0L -> gamma + gamma
    mode[0] = new G4PhaseSpaceDecayChannel("kaon0L",0.,2,"gamma","gamma");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLbeta
    mode = new G4VDecayChannel*[2];
    mode[0] = new GsimKLBetaDecayChannel("kaon0L",0.,"kaon+","e-","anti_nu_e");
    mode[1] = new GsimKLBetaDecayChannel("kaon0L",0.,"kaon-","e+","nu_e");
    
    for (G4int index=0; index <2; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpi02gamma
    mode = new G4VDecayChannel*[1];
    mode[0] = new GsimKPiGGDecayChannel("kaon0L",0.,"pi0","gamma","gamma");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpi0pi0nunu
    mode = new G4VDecayChannel*[3];
    // kaon0L -> pi0 + pi0 + nu_e + anti_nu_e
    mode[0] = new GsimKPiPiNuNuDecayChannel("kaon0L",0.,"pi0","pi0","nu_e","anti_nu_e");
    // kaon0L -> pi0 + pi0 + nu_mu + anti_nu_mu
    mode[1] = new GsimKPiPiNuNuDecayChannel("kaon0L",0.,"pi0","pi0","nu_mu","anti_nu_mu");
    // kaon0L -> pi0 + pi0 + nu_tau + anti_nu_tau
    mode[2] = new GsimKPiPiNuNuDecayChannel("kaon0L",0.,"pi0","pi0","nu_tau","anti_nu_tau");
    
    for (G4int index=0; index <3; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  // {
  //   //GsimKLpi0pi0Agamma
  //   mode = new G4VDecayChannel*[1];
  //   // kaon0L -> pi0 + pi0 + pi0
  //   mode[0] = new GsimKPi3DecayChannel("kaon0L",0.,"pi0","pi0","pi0Agamma");
    
  //   for (G4int index=0; index <1; index++ ) {
  //     table->Insert(mode[index]);
  //   }
  //   delete [] mode;
  // }
  {
    //GsimKL2pi0gamma
    mode = new G4VDecayChannel*[1];
    mode[0] = new Gsim2Pi0GDecayChannel("kaon0L",0.,"pi0","pi0","gamma");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLeegamma
    mode = new G4VDecayChannel*[1];
    mode[0] = new GsimEEGDecayChannel("kaon0L",0.,"e-","e+","gamma");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLmumugamma
    mode = new G4VDecayChannel*[1];
    mode[0] = new GsimMMGDecayChannel("kaon0L",0.,"mu-","mu+","gamma");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }
  {
    //GsimKLpipigamma
    mode = new G4VDecayChannel*[1];
    mode[0] = new GsimPiPiGDecayChannel("kaon0L",0.,"pi-","pi+","gamma");
    
    for (G4int index=0; index <1; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }

  {
    //GsimKLpipi0enu
    mode = new G4VDecayChannel*[2];
    // kaon0L -> pi- + pi0 + e+ + nu_e 
    mode[0] = new GsimPiPi0ENuDecayChannel("kaon0L",0.,"pi-","pi0", "e+", "nu_e");
    // kaon0L -> pi+ + pi0 + e- + anti_nu_e 
    mode[1] = new GsimPiPi0ENuDecayChannel("kaon0L",0.,"pi+","pi0", "e-", "anti_nu_e");
    
    for (G4int index=0; index <2; index++ ) {
      table->Insert(mode[index]);
    }
    delete [] mode;
  }


  //m_customKaon0LDecayTable->DumpInfo();
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}




void GsimKLConstruction::switchDecayTable(int iMode)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  if( m_decayTableMode == iMode ) {
    //Do nothing
    //Warning
    GsimMessage::getInstance()
      ->report("info","Decay table of kaon0L is already set as you selected.");
    return;
  }
  G4ParticleTable* pt = G4ParticleTable::GetParticleTable();
  if(!pt) {
    //Warning
    GsimMessage::getInstance()
      ->report("warning","No particle table for GsimKLConstruction.");
  }
  G4ParticleDefinition* pd=0;
  if(pt) pd=pt->FindParticle("kaon0L");
  if(!pd) {
    //Warning
    GsimMessage::getInstance()
      ->report("warning","No kaon0L. Do nothing.");
    return; 
  }


  m_decayTableMode = iMode;
  if(iMode == 0) {
    pd->SetDecayTable(m_originalKaon0LDecayTable);
  } else {
    pd->SetDecayTable(m_customKaon0LDecayTable);
  }
    
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
 
