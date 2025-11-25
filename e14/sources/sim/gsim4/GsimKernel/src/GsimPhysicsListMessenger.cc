/**
 *  @file
 *  $Id: $
 *  $Log: $
 */
#include "GsimKernel/GsimPhysicsListMessenger.h"
#include "GsimParticle/GsimKLpi0nunu.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimParticle/GsimXMessenger.h"
#include "GsimParticle/GsimAprimeMessenger.h"
#include "GsimParticle/GsimKLMessenger.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessTable.hh"
#include "G4VProcess.hh"
#include "G4Version.hh"
#include "G4DecayTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4VUserPhysicsList.hh"
#include "G4Transportation.hh"
#include "G4Decay.hh"
#include "G4VDecayChannel.hh"
#include "G4SystemOfUnits.hh"
         
#include <string>
#include <vector>
#include <cstdlib>

GsimPhysicsListMessenger::GsimPhysicsListMessenger()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  std::string dirName="/GsimPhysicsList/";
  m_physicsListDir = new G4UIdirectory(G4String(dirName.c_str()));
  m_physicsListDir->SetGuidance("Control GsimPhysicsList.");
  
  {
    std::string commandName=dirName;
    commandName+="setKLPi0NuNuDecayModel";
    m_cmdSetKLPi0NuNuDecayModel = new G4UIcmdWithAnInteger(G4String(commandName.c_str()),this);
    m_cmdSetKLPi0NuNuDecayModel->SetGuidance("Set KLPi0NuNu decay model.");
    m_cmdSetKLPi0NuNuDecayModel->SetGuidance("0: A model used in E391.");
    m_cmdSetKLPi0NuNuDecayModel->SetGuidance("1: Kmu3 dalitz density.");
    m_cmdSetKLPi0NuNuDecayModel->SetGuidance("2: Phase space.");
    m_cmdSetKLPi0NuNuDecayModel->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setKLPi0NuNuLambdaPlus";
    m_cmdSetKLPi0NuNuLambdaPlus = new G4UIcmdWithADouble(G4String(commandName.c_str()),this);
    m_cmdSetKLPi0NuNuLambdaPlus->SetGuidance("Set KLPi0NuNu dalitz parameter, lambda+.");
    m_cmdSetKLPi0NuNuLambdaPlus->SetGuidance("Default : 0.0284");
    m_cmdSetKLPi0NuNuLambdaPlus->SetGuidance("        : PDG 2006 (2.84+-0.04)x10^-2");
    m_cmdSetKLPi0NuNuLambdaPlus->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="addPhaseSpaceDecay";
    m_cmdAddPhaseSpaceDecay = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdAddPhaseSpaceDecay->SetGuidance("Add Phasespace Decay (max 4-body decay) ");
    m_cmdAddPhaseSpaceDecay->SetGuidance("addPhaseSpaceDecay parent daughter1 daughter2 ..");
    m_cmdAddPhaseSpaceDecay->SetGuidance("This Br=1, othres are 0.");
    m_cmdAddPhaseSpaceDecay->SetGuidance("If you want to change Br, do as follows.");
    m_cmdAddPhaseSpaceDecay->SetGuidance("   /particle/select kaon0L");
    m_cmdAddPhaseSpaceDecay->SetGuidance("   /particle/property/decay/select 0");
    m_cmdAddPhaseSpaceDecay->SetGuidance("   /particle/property/decay/br 0");
  }


  {
    std::string commandName=dirName;
    commandName+="withUserLimits";
    m_cmdWithUserLimits = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
    m_cmdWithUserLimits->SetGuidance("With/without GsimUserLimits for the  PhysicsList.");
    m_cmdWithUserLimits->SetGuidance("withUserLimits 1/0");
    m_cmdWithUserLimits->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="addCerenkov";
    m_cmdAddCerenkov = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdAddCerenkov->SetGuidance("Add Cerenkov process.");
    m_cmdAddCerenkov->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setDefaultCut";
    m_cmdSetDefaultCut = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetDefaultCut->SetGuidance("Set default cut.");
    m_cmdSetDefaultCut->SetGuidance("setDefaultCut 0.1*mm");
  }

  m_XMessenger = new GsimXMessenger();
  m_AprimeMessenger = new GsimAprimeMessenger();
  m_KLMessenger = new GsimKLMessenger();

  {
    std::string commandName=dirName;
    commandName+="selectParticle";
    m_cmdSelectParticle = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSelectParticle->SetGuidance("Select particle to modify properties.");
    m_cmdSelectParticle->SetGuidance("selectParticle name");
  }
  {
    std::string commandName=dirName;
    commandName+="setBR";
    m_cmdSetBR = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetBR->SetGuidance("Set branching ratio.");
    m_cmdSetBR->SetGuidance("setBR br      : to assign br for all the decay channels.");
    m_cmdSetBR->SetGuidance("setBR i-th br : to assign br for i-th decay channel.");
  }
  {
    std::string commandName=dirName;
    commandName+="dumpParticle";
    m_cmdDumpParticle = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdDumpParticle->SetGuidance("Dump particle properties.");
    m_cmdDumpParticle->SetGuidance("dumpParticle name.");
    m_cmdDumpParticle->SetGuidance("The particle is selected at the same time.");
  }

  m_currentParticle=0;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimPhysicsListMessenger::~GsimPhysicsListMessenger()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  delete m_cmdDumpParticle;
  delete m_cmdSetBR;
  delete m_cmdSelectParticle;
  delete m_KLMessenger;
  delete m_AprimeMessenger;
  delete m_XMessenger;
  delete m_cmdSetKLPi0NuNuDecayModel;
  delete m_cmdSetKLPi0NuNuLambdaPlus;
  //delete m_cmdAddXParticle;
  delete m_cmdAddPhaseSpaceDecay;
  delete m_cmdAddCerenkov;
  delete m_cmdWithUserLimits;
  delete m_cmdSetDefaultCut;
  delete m_physicsListDir;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif

}

