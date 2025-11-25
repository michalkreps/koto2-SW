#include "GsimParticle/GsimKLMessenger.h"
#include "GsimParticle/GsimKLConstruction.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4SystemOfUnits.hh"

GsimKLMessenger::~GsimKLMessenger()
{
  delete m_cmdSwitchKLDecayTable;
}

GsimKLMessenger::GsimKLMessenger()
{
  std::string dirName="/GsimPhysicsList/";
  std::string commandName = dirName;
  commandName+="switchKLDecayTable";
  m_cmdSwitchKLDecayTable = new G4UIcmdWithAnInteger(G4String(commandName.c_str()),this);
  m_cmdSwitchKLDecayTable->SetGuidance("Swich kaon0L decay table.");
  m_cmdSwitchKLDecayTable->SetGuidance("    swichKLDecayTable 0/1");
  m_cmdSwitchKLDecayTable->SetGuidance("    0: original one");
  m_cmdSwitchKLDecayTable->SetGuidance("    1: custom one");
  m_cmdSwitchKLDecayTable->AvailableForStates(G4State_PreInit,G4State_Idle);
}


void GsimKLMessenger::SetNewValue(G4UIcommand* command,G4String aValue)
{
  if(command==m_cmdSwitchKLDecayTable) {
    GsimKLConstruction* klC=GsimKLConstruction::getKLConstruction();
    klC->switchDecayTable( m_cmdSwitchKLDecayTable->GetNewIntValue(aValue) );
  }
}
