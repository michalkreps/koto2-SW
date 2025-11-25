#include "GsimKernel/GsimDetectorMessenger.h"
#include "GsimKernel/GsimDetector.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimTools/GsimDXFVolume.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UImanager.hh"

GsimDetectorMessenger::GsimDetectorMessenger(GsimDetector* detector)
  :m_detector(detector)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  std::string dirName="/GsimDetector";
  dirName+=detector->getFullName();
  dirName+="/";
  m_detectorDir = new G4UIdirectory(G4String(dirName.c_str()));
  G4String guid="Control GsimDetector : ";
  guid+= detector->getFullName();
  guid+= ".";
  m_detectorDir->SetGuidance(guid);
  
  {
    std::string commandName=dirName;
    commandName+="setLocalCenterVector";
    m_cmdSetLocalCenterVector = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetLocalCenterVector->SetGuidance("setLocalCenterVector x y z");
    m_cmdSetLocalCenterVector->SetGuidance("Set local center vector.");
    m_cmdSetLocalCenterVector->SetGuidance("Volume center from the original center.");
    m_cmdSetLocalCenterVector->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setTranslationVector";
    m_cmdSetTranslationVector = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetTranslationVector->SetGuidance("setTranslationVector x y z");
    m_cmdSetTranslationVector->SetGuidance("Set translation vector.");
    m_cmdSetTranslationVector->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setRotationVector";
    m_cmdSetRotationVector = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetRotationVector->SetGuidance("setRotationVector x y z");
    m_cmdSetRotationVector->SetGuidance("Set rotation vector (xAxisRotation->y->z).");
    m_cmdSetRotationVector->SetGuidance("Rotate around x ->y ->z");
    m_cmdSetRotationVector->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisMagneticField";
    m_cmdSetThisMagneticField = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisMagneticField->SetGuidance("setThisMagneticField Bx By Bz");
    m_cmdSetThisMagneticField->SetGuidance("Set this magnetic field.");
    m_cmdSetThisMagneticField->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterMagneticField";
    m_cmdSetThisAndDaughterMagneticField = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterMagneticField->SetGuidance("setThisAndDaughterMagneticField Bx By Bz");
    m_cmdSetThisAndDaughterMagneticField->SetGuidance("Set this and daughter magnetic field.");
    m_cmdSetThisAndDaughterMagneticField->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisUserStepMax";
    m_cmdSetThisUserStepMax = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisUserStepMax->SetGuidance("setThisUserStepMax stepMaxLimit");
    m_cmdSetThisUserStepMax->SetGuidance("Set This UserStepMax limit.");
    m_cmdSetThisUserStepMax->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisUserTrackMax";
    m_cmdSetThisUserTrackMax = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisUserTrackMax->SetGuidance("setThisUserTrackMax trackMaxLimit");
    m_cmdSetThisUserTrackMax->SetGuidance("Set this UserTrackMax limit.");
    m_cmdSetThisUserTrackMax->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisUserTimeMax";
    m_cmdSetThisUserTimeMax = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisUserTimeMax->SetGuidance("setThisUserTimeMax timeMaxLimit");
    m_cmdSetThisUserTimeMax->SetGuidance("Set this UserTimeMax limit.");
    m_cmdSetThisUserTimeMax->AvailableForStates(G4State_PreInit,G4State_Idle);
  }


  {
    std::string commandName=dirName;
    commandName+="setThisUserEkinMin";
    m_cmdSetThisUserEkinMin = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisUserEkinMin->SetGuidance("setThisUserEkinMin ekinMinLimit");
    m_cmdSetThisUserEkinMin->SetGuidance("Set this UserEkinMin limit.");
    m_cmdSetThisUserEkinMin->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisUserRangeMin";
    m_cmdSetThisUserRangeMin = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisUserRangeMin->SetGuidance("setThisUserRangeMin rangeMinLimit");
    m_cmdSetThisUserRangeMin->SetGuidance("Set this UserRangeMin limit.");
    m_cmdSetThisUserRangeMin->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="resetThisUserLimits";
    m_cmdResetThisUserLimits = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdResetThisUserLimits->SetGuidance("resetThisUserLimits");
    m_cmdResetThisUserLimits->SetGuidance("Reset this UserLimits");
    m_cmdResetThisUserLimits->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  
  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterUserStepMax";
    m_cmdSetThisAndDaughterUserStepMax = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterUserStepMax->SetGuidance("setThisAndDaughterUserStepMax stepMaxLimit");
    m_cmdSetThisAndDaughterUserStepMax->SetGuidance("Set This and daughterUserStepMax limit.");
    m_cmdSetThisAndDaughterUserStepMax->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterUserTrackMax";
    m_cmdSetThisAndDaughterUserTrackMax = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterUserTrackMax->SetGuidance("setThisAndDaughter UserTrackMax trackMaxLimit");
    m_cmdSetThisAndDaughterUserTrackMax->SetGuidance("Set this and daughter UserTrackMax limit.");
    m_cmdSetThisAndDaughterUserTrackMax->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterUserTimeMax";
    m_cmdSetThisAndDaughterUserTimeMax = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterUserTimeMax->SetGuidance("setThisAndDaughterUserTimeMax timeMaxLimit");
    m_cmdSetThisAndDaughterUserTimeMax->SetGuidance("Set this and daughter UserTimeMax limit.");
    m_cmdSetThisAndDaughterUserTimeMax->AvailableForStates(G4State_PreInit,G4State_Idle);
  }


  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterUserEkinMin";
    m_cmdSetThisAndDaughterUserEkinMin = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterUserEkinMin->SetGuidance("setThisAndDaughterUserEkinMin ekinMinLimit");
    m_cmdSetThisAndDaughterUserEkinMin->SetGuidance("Set this and daughter UserEkinMin limit.");
    m_cmdSetThisAndDaughterUserEkinMin->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterUserRangeMin";
    m_cmdSetThisAndDaughterUserRangeMin = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterUserRangeMin->SetGuidance("setThisAndDaughterUserRangeMin rangeMinLimit");
    m_cmdSetThisAndDaughterUserRangeMin->SetGuidance("Set this and daughter UserRangeMin limit.");
    m_cmdSetThisAndDaughterUserRangeMin->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="resetThisAndDaughterUserLimits";
    m_cmdResetThisAndDaughterUserLimits = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdResetThisAndDaughterUserLimits->SetGuidance("resetThisAndDaughterUserLimits");
    m_cmdResetThisAndDaughterUserLimits->SetGuidance("Reset this and daughter UserLimits");
    m_cmdResetThisAndDaughterUserLimits->AvailableForStates(G4State_PreInit,G4State_Idle);
  }


  {
    std::string commandName=dirName;
    commandName+="setOuterMaterial";
    m_cmdSetOuterMaterial = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetOuterMaterial->SetGuidance("setOuterMaterialName materialName");
    m_cmdSetOuterMaterial->SetGuidance("setOuterMaterialName G4_Fe");
    m_cmdSetOuterMaterial->SetGuidance("Set outer material name.");
    m_cmdSetOuterMaterial->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setOuterColor";
    m_cmdSetOuterColor = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetOuterColor->SetGuidance("setOuterColorName blue");
    m_cmdSetOuterColor->SetGuidance("Set outer color name.");
    m_cmdSetOuterColor->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setOuterVisibility";
    m_cmdSetOuterVisibility = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
    m_cmdSetOuterVisibility->SetGuidance("setOuterVisibility true/false");
    m_cmdSetOuterVisibility->SetGuidance("Set outer visibility.");
    m_cmdSetOuterVisibility->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterOuterVisibility";
    m_cmdSetThisAndDaughterOuterVisibility = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterOuterVisibility->SetGuidance("setThisAndDaughterOuterVisibility true/false");
    m_cmdSetThisAndDaughterOuterVisibility->SetGuidance("Set this and daughter outer visibility.");
    m_cmdSetThisAndDaughterOuterVisibility->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setParameters";
    m_cmdSetParameters = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetParameters->SetGuidance("setParameters xLength yLength zLength");
    m_cmdSetParameters->SetGuidance("setParameters ......");
    m_cmdSetParameters->SetGuidance("Set shape parameters.");
    m_cmdSetParameters->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAsARootRegion";
    m_cmdSetThisAsARootRegion = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAsARootRegion->SetGuidance("setThisAsARootRegion regionName productionCutValue");
    m_cmdSetThisAsARootRegion->SetGuidance("Set this as a root region ... regionName cutValue");
    m_cmdSetThisAsARootRegion->AvailableForStates(G4State_PreInit,G4State_Idle);
  }
  
  {
    std::string commandName=dirName;
    commandName+="setFastSimulationLevel";
    m_cmdSetFastSimulationLevel
      = new G4UIcmdWithAnInteger(G4String(commandName.c_str()),this);
    m_cmdSetFastSimulationLevel->SetGuidance("setFastSimulationLevel 0/1/2/3/4/5/6");
    m_cmdSetFastSimulationLevel->SetGuidance("  0: no fast simulation (default).");
    m_cmdSetFastSimulationLevel->SetGuidance("  1:");
    m_cmdSetFastSimulationLevel->SetGuidance("  2: suspend at briefDetector transit.");
    m_cmdSetFastSimulationLevel->SetGuidance("  3: shower(conv,eBrem,annihil) off");
    m_cmdSetFastSimulationLevel->SetGuidance("  4: stop when a step enter the volume");
    m_cmdSetFastSimulationLevel->SetGuidance("  5: stop when a step touch the volume");
    m_cmdSetFastSimulationLevel->SetGuidance("  6: special use for E14/E391 fast simulation.");
    m_cmdSetFastSimulationLevel->SetGuidance("      All charged veto counters are moved to far upstream.");
    m_cmdSetFastSimulationLevel->SetGuidance("      Dead mateirals are moved to far upstream or setSensitive.");
    m_cmdSetFastSimulationLevel->SetGuidance("Set fast simulation level (0..6).");
    m_cmdSetFastSimulationLevel->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setDaughterFastSimulationLevel";
    m_cmdSetDaughterFastSimulationLevel
      = new G4UIcmdWithAnInteger(G4String(commandName.c_str()),this);
    m_cmdSetDaughterFastSimulationLevel->SetGuidance("setDaughterFastSimulationLevel 0/1/2/3/4/5");
    m_cmdSetDaughterFastSimulationLevel->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setSensitiveVolume";
    m_cmdSetSensitiveVolume
      = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetSensitiveVolume->SetGuidance("setSensitiveVolume  LogicalVolumeName");
    m_cmdSetSensitiveVolume->SetGuidance("setSensitiveVolume  LogicalVolumeName SDname");
    m_cmdSetSensitiveVolume->SetGuidance("Set sensitiveDetector for the LogicalVolume.");
    m_cmdSetSensitiveVolume->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setSensitiveDetector";
    m_cmdSetSensitiveDetector
      = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdSetSensitiveDetector->SetGuidance("setSensitiveDetector");
    m_cmdSetSensitiveDetector->SetGuidance("Set SensitiveDetector, ");
    m_cmdSetSensitiveDetector->SetGuidance("     where SDname is logical volume name.");
    m_cmdSetSensitiveDetector->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setSensitiveDetectorWithName";
    m_cmdSetSensitiveDetectorWithName
      = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetSensitiveDetectorWithName->SetGuidance("setSensitiveDetector SDname [channel]");
    m_cmdSetSensitiveDetectorWithName->SetGuidance("Set sensitiveDetector  SDname [channel]");
    m_cmdSetSensitiveDetectorWithName->SetGuidance(" if [channel] is omitted, copyNo is used.");
    m_cmdSetSensitiveDetectorWithName->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterSensitiveDetectorWithName";
    m_cmdSetThisAndDaughterSensitiveDetectorWithName
      = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterSensitiveDetectorWithName->SetGuidance("setThisAndDaughterSensitiveDetector SDname [channel]");
    m_cmdSetThisAndDaughterSensitiveDetectorWithName->SetGuidance("Set sensitiveDetector  SDname [channel] for all daughters.");
    m_cmdSetThisAndDaughterSensitiveDetectorWithName->SetGuidance(" if [channel] is omitted, copyNo is used.");
    m_cmdSetThisAndDaughterSensitiveDetectorWithName->SetGuidance(" Already assigned channels are over written.");
    m_cmdSetThisAndDaughterSensitiveDetectorWithName->AvailableForStates(G4State_PreInit,G4State_Idle);
  }
  
  {
    std::string commandName=dirName;
    commandName+="setHitsStore";
    m_cmdSetHitsStore
      = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
    m_cmdSetHitsStore->SetGuidance("setHitsStore true/false");
    m_cmdSetHitsStore->SetGuidance("Set hits store. ");
    m_cmdSetHitsStore->SetGuidance("  default : false");
    m_cmdSetHitsStore->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterHitsStore";
    m_cmdSetThisAndDaughterHitsStore
      = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterHitsStore->SetGuidance("setThisAndDaughterHitsStore true/false");
    m_cmdSetThisAndDaughterHitsStore->SetGuidance("Set hits store of this and all daughters. ");
    m_cmdSetThisAndDaughterHitsStore->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setDigiStore";
    m_cmdSetDigiStore
      = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
    m_cmdSetDigiStore->SetGuidance("setDigiStore true/false");
    m_cmdSetDigiStore->SetGuidance("Set digi store. ");
    m_cmdSetDigiStore->SetGuidance(" default : true");
    m_cmdSetDigiStore->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterDigiStore";
    m_cmdSetThisAndDaughterDigiStore
      = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterDigiStore->SetGuidance("setThisAndDaughterDigiStore true/false");
    m_cmdSetThisAndDaughterDigiStore->SetGuidance("Set digi store of this and all daughters.");
    m_cmdSetThisAndDaughterDigiStore->SetGuidance(" default : true");
    m_cmdSetThisAndDaughterDigiStore->AvailableForStates(G4State_PreInit,G4State_Idle);
  }
	
  {
	std::string commandName=dirName;
	commandName+="setMtimeStore";
	m_cmdSetMtimeStore
	  = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
	m_cmdSetMtimeStore->SetGuidance("setMtimeStore true/false");
	m_cmdSetMtimeStore->SetGuidance("Set mtime store. ");
	m_cmdSetMtimeStore->SetGuidance(" default : true");
	m_cmdSetMtimeStore->AvailableForStates(G4State_PreInit,G4State_Idle);
  }
	
  {
	std::string commandName=dirName;
	commandName+="setThisAndDaughterMtimeStore";
	m_cmdSetThisAndDaughterMtimeStore
	  = new G4UIcmdWithABool(G4String(commandName.c_str()),this);
	m_cmdSetThisAndDaughterMtimeStore->SetGuidance("setThisAndDaughterMtimeStore true/false");
	m_cmdSetThisAndDaughterMtimeStore->SetGuidance("Set mtime store of this and all daughters.");
	m_cmdSetThisAndDaughterMtimeStore->SetGuidance(" default : true");
	m_cmdSetThisAndDaughterMtimeStore->AvailableForStates(G4State_PreInit,G4State_Idle);
  }
  
  {
    std::string commandName=dirName;
    commandName+="placeDetector";
    m_cmdPlaceDetector
      = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdPlaceDetector->SetGuidance("placeDetector motherFullName transX transY transZ rotX rotY rotZ");
    m_cmdPlaceDetector->SetGuidance("If detector is created at /, it can be place other volume directory.");
    m_cmdPlaceDetector->SetGuidance("After the placement, /GsimDetectorManager/renewMessenger is needed.");
    m_cmdPlaceDetector->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="cloneDetector";
    m_cmdCloneDetector
      = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdCloneDetector->SetGuidance("cloneDetector transX transY transZ rotX rotY rotZ channel shapeParam0,1,...");
    m_cmdCloneDetector->SetGuidance("      For only parameterized volume.");
    m_cmdCloneDetector->SetGuidance("        channel should be 0...multiplicity-1.");
    m_cmdCloneDetector->SetGuidance("        shape parameter can be assigned.");
    m_cmdCloneDetector->SetGuidance("cloneDetector transX transY transZ rotX rotY rotZ channel");
    m_cmdCloneDetector->SetGuidance("cloneDetector transX transY transZ rotX rotY rotZ");
    m_cmdCloneDetector->SetGuidance("cloneDetector transX transY transZ");
    m_cmdCloneDetector->SetGuidance("cloneDetector");
    m_cmdCloneDetector->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="clearThisClone";
    m_cmdClearThisClone
      = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdClearThisClone->SetGuidance("clearThisClone.");
    m_cmdClearThisClone->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setThisAndDaughterBriefName";
    m_cmdSetThisAndDaughterBriefName
      = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetThisAndDaughterBriefName->SetGuidance("setThisAndDaughterBriefName briefName");
    m_cmdSetThisAndDaughterBriefName->SetGuidance("Set this and daughter brief name.");
    m_cmdSetThisAndDaughterBriefName->AvailableForStates(G4State_PreInit,G4State_Idle);
  }


  {
    std::string commandName=dirName;
    commandName+="print";
    m_cmdPrint = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdPrint->SetGuidance("print");
    m_cmdPrint->SetGuidance("Print detector properties.");
    m_cmdPrint->SetGuidance("Units of shape parameters are mm or rad.");
    m_cmdPrint->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="update";
    m_cmdUpdate = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdUpdate->SetGuidance("update");
    m_cmdUpdate->SetGuidance("Update detector properties.");
    m_cmdUpdate->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setUserParameters";
    m_cmdSetUserParameters
      = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetUserParameters->SetGuidance("setUserParameters tag values");
    m_cmdSetUserParameters->SetGuidance("Modify user parameters");
    m_cmdSetUserParameters->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="dumpDaughters";
    m_cmdDumpDaughters = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdDumpDaughters->SetGuidance("dumpDaughters");
    m_cmdDumpDaughters->SetGuidance("Dump daughter detector lists.");
    m_cmdDumpDaughters->AvailableForStates(G4State_PreInit,G4State_Idle);
  }


  ////////////////////////
  // DXF
  
  {
    std::string commandName=dirName;
    commandName+="openDXF";
    m_cmdOpenDXF = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdOpenDXF->SetGuidance("openDXF fileName");
    m_cmdOpenDXF->SetGuidance("Open a DXF file.");
    m_cmdOpenDXF->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="closeDXF";
    m_cmdCloseDXF = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdCloseDXF->SetGuidance("closeDXF");
    m_cmdCloseDXF->SetGuidance("Close the DXF file.");
    m_cmdCloseDXF->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="addDXFLayer";
    m_cmdAddDXFLayer = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdAddDXFLayer->SetGuidance("addDXFLayer layerName");
    m_cmdAddDXFLayer->SetGuidance("Add DXF Layer.");
    m_cmdAddDXFLayer->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="dumpThisAndDaughterDXF";
    m_cmdDumpThisAndDaughterDXF = new G4UIcmdWithoutParameter(G4String(commandName.c_str()),this);
    m_cmdDumpThisAndDaughterDXF->SetGuidance("dumpThisAndDaughterDXF layerName");
    m_cmdDumpThisAndDaughterDXF->SetGuidance("Dump DXF for this and daughter detectors.");
    m_cmdDumpThisAndDaughterDXF->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setExportModeDXF";
    m_cmdSetExportModeDXF = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetExportModeDXF->SetGuidance("setExportModeDXF exportMode");
    m_cmdSetExportModeDXF->SetGuidance("  0: 3D lines");
    m_cmdSetExportModeDXF->SetGuidance("  1: 2D cross-section");
    m_cmdSetExportModeDXF->SetGuidance("  2: 3D cutaway");
    m_cmdSetExportModeDXF->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setCoordinateDXF";
    m_cmdSetCoordinateDXF = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetCoordinateDXF->SetGuidance("setCoordinateDXF xAxis.X,xAxis.Y,xAxis.Z,yAxis.X,yAxis.Y,yAxis.Z");
    m_cmdSetCoordinateDXF->SetGuidance("  new xAxis in the original coordinate.");
    m_cmdSetCoordinateDXF->SetGuidance("  new yAxis in the original coordinate.");
    m_cmdSetCoordinateDXF->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setCutSurfaceDXF";
    m_cmdSetCutSurfaceDXF = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetCutSurfaceDXF->SetGuidance("setCutSurfaceDXF xNormal yNormal zNormal xPos yPos zPos");
    m_cmdSetCutSurfaceDXF->SetGuidance("  Noraml vector");
    m_cmdSetCutSurfaceDXF->SetGuidance("  A point included in the surface.");
    m_cmdSetCutSurfaceDXF->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  {
    std::string commandName=dirName;
    commandName+="setScaleDXF";
    m_cmdSetScaleDXF = new G4UIcmdWithAString(G4String(commandName.c_str()),this);
    m_cmdSetScaleDXF->SetGuidance("setScaleDXF xScale yScale");
    m_cmdSetScaleDXF->SetGuidance("set  scale factor for x and y.");
    m_cmdSetScaleDXF->AvailableForStates(G4State_PreInit,G4State_Idle);
  }

  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


GsimDetectorMessenger::~GsimDetectorMessenger()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  
  delete m_cmdSetLocalCenterVector;
  delete m_cmdSetTranslationVector;
  delete m_cmdSetRotationVector;
  delete m_cmdSetThisMagneticField;
  delete m_cmdSetThisAndDaughterMagneticField;
  
  delete m_cmdSetThisUserStepMax;
  delete m_cmdSetThisUserTrackMax;
  delete m_cmdSetThisUserTimeMax;
  delete m_cmdSetThisUserEkinMin;
  delete m_cmdSetThisUserRangeMin;
  delete m_cmdResetThisUserLimits;
  delete m_cmdSetThisAndDaughterUserStepMax;
  delete m_cmdSetThisAndDaughterUserTrackMax;
  delete m_cmdSetThisAndDaughterUserTimeMax;
  delete m_cmdSetThisAndDaughterUserEkinMin;
  delete m_cmdSetThisAndDaughterUserRangeMin;
  delete m_cmdResetThisAndDaughterUserLimits;
  
  delete m_cmdSetOuterMaterial;
  delete m_cmdSetOuterColor;
  delete m_cmdSetOuterVisibility;
  delete m_cmdSetThisAndDaughterOuterVisibility;
  delete m_cmdSetParameters;
  delete m_cmdSetThisAsARootRegion;
  delete m_cmdSetFastSimulationLevel;
  delete m_cmdSetDaughterFastSimulationLevel;
  delete m_cmdSetSensitiveVolume;
  delete m_cmdSetSensitiveDetector;
  delete m_cmdSetSensitiveDetectorWithName;
  delete m_cmdSetThisAndDaughterSensitiveDetectorWithName;
  delete m_cmdSetHitsStore;
  delete m_cmdSetThisAndDaughterHitsStore;
  delete m_cmdSetDigiStore;
  delete m_cmdSetThisAndDaughterDigiStore;
  delete m_cmdSetMtimeStore;
  delete m_cmdSetThisAndDaughterMtimeStore;
  delete m_cmdPlaceDetector;
  delete m_cmdCloneDetector;
  delete m_cmdClearThisClone;
  delete m_cmdSetThisAndDaughterBriefName;
  delete m_cmdPrint;
  delete m_cmdUpdate;
  delete m_cmdSetUserParameters;
  delete m_cmdDumpDaughters;

  delete m_cmdOpenDXF;
  delete m_cmdCloseDXF;
  delete m_cmdAddDXFLayer;
  delete m_cmdDumpThisAndDaughterDXF;
  delete m_cmdSetExportModeDXF;
  delete m_cmdSetCoordinateDXF;
  delete m_cmdSetCutSurfaceDXF;
  delete m_cmdSetScaleDXF;
  
  delete m_detectorDir;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}



