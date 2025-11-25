/**
 *  $Id$
 *  $Log$
 */
#include "GsimSpectrum/GsimBetaDecaySpectrumMessenger.h"
#include "GsimSpectrum/GsimBetaDecaySpectrum.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UImanager.hh"

#include <cstdlib>
#include <string>

GsimBetaDecaySpectrumMessenger::GsimBetaDecaySpectrumMessenger(GsimBetaDecaySpectrum* spectrum)
  :m_spectrum(spectrum)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  std::string dirName="/GsimSpectrum/";
  dirName+=spectrum->getName();
  dirName+="/";
  m_spectrumDir = new G4UIdirectory(G4String(dirName.c_str()));
  m_spectrumDir->SetGuidance("Control GsimBetaDecaySpectrum.");
  
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
    commandName+="setBetaDecay";
    m_cmdSetBetaDecay = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetBetaDecay->SetGuidance("setBetaDecay val");
    m_cmdSetBetaDecay->SetGuidance(" A0 Z0 Z0 [A1 Z1 Q1]");
    m_cmdSetBetaDecay->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setBetaDecayR";
    m_cmdSetBetaDecayR = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetBetaDecayR->SetGuidance("setBetaDecayR r");
    m_cmdSetBetaDecayR->SetGuidance("set source radius in X-Y plain.");
    m_cmdSetBetaDecayR->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setBetaDecayTheta";
    m_cmdSetBetaDecayTheta = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetBetaDecayTheta->SetGuidance("setBetaDecayTheta theta");
    m_cmdSetBetaDecayTheta->SetGuidance("set direction in zenith angle, theta");
    m_cmdSetBetaDecayTheta->AvailableForStates(G4State_PreInit,G4State_Idle);
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


GsimBetaDecaySpectrumMessenger::~GsimBetaDecaySpectrumMessenger()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_cmdAddSpectrum;
  delete m_cmdClear;
  delete m_cmdSetBetaDecayTheta;
  delete m_cmdSetBetaDecayR;
  delete m_cmdSetBetaDecay;
  delete m_spectrumDir;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimBetaDecaySpectrumMessenger::SetNewValue(G4UIcommand* command,G4String aValue)
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
  } else if(command == m_cmdSetBetaDecay) {

    int A0,Z0,A1,Z1;
    double Q0,Q1;
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    bool ok=true;
    int okCount=0;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	if(okCount==0) {
	  A0=std::atoi(tmpout.data());
	  isValid=false;
	} else if(okCount==1) {
	  Z0=std::atoi(tmpout.data());
	  isValid=false;
	} else  if(okCount==2) {
	  Q0=std::atof(tmpout.data());
	  isValid=true;
	} else  if(okCount==3) {
	  A1=std::atoi(tmpout.data());
	  isValid=false;
	} else  if(okCount==4) {
	  Z1=std::atoi(tmpout.data());
	  isValid=false;
	} else  if(okCount==5) {
	  Q1=std::atof(tmpout.data());
	  isValid=true;
	} else {
	  isValid=false;
	}
	okCount++;
      }
    }
    if(isValid) {
      if(okCount==3) {
	m_spectrum->setBetaDecay(A0,Z0,Q0);
      } else if(okCount==6) {
	m_spectrum->setBetaDecay(A0,Z0,Q0,A1,Z1,Q1);
      }
    }
  } else if(command == m_cmdSetBetaDecayR) {
    GsimUtil* util = GsimUtil::getUtil();
    m_spectrum->setSourceR(util->interpret(aValue));
    isValid=true;
  } else if(command == m_cmdSetBetaDecayTheta) {
    GsimUtil* util = GsimUtil::getUtil();
    m_spectrum->setSourceTheta(util->interpret(aValue));
    isValid=true;
  } else if(command == m_cmdClear) {
    m_spectrum->clear();
    isValid=true;
  }
  if(!isValid) {
    std::string msg = "Wrong command  at GsimBetaDecaySpectrumMessenger::";
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
