/**
 *  $Id$
 *  $Log$
 */
#include "GsimE14Spectrum/GsimE14SpectrumMessenger.h"
#include "GsimE14Spectrum/GsimE14Spectrum.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UImanager.hh"

#include <cstdlib>
#include <vector>
#include <string>

GsimE14SpectrumMessenger::GsimE14SpectrumMessenger(GsimE14Spectrum* spectrum)
  :m_spectrum(spectrum)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif


  std::string dirName="/GsimSpectrum/";
  dirName+=spectrum->getName();
  dirName+="/";
  m_spectrumDir = new G4UIdirectory(G4String(dirName.c_str()));
  m_spectrumDir->SetGuidance("Control GsimE14Spectrum.");
  
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
    commandName+="setDetectorShiftZ";
    m_cmdSetDetectorShiftZ = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetDetectorShiftZ->SetGuidance("setDetectorShiftZ val");
    m_cmdSetDetectorShiftZ->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setBeamCondition";
    m_cmdSetBeamCondition = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetBeamCondition->SetGuidance("setBeamCondition yearMonth");
    m_cmdSetBeamCondition->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setParameter";
    m_cmdSetParameter= new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetParameter->SetGuidance("setParameter val");
    m_cmdSetParameter->AvailableForStates(G4State_PreInit,G4State_Idle);
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


GsimE14SpectrumMessenger::~GsimE14SpectrumMessenger()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_cmdAddSpectrum;
  delete m_cmdClear;
  delete m_cmdSetDetectorShiftZ;
  delete m_cmdSetBeamCondition;
  delete m_cmdSetParameter;
  delete m_spectrumDir;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimE14SpectrumMessenger::SetNewValue(G4UIcommand* command,G4String aValue)
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
    double      tmpxmin=-1;
    double      tmpxmax=-1;
    bool ok=true;
    int okCount=0;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	if(okCount==0) {
	  tmpformula=std::string(tmpout.data());
	} else if(okCount==1) {
	  tmpxmin=std::atof(tmpout.data());
	} else {
	  tmpxmax=std::atof(tmpout.data());
	}
	okCount++;
      }
    }
    if(okCount==1 && tmpformula=="Special") {
      m_spectrum->addSpectrum("Special",0,0);
      isValid=true;
    } else if (okCount==3) {
      m_spectrum->addSpectrum(tmpformula,tmpxmin,tmpxmax);
      isValid=true;
    }
  } else if(command == m_cmdSetDetectorShiftZ) {
    GsimUtil* util = GsimUtil::getUtil();
    m_spectrum->setDetectorShiftZ(util->interpret(aValue));
    isValid=true;
  } else if(command == m_cmdSetBeamCondition) {
    GsimUtil* util = GsimUtil::getUtil();
    m_spectrum->setBeamCondition(util->interpret(aValue));
    isValid=true;
  } else if(command == m_cmdSetParameter) {
    GsimUtil* util = GsimUtil::getUtil();
    m_spectrum->setParameter(util->interpret(aValue));
    isValid=true;
  } else if(command == m_cmdClear) {
    m_spectrum->clear();
    isValid=true;
  }
  if(!isValid) {
    std::string msg = "Wrong command  at GsimE14SpectrumMessenger::";
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