void GsimDetectorMessenger::SetNewValue(G4UIcommand* command,G4String aValue)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif



  aValue=G4UImanager::GetUIpointer()->SolveAlias(aValue.data());

  if( command == m_cmdSetUserParameters) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    std::string tag;
    std::vector<double> vec;
    bool ok=true;
    bool isFirst=true;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	if(isFirst) {
	  tag=tmpout.data();
	  isFirst=false;
	} else {
	  vec.push_back(util->interpret(tmpout));
	}
      }
    }
    if(isFirst) {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
      return;
    } else {
      m_detector->setUserParameters(tag,vec);
    }
  } else if( command == m_cmdSetLocalCenterVector ||
	     command == m_cmdSetTranslationVector ||
	     command == m_cmdSetRotationVector ||
	     command == m_cmdSetThisMagneticField || 
	     command == m_cmdSetThisAndDaughterMagneticField) {
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
    if(vec.size()!=3) {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
      return;
    }
    G4ThreeVector tvec(vec[0],vec[1],vec[2]);
    if(command == m_cmdSetLocalCenterVector ) {
      m_detector->setLocalCenterVector(tvec);
    } else if(command == m_cmdSetTranslationVector) {
      m_detector->setTranslationVector(tvec);
    } else if(command == m_cmdSetRotationVector) {
      m_detector->setRotationVector(tvec);
    } else if(command == m_cmdSetThisMagneticField) {
      m_detector->setThisMagneticField(tvec);
    } else if(command == m_cmdSetThisAndDaughterMagneticField) {
      m_detector->setThisAndDaughterMagneticField(tvec);
    }
  } else if(command == m_cmdSetThisUserStepMax ||
 	    command == m_cmdSetThisUserTrackMax ||
 	    command == m_cmdSetThisUserTimeMax ||
 	    command == m_cmdSetThisUserEkinMin ||
 	    command == m_cmdSetThisUserRangeMin ||
 	    command == m_cmdSetThisAndDaughterUserStepMax ||
 	    command == m_cmdSetThisAndDaughterUserTrackMax ||
 	    command == m_cmdSetThisAndDaughterUserTimeMax ||
 	    command == m_cmdSetThisAndDaughterUserEkinMin ||
 	    command == m_cmdSetThisAndDaughterUserRangeMin ) {
     GsimUtil* util = GsimUtil::getUtil();
     G4String tmpline = aValue;
     G4String tmpout;
     bool ok=util->shift(tmpline,tmpout);
     if(!ok) {
       return;
     }
     double cut=util->interpret(tmpout);
     if(command == m_cmdSetThisUserStepMax)
       {m_detector->setThisUserStepMax(cut);}
     else if(command == m_cmdSetThisUserTrackMax)
       {m_detector->setThisUserTrackMax(cut); }
     else if(command == m_cmdSetThisUserTimeMax)
       {m_detector->setThisUserTimeMax(cut);}
     else if(command == m_cmdSetThisUserEkinMin)
       {m_detector->setThisUserEkinMin(cut);}
     else if(command == m_cmdSetThisUserRangeMin)
       {m_detector->setThisUserRangeMin(cut);}
     else if(command == m_cmdSetThisAndDaughterUserStepMax)
       {m_detector->setThisAndDaughterUserStepMax(cut);}
     else if(command == m_cmdSetThisAndDaughterUserTrackMax)
       {m_detector->setThisAndDaughterUserTrackMax(cut);}
     else if(command == m_cmdSetThisAndDaughterUserTimeMax)
       {m_detector->setThisAndDaughterUserTimeMax(cut);}
     else if(command == m_cmdSetThisAndDaughterUserEkinMin)
       {m_detector->setThisAndDaughterUserEkinMin(cut);}
     else if(command == m_cmdSetThisAndDaughterUserRangeMin)
       {m_detector->setThisAndDaughterUserRangeMin(cut);}
  } else if(command == m_cmdResetThisUserLimits) {
    m_detector->resetThisUserLimits();
  } else if(command == m_cmdResetThisAndDaughterUserLimits){
    m_detector->resetThisAndDaughterUserLimits();
  } else if(command == m_cmdSetOuterMaterial) {
    m_detector->setOuterMaterial(aValue);
  } else if(command == m_cmdSetOuterColor) {
    m_detector->setOuterColor(aValue);
  } else if(command == m_cmdSetOuterVisibility) {
    m_detector->setOuterVisibility(m_cmdSetOuterVisibility
				   ->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetThisAndDaughterOuterVisibility) {
    m_detector->setThisAndDaughterOuterVisibility(m_cmdSetOuterVisibility
						  ->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetFastSimulationLevel) {
    m_detector->setFastSimulationLevel(m_cmdSetFastSimulationLevel
				       ->GetNewIntValue(aValue));
  } else if(command == m_cmdSetDaughterFastSimulationLevel) {
    m_detector->setDaughterFastSimulationLevel(m_cmdSetFastSimulationLevel
					       ->GetNewIntValue(aValue));
  } else if(command == m_cmdSetSensitiveDetector) {
    m_detector->setSensitiveDetector();
  } else if(command == m_cmdSetSensitiveVolume) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String lvName;
    G4String sdName;
    bool ok=true;
    ok=util->shift(tmpline,lvName);
    if(ok) {
      ok=util->shift(tmpline,sdName);
      if(ok) {
	m_detector->setSensitiveDetector(std::string(lvName.data()),
					 std::string(sdName.data()));
      } else {
	m_detector->setSensitiveDetector(std::string(lvName.data()),"");
      }
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command == m_cmdSetSensitiveDetectorWithName) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String sdName;
    G4String tmpout;
    bool ok=true;
    ok=util->shift(tmpline,sdName);
    if(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
        int ch=std::atoi(tmpout.data());
        m_detector->setSensitiveDetector(std::string(sdName.data()),ch);
      } else {
        m_detector->setSensitiveDetector(std::string(sdName.data()));
      }
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command == m_cmdSetThisAndDaughterSensitiveDetectorWithName) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String sdName;
    G4String tmpout;
    bool ok=true;
    ok=util->shift(tmpline,sdName);
    if(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
        int ch=std::atoi(tmpout.data());
        m_detector->setThisAndDaughterSensitiveDetector(std::string(sdName.data()),ch);
      } else {
        m_detector->setThisAndDaughterSensitiveDetector(std::string(sdName.data()));
      }
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command == m_cmdSetHitsStore) {
    m_detector->setHitsStore(m_cmdSetHitsStore
			     ->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetThisAndDaughterHitsStore) {
    m_detector->setThisAndDaughterHitsStore(m_cmdSetThisAndDaughterHitsStore
					     ->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetDigiStore) {
    m_detector->setDigiStore(m_cmdSetDigiStore
			     ->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetThisAndDaughterDigiStore) {
    m_detector->setThisAndDaughterDigiStore(m_cmdSetThisAndDaughterDigiStore
					     ->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetMtimeStore) {
	m_detector->setMtimeStore(m_cmdSetMtimeStore
						  ->GetNewBoolValue(aValue));
  } else if(command == m_cmdSetThisAndDaughterMtimeStore) {
	m_detector->setThisAndDaughterMtimeStore(m_cmdSetThisAndDaughterMtimeStore
						  ->GetNewBoolValue(aValue));
  } else if(command == m_cmdPrint) {
    m_detector->print();
    m_detector->comment();
  } else if(command == m_cmdUpdate) {
    m_detector->update();
  } else if(command == m_cmdSetThisAsARootRegion) {

    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;

    std::string regionName = "";
    double productionCut = -1;
    if(util->shift(tmpline,tmpout)) {
      regionName = std::string(tmpout.data());
      if(util->shift(tmpline,tmpout)) 
	productionCut = util->interpret(tmpout);
    }
    m_detector->setThisAsARootRegion(regionName,productionCut);
  } else if(command == m_cmdSetParameters) {
    unsigned int nPara=m_detector->getNumberOfParameters();
    GsimUtil* util = GsimUtil::getUtil();
    G4String line = aValue;
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
    if(vec.size()==nPara) {
      m_detector->setParameters(vec);
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command == m_cmdPlaceDetector) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;

    std::string motherFullName;
    GsimDetector* motherDet=0;
    G4ThreeVector tvec(0,0,0);
    G4ThreeVector rvec(0,0,0);
    
    std::vector<double> vec;
    bool ok=true;
    ok=util->shift(tmpline,tmpout);
    if(!ok) {
      std::string msg = "Wrong command format at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
      return;
    }
    motherFullName = std::string(tmpout.data());
    motherDet = m_detector->findDetectorFromWorld(motherFullName);
    if(!motherDet) {
      std::string msg = "No such motherDetector whose fullName is ";
      msg+=motherFullName;
      msg+=". Placement is not performed.";
      GsimMessage::getInstance()
	->report("warning",msg);
      return;
    }
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	vec.push_back(util->interpret(tmpout));
      }
    }
    if(vec.size()==6) {
      tvec=G4ThreeVector(vec[0],vec[1],vec[2]);
      rvec=G4ThreeVector(vec[3],vec[4],vec[5]);
    } else if(vec.size()==3) {
      tvec=G4ThreeVector(vec[0],vec[1],vec[2]);
      rvec=G4ThreeVector(0,0,0);
    } else if(vec.size()==0) {
      tvec=G4ThreeVector(0,0,0);
      rvec=G4ThreeVector(0,0,0);
    } else {
      std::string msg = "Wrong command format at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
      return;
    }
    m_detector->placeDetector(motherDet,tvec,rvec);
  } else if(command == m_cmdCloneDetector) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    
    std::string newName;
    G4ThreeVector tvec(0,0,0);
    G4ThreeVector rvec(0,0,0);
    int channel=-999;
    
    std::vector<double> vec;
    bool ok=true;
    int cnt=0;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	cnt++;
	if(cnt==7) {
	  channel = std::atoi(tmpout.data());
	} else {
	  vec.push_back(util->interpret(tmpout));
	}
      }
    }
    if(vec.size()==6) {
      tvec=G4ThreeVector(vec[0],vec[1],vec[2]);
      rvec=G4ThreeVector(vec[3],vec[4],vec[5]);
    } else if(vec.size()==3) {
      tvec=G4ThreeVector(vec[0],vec[1],vec[2]);
      rvec=G4ThreeVector(0,0,0);
    } else if(vec.size()==0) {
      tvec=G4ThreeVector(0,0,0);
      rvec=G4ThreeVector(0,0,0);
    } else {
      std::string msg = "Wrong command format at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
      return;
    }
    m_detector->cloneDetector(tvec,rvec,channel);
  } else if(command == m_cmdClearThisClone) {
    m_detector->clearThisClone();
  } else if(command == m_cmdSetThisAndDaughterBriefName) {
    m_detector->setThisAndDaughterBriefName(std::string(aValue.data()));
  } else if(command == m_cmdDumpDaughters) {
    m_detector->dumpDaughters();
  } else if(command == m_cmdOpenDXF) {
    GsimDXFVolume::openDXF(aValue);
  } else if(command ==m_cmdCloseDXF) {
    GsimDXFVolume::closeDXF();
  } else if(command ==m_cmdAddDXFLayer) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    bool ok=true;
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	GsimDXFVolume::addDXFLayer(tmpout);
      }
    }
  } else if(command ==m_cmdDumpThisAndDaughterDXF) {
    GsimDXFVolume* vol=m_detector->registerDXFVolume();
    //  , which is newed in GsimDetector.cc
    GsimDXFVolume::setTopVolume(vol);
    vol->dumpDXF();
  } else if(command ==m_cmdSetExportModeDXF) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    bool ok=true;
    ok=util->shift(tmpline,tmpout);
    if(ok) {
      int mode=std::atoi(tmpout.data());
      GsimDXFVolume::setExportMode(mode);
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command ==m_cmdSetCoordinateDXF) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    bool ok=true;
    std::vector<double> vec; 
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	vec.push_back( std::atof(tmpout.data()) );
      }
    }
    if(vec.size()==6) {
      GsimDXFVolume::setXVec(TVector3(vec[0],vec[1],vec[2]));
      GsimDXFVolume::setYVec(TVector3(vec[3],vec[4],vec[5]));
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command ==m_cmdSetCutSurfaceDXF) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    bool ok=true;
    std::vector<double> vec; 
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	vec.push_back(util->interpret(tmpout));
      }
    }
    if(vec.size()==6) {
      GsimDXFVolume::setCutSurfaceNormal(TVector3(vec[0],vec[1],vec[2]));
      GsimDXFVolume::setCutSurfacePoint(TVector3(vec[3],vec[4],vec[5]));
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else if(command ==m_cmdSetScaleDXF) {
    GsimUtil* util = GsimUtil::getUtil();
    G4String tmpline = aValue;
    G4String tmpout;
    bool ok=true;
    std::vector<double> vec; 
    while(ok) {
      ok=util->shift(tmpline,tmpout);
      if(ok) {
	vec.push_back(util->interpret(tmpout));
      }
    }
    if(vec.size()==2) {
      GsimDXFVolume::setScaleXY(vec[0],vec[1]);
    } else {
      std::string msg = "Wrong number of paramaeters at GsimDetectorMessenger::";
      msg+="SetNewValue(G4UIcommand* command,G4String newValue=";
      msg+=std::string(aValue.data()) ;
      msg+=")";
      GsimMessage::getInstance()
	->report("warning",msg);
    }
  } else {
    std::string msg = "Wrong command  at GsimDetectorMessenger::";
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
