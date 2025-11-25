#include "GsimParticle/GsimXMessenger.h"
#include "GsimParticle/GsimX.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4RunManager.hh"
#include "G4VUserPhysicsList.hh"
#include "G4Transportation.hh"
#include "G4ProcessTable.hh"
#include "G4Decay.hh"

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"
#include "G4Version.hh"

#include <sstream>
#include <cmath>


GsimXMessenger::~GsimXMessenger()
{
  delete m_cmdAddXParticle;
}

GsimXMessenger::GsimXMessenger()
{
  std::string dirName="/GsimPhysicsList/";
  std::string commandName = dirName;
  commandName+="addXParticle";
  m_cmdAddXParticle = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
  m_cmdAddXParticle->SetGuidance("Add X particle.");
  m_cmdAddXParticle->SetGuidance("  addXParticle mass lifetime width charge 1 0");
  m_cmdAddXParticle->SetGuidance("    The last two are isStable isShortlived");
  m_cmdAddXParticle->SetGuidance("  214.3*MeV [1*s] [0] [0] [1] [0]");
  m_cmdAddXParticle->AvailableForStates(G4State_PreInit,G4State_Idle);
}


void GsimXMessenger::SetNewValue(G4UIcommand* command,G4String aValue)
{
  if(command==m_cmdAddXParticle) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    std::vector<double> vec;
    bool ok=true;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	vec.push_back(util->interpret(tmpout));
      }
    }
    double mass=214.3*MeV;
    double lifetime=1*s;
    double width=0.;
    double charge=0.;
    bool isStable=true;
    bool isShortlived=false;
    
    if(vec.size()>6) {
      //Warning
      GsimMessage::getInstance()
	->report("warning","Wrong number of arguments for addXParticle.");
    }
    
    if(vec.size()>=1)  mass=vec[0];
    if(vec.size()>=2)  lifetime=vec[1];
    if(vec.size()>=3)  width=vec[2];
    if(vec.size()>=4)  charge=vec[3];
    if(vec.size()>=5) { if(vec[4]==0)  isStable=false; }
    if(vec.size()==6) { if(vec[5]>0)  isShortlived=true; }
    GsimX::initialize(mass,lifetime,width,charge,isStable,isShortlived);
  }
}