void GsimPhysicsListMessenger::SetNewValue(G4UIcommand* command,G4String aValue)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  aValue=G4UImanager::GetUIpointer()->SolveAlias(aValue.data());
  
  if(command == m_cmdSetKLPi0NuNuDecayModel) {
    GsimKLpi0nunu::Definition()
      ->setDecayModel(m_cmdSetKLPi0NuNuDecayModel->GetNewIntValue(aValue));
  } else if (command == m_cmdSetKLPi0NuNuLambdaPlus) {
    GsimKLpi0nunu::Definition()
      ->setLambdaPlus(m_cmdSetKLPi0NuNuLambdaPlus->GetNewDoubleValue(aValue));
  } else if(command==m_cmdAddPhaseSpaceDecay) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    G4String pname[5];
    bool ok=true;
    int cnt=0;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	pname[cnt]=tmpout;
	cnt++;
	if(cnt>5) {
	  //Warning
	  GsimMessage::getInstance()
	    ->report("warning","Wrong number of arguments for addPhaseSpaceDecay.");
	  return;
	}
      }
    }
    G4PhaseSpaceDecayChannel* mode=0;
    if(cnt<3) {
      //Warning
      GsimMessage::getInstance()
	->report("warning","Wrong number of arguments for addPhaseSpaceDecay.");
      return;
    }
    G4ParticleTable* pt = G4ParticleTable::GetParticleTable();
    if(!pt) {
      //Warning
      GsimMessage::getInstance()
	->report("warning","No particle table for addPhaseSpaceDecay.");
      return;
    }
    G4ParticleDefinition* pd=pt->FindParticle(pname[0]);
    if(!pd) {
      //Warning
      GsimMessage::getInstance()
	->report("warning","No such parent particle for addPhaseSpaceDecay.");
      return;
    }
    if(cnt==3) {
      mode = new G4PhaseSpaceDecayChannel(pname[0],1.,2,pname[1],pname[2]);
    } else if(cnt==4) {
      mode = new G4PhaseSpaceDecayChannel(pname[0],1.,3,pname[1],pname[2],pname[3]);
    } else if(cnt==5) {
      mode = new G4PhaseSpaceDecayChannel(pname[0],1.,4,pname[1],pname[2],pname[3],pname[4]);
    }
    G4DecayTable* dt=pd->GetDecayTable();
    if(dt==0) {
      dt = new G4DecayTable();
    }
    int num=dt->entries();
    for(int i=0;i<num;i++) {
      G4VDecayChannel* dc = dt->GetDecayChannel(i);
      dc->SetBR(0.);
    }
    pd->SetDecayTable(dt);
    dt->Insert(mode);

//     Process should be added elsewhere.
//     For KL, it will be added in QGSP_BERT or similar places
//     For X,  it was added at the creation of X
//     G4ProcessManager* pmanager = pd->GetProcessManager();
//     if(!pmanager) {
// #if ( G4VERSION_NUMBER/10 < 100 )
//       G4RunManager* fRunManager = G4RunManager::GetRunManager();
//       const G4VUserPhysicsList* pl = fRunManager->GetUserPhysicsList();
//       const_cast<G4VUserPhysicsList*>(pl)->AddProcessManager(pd);
//       pmanager = pd->GetProcessManager();
// #else
//       pmanager = new G4ProcessManager(pd);
//       pd->SetProcessManager(pmanager);
// #endif      
//       pmanager = pd->GetProcessManager();
//     }

