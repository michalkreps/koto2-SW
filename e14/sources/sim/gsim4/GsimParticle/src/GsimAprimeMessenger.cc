#include "GsimParticle/GsimAprimeMessenger.h"
#include "GsimParticle/GsimAprime.h"
#include "GsimParticle/GsimPi0AGamma.h"

#include "G4RunManager.hh"
#include "G4VUserPhysicsList.hh"


#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4SystemOfUnits.hh"
#include <sstream>
#include <cmath>

GsimAprimeMessenger::~GsimAprimeMessenger()
{
  delete m_cmdSetAmass;
}

GsimAprimeMessenger::GsimAprimeMessenger()
{
  std::string dirName="/GsimPhysicsList/";
  std::string commandName = dirName;
  commandName+="setAprimeMass";
  m_cmdSetAmass = new G4UIcmdWithADouble(G4String(commandName.c_str()),this);
  m_cmdSetAmass->SetGuidance("set the Aprime mass (MeV) (the mass must be non-negative && less than pi0 mass==134)");
  m_cmdSetAmass->AvailableForStates(G4State_PreInit,G4State_Idle);
}


void GsimAprimeMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if(command == m_cmdSetAmass){
    double Amass = m_cmdSetAmass->GetNewDoubleValue(newValue);
    // Cats are pretty :)
    if(std::isnan(Amass*1.0) || Amass>134 || Amass<0){
      G4cout << "Warning: Invalid value of Aprime mass. We assume that Aprime is massless!" << G4endl;
      Amass = 0;
    }
    
    G4RunManager* fRunManager = G4RunManager::GetRunManager();
    const G4VUserPhysicsList* pl = fRunManager->GetUserPhysicsList();
    
    G4ParticleDefinition* particle = GsimAprime::Initialize(Amass*MeV);
    const_cast<G4VUserPhysicsList*>(pl)->AddProcessManager(particle);
    particle = GsimPi0AGamma::Definition();
    const_cast<G4VUserPhysicsList*>(pl)->AddProcessManager(particle);
  }
}
