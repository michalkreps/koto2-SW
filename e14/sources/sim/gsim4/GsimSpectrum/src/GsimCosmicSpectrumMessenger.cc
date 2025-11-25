/**
 *  $Id$
 *  $Log$
 */
#include "GsimSpectrum/GsimCosmicSpectrumMessenger.h"
#include "GsimSpectrum/GsimCosmicSpectrum.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UImanager.hh"

#include <cstdlib>
#include <vector>
#include <string>

GsimCosmicSpectrumMessenger::GsimCosmicSpectrumMessenger(GsimCosmicSpectrum* spectrum)
  :m_spectrum(spectrum)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif


  std::string dirName="/GsimSpectrum/";
  dirName+=spectrum->getName();
  dirName+="/";
  m_spectrumDir = new G4UIdirectory(G4String(dirName.c_str()));
  m_spectrumDir->SetGuidance("Control GsimCosmicSpectrum.");
  
  {
    std::string commandName=dirName;
    commandName+="addSpectrum";
    m_cmdAddSpectrum = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdAddSpectrum->SetGuidance("addSpectrum Special");
    m_cmdAddSpectrum->SetGuidance("Special spectrum prepared..");
    m_cmdAddSpectrum->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setCosmicRayDX";
    m_cmdSetCosmicRayDX = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetCosmicRayDX->SetGuidance("setCosmicRayDX val");
    m_cmdSetCosmicRayDX->SetGuidance(" CR Generation region in x is from -val+x to +val+x at y.");
    m_cmdSetCosmicRayDX->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setCosmicRayDZ";
    m_cmdSetCosmicRayDZ = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetCosmicRayDZ->SetGuidance("setCosmicRayDZ val");
    m_cmdSetCosmicRayDZ->SetGuidance(" CR Generation region in z is from -val+z to +val+z at y.");
    m_cmdSetCosmicRayDZ->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="clear";
    m_cmdClear = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdClear->SetGuidance("clear");
    m_cmdClear->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimCosmicSpectrumMessenger::~GsimCosmicSpectrumMessenger()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_cmdAddSpectrum;
  delete m_cmdClear;
  delete m_cmdSetCosmicRayDX;
  delete m_cmdSetCosmicRayDZ;
  delete m_spectrumDir;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimCosmicSpectrumMessenger::SetNewValue(G4UIcommand* command,G4String aValue)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  aValue=G4UImanager::GetUIpointer()->SolveAlias(aValue.data());

  bool isValid=false;
  if(command == m_cmdAddSpectrum) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    std::string tmpformula;
    bool ok=true;
    int okCount=0;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	if(okCount==0) {
	  tmpformula=std::string(tmpout.data());
	} 
	okCount++;
      }
    }
    if(okCount==1 && tmpformula=="Special") {
      m_spectrum->addSpectrum("Special",0,0);
      isValid=true;
    }
  } else if(command == m_cmdSetCosmicRayDX) {
    GsimUtil* util = GsimUtil::getUtil();
    m_spectrum->setCosmicRayDX(util->interpret(aValue));
    isValid=true;
  } else if(command == m_cmdSetCosmicRayDZ) {
    GsimUtil* util = GsimUtil::getUtil();
    m_spectrum->setCosmicRayDZ(util->interpret(aValue));
    isValid=true;
  } else if(command == m_cmdClear) {
    m_spectrum->clear();
    isValid=true;
  }
  if(!isValid) {
    std::string msg = "Wrong command  at GsimCosmicSpectrumMessenger::";
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