//     G4ProcessVector* procV=pmanager->GetProcessList();
//     bool isDecay = false;
//     for(int i=0;i<procV->size();i++) {
//       G4String procName=(*procV)[i]->GetProcessName();
//       if(procName=="Decay") {
// 	isDecay = true;
// 	break;
//       }
//     }
//     if(!isDecay) {
//       G4ProcessTable* procT=G4ProcessTable::GetProcessTable();
//       G4ProcessVector* procV=procT->FindProcesses("Decay");
//       G4VProcess* fDecayProcess = 0;
//       if(procV->size()==1) {
// 	fDecayProcess = (*procV)[0];
//       } else {
// 	fDecayProcess = new G4Decay();
//       }
//       pmanager -> AddProcess(fDecayProcess);
//       pmanager -> SetProcessOrdering(fDecayProcess, idxPostStep);
//       pmanager -> SetProcessOrdering(fDecayProcess, idxAtRest);
//     }
    
  } else if(command==m_cmdAddCerenkov) {
    GsimUtil* util = GsimUtil::getUtil();
    util->addCerenkov();
  } else if(command==m_cmdWithUserLimits) {
    GsimUtil* util = GsimUtil::getUtil();
    util->withUserLimits(m_cmdWithUserLimits->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetDefaultCut) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    if(util->shift(tmpline,tmpout)) {
      double defaultCutValue=util->interpret(tmpout);
      G4RunManager* fRunManager = G4RunManager::GetRunManager();
      const G4VUserPhysicsList* pl = fRunManager->GetUserPhysicsList();
      const_cast<G4VUserPhysicsList*>(pl)->SetDefaultCutValue(defaultCutValue);
      const_cast<G4VUserPhysicsList*>(pl)->SetCutsWithDefault();
    } else {
      GsimMessage::getInstance()
	->report("warning","Wrong command format.");
    }
  } else if(command==m_cmdSelectParticle) {
    G4ParticleTable* pt = G4ParticleTable::GetParticleTable();
    m_currentParticle = pt->FindParticle(aValue);
    if(m_currentParticle == 0) {
      std::string msg = "Unknown particle : " + std::string(aValue.data());
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command==m_cmdSetBR) {
    if(m_currentParticle == 0) {
      std::string msg = "No particle selected.";
      GsimMessage::getInstance()
	->report("warning",msg);
      return;
    }
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    G4String pname[2];
    bool ok=true;
    int cnt=0;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	pname[cnt]=tmpout;
	cnt++;
	if(cnt>2) {
	  //Warning
	  GsimMessage::getInstance()
	    ->report("warning","Wrong number of arguments for setBR.");
	  return;
	}
      }
    }
    if(cnt==0) {
      //Warning
      GsimMessage::getInstance()
	->report("warning","Wrong number of arguments for setBR.");
      return;
    } else {
      int nDC=0;
      G4DecayTable* dt=m_currentParticle->GetDecayTable();
      if(dt==0) {
	GsimMessage::getInstance()
	  ->report("warning","No decay table.");
	return;
      }
      nDC=dt->entries();
      if(cnt==1) {
	double BR=util->interpret(pname[0]);
	for(int i=0;i<nDC;i++) {
	  G4VDecayChannel* dc = dt->GetDecayChannel(i);
	  dc->SetBR(BR);
	}
      } else if(cnt==2) {
	int iDC=int( util->interpret( pname[0]) );
	double BR=util->interpret(pname[1]);
	if(iDC>=nDC) {
	  GsimMessage::getInstance()
	    ->report("warning","Decay channel larger than the number of decay channels.");
	  return;
	}
	G4VDecayChannel* dc = dt->GetDecayChannel(iDC);
	dc->SetBR(BR);
      }
    }
  } else if(command==m_cmdDumpParticle) {
    G4ParticleTable* pt = G4ParticleTable::GetParticleTable();
    m_currentParticle = pt->FindParticle(aValue);
    if(m_currentParticle == 0) {
      std::string msg = "Unknown particle : " + std::string(aValue.data());
      GsimMessage::getInstance()
	->report("warning",msg);
    }
    m_currentParticle->DumpTable();
  } else {
    std::string msg = "Wrong command  at GsimPhysicsListMessenger::";
    msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
    msg+=std::string(aValue.data()) ;
    msg+=")";
    GsimMessage::getInstance()
      ->report("warning",msg);
  } 
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  
}
