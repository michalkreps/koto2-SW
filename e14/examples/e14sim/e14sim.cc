#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimKernel/GsimRunActionMessenger.h"
#include "GsimKernel/GsimDetectorConstruction.h"
#include "GsimKernel/GsimPhysicsList.h"
#include "GsimKernel/GsimPhysicsListMessenger.h"
#include "GsimKernel/GsimPhysicsListFactory.h"

#include "G4Version.hh"
#include "GsimKernel/GsimG4PhysicsList.h"

#include "GsimKernel/GsimPrimaryGeneratorActionFactory.h"
#include "GsimKernel/GsimDetectorManager.h"
#include "GsimKernel/GsimRunAction.h"
#include "GsimKernel/GsimEventAction.h"
#include "GsimKernel/GsimStackingAction.h"
#include "GsimKernel/GsimTrackingAction.h"
#include "GsimKernel/GsimSteppingAction.h"
#include "GsimKernel/GsimDetectorFactory.h"
#include "GsimKernel/GsimSpectrum.h"
#include "GsimKernel/GsimSpectrumFactory.h"

#include "GsimKernel/GsimMaterialFactory.h"
#include "GsimMaterial/GsimOpticalMaterial.h"

#include "GsimKernel/GsimSurfaceFactory.h"

#include "GsimGun/GsimParticleGun.h"
#include "GsimGun/GsimGeneralParticleSource.h"
#include "GsimGun/GsimHEPEvtInterface.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimTube.h"
#include "GsimDetector/GsimCone.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimDetector/GsimPolycone2.h"
#include "GsimDetector/GsimBoxWithAHole.h"
#include "GsimDetector/GsimBoxWithoutABox.h"
#include "GsimDetector/GsimTubeWithAHole.h"
#include "GsimDetector/GsimPolyhedra2WithASquareHole.h"
#include "GsimDetector/GsimTrap.h"
#include "GsimDetector/GsimTrapWithHoles.h"

#include "GsimSpectrum/GsimBetaDecaySpectrum.h"
#include "GsimSpectrum/GsimCosmicSpectrum.h"
#include "GsimSpectrum/GsimCosmicSpectrumZ.h"

#include "GsimE14add.h"

#include "GsimUIbash/GsimUIbash.h"

//Geant4
#include "G4RunManager.hh"
#include "G4RunManagerKernel.hh"
#include "G4UIdirectory.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4ios.hh"


#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{


  // arguments
  int runConfiguration=0;
  std::string kaonDecay;
  char* rootFileName=0;
  int eventNumber=0;
  int rndSeed=0;
  int runNumberBase=0;
  double beamShiftX=0;
  double beamShiftY=0;


  //e14sim 

  if(argc != 9) {
    G4cout << "Wrong number of arguments." << G4endl;
    G4cout << "e14sim [runConfiguration] [kaonDecay] [rootFileNmae] [eventNmuber] [rnsSeed] [runNumber] [beamShiftX in mm] [beamShiftY in mm]" << G4endl;
    return 1;
  }


  runConfiguration = std::atoi(argv[1]);
  kaonDecay = argv[2];
  rootFileName  = argv[3];
  eventNumber   = std::atoi(argv[4]);
  rndSeed       = std::atoi(argv[5]);
  runNumberBase = std::atoi(argv[6]);
  beamShiftX = std::atof(argv[7]);
  beamShiftY = std::atof(argv[8]);

  if(!
     ( kaonDecay == "KL2gamma" ||
       kaonDecay == "KL3pi0" ||
       kaonDecay == "KLbeta" ||
       kaonDecay == "KLpi02gamma" ||
       kaonDecay == "KLpi0nunu" ||
       kaonDecay == "KLpi0pi0" ||
       kaonDecay == "KLpi0pi0nunu" ||
       kaonDecay == "KLpienu" ||
       kaonDecay == "KLpienugamma" ||
       kaonDecay == "KLpimunu" ||
       kaonDecay == "KLpimunugamma" ||
       kaonDecay == "KLpipi" ||
       kaonDecay == "KLpipipi0"
       ) 
     ) {
    G4cout << "Wrong kaon decay channel" << kaonDecay << G4endl;
    G4cout << "Please asign the follwoing kaon decay channels" << G4endl;
    G4cout << "\t" << "KL2gamma" << G4endl;
    G4cout << "\t" << "KL3pi0" << G4endl;
    G4cout << "\t" << "KLbeta" << G4endl;
    G4cout << "\t" << "KLpi02gamma" << G4endl;
    G4cout << "\t" << "KLpi0nunu"  << G4endl;
    G4cout << "\t" << "KLpi0pi0" << G4endl;
    G4cout << "\t" << "KLpi0pi0nunu"  << G4endl;
    G4cout << "\t" << "KLpienu" << G4endl;
    G4cout << "\t" << "KLpienugamma" << G4endl;
    G4cout << "\t" << "KLpimunu"  << G4endl;
    G4cout << "\t" << "KLpimunugamma" << G4endl;
    G4cout << "\t" << "KLpipi" << G4endl;
    G4cout << "\t" << "KLpipipi0" << G4endl;
    return 1;
  }

     


  for(int i=0;i<argc;i++) {
    G4cout << argv[i] << " ";
  }
  G4cout << G4endl;


  std::stringstream ssRunConfiguration;
  if(runConfiguration==201305) {
#include "201305.icc"
  } else if(runConfiguration==201504) {
#include "201504.icc"
  } else if(runConfiguration==201605) {
#include "201605.icc"
  } else if(runConfiguration==201902) {
#include "201902.icc"
  } else if(runConfiguration==202002) {
#include "202002.icc"
  } else if(runConfiguration==202102) {
#include "202102.icc"
  } else if(runConfiguration==202306) {
#include "202306.icc"
  } else {
    G4cout << "Wrong runConfiguration : " << runConfiguration << G4endl;
    G4cout << "Select 201305, 201504, 201605, 201902 or 202002." << G4endl;
    return 1;
  }

  
  
  
  // run manger
  G4RunManager* runManager = new G4RunManager();
  //runManager->SetVerboseLevel(10);
  
  GsimPersistencyManager* persistencyManager
     = new GsimPersistencyManager(rootFileName);
  
  GsimDetectorManager *dm = new GsimDetectorManager();
  
  // UserInitialization classes - mandatory
  GsimDetectorConstruction* pl = new GsimDetectorConstruction(dm);
  runManager->SetUserInitialization(pl);
  //Some materials and World are created here.
  G4RunManagerKernel::GetRunManagerKernel()
    ->DefineWorldVolume( pl->Construct() ,false);
  
  runManager->SetUserInitialization(new GsimPhysicsList);
  new GsimPhysicsListMessenger();


  GsimG4PhysicsList* g4pl = new GsimG4PhysicsList();
  g4pl->construct();
  

  new GsimPrimaryGeneratorActionFactory<GsimParticleGun>
    ("GsimParticleGun");
  new GsimPrimaryGeneratorActionFactory<GsimGeneralParticleSource>
    ("GsimGeneralParticleSource");
  new GsimPrimaryGeneratorActionFactory<GsimHEPEvtInterface>
    ("GsimHEPEvtInterface");
  
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  GsimRunAction* runAction = new GsimRunAction(dm);
  new GsimRunActionMessenger(runAction);
  runAction->setSeed(rndSeed);
  runAction->setRunNumberBase(runNumberBase);
  
  runManager->SetUserAction(runAction);
  GsimEventAction* eventAction = new GsimEventAction(dm); 
  runManager->SetUserAction(eventAction);
  runManager->SetUserAction(new GsimStackingAction);
  runManager->SetUserAction(new GsimTrackingAction(dm));
  runManager->SetUserAction(new GsimSteppingAction(dm));

  new GsimMaterialFactory<GsimOpticalMaterial>("GsimOpticalMaterial");

  new GsimSurfaceFactory(dm);

  new GsimDetectorFactory<GsimDetector>(dm,"GsimDetector");
  new GsimDetectorFactory<GsimBox>(dm,"GsimBox");
  new GsimDetectorFactory<GsimTube>(dm,"GsimTube");
  new GsimDetectorFactory<GsimCone>(dm,"GsimCone");
  new GsimDetectorFactory<GsimPolyhedra2>(dm,"GsimPolyhedra2");
  new GsimDetectorFactory<GsimPolycone2>(dm,"GsimPolycone2");
  new GsimDetectorFactory<GsimBoxWithoutABox>(dm,"GsimBoxWithoutABox");
  new GsimDetectorFactory<GsimBoxWithAHole>(dm,"GsimBoxWithAHole");
  new GsimDetectorFactory<GsimTubeWithAHole>(dm,"GsimTubeWithAHole");
  new GsimDetectorFactory<GsimPhotoNuclearCounter>(dm,"GsimPhotoNuclearCounter");
#include "GsimE14add.icc"

  new GsimSpectrumFactory<GsimSpectrum>("GsimSpectrum");

  new GsimSpectrumFactory<GsimCosmicSpectrum>("GsimCosmicSpectrum");
  new GsimSpectrumFactory<GsimCosmicSpectrumZ>("GsimCosmicSpectrumZ");
  new GsimSpectrumFactory<GsimBetaDecaySpectrum>("GsimBetaDecaySpectrum");

  
  // initialize G4 kernel
  //runManager->Initialize();

  // get the pointer to the User Interface manager 
  G4UImanager* UI = G4UImanager::GetUIpointer();
  

  
  //persistencyManager->readScript(macroFileName.data());
  std::string ss;
  while(std::getline(ssRunConfiguration,ss,'\n')) {
    G4cout << ss << G4endl;
    if(ss=="") continue;
    UI->ApplyCommand(ss);
    persistencyManager->readCommand(ss.c_str());
  }

  {
    std::ostringstream ostr;
    ostr << "/gps/particle ";
    ostr << kaonDecay;
    G4cout << ostr.str() << G4endl;
    UI->ApplyCommand(ostr.str());
    persistencyManager->readCommand(ostr.str().c_str());
  }

  {
    std::ostringstream ostr;
    ostr << "/gps/position ";
    ostr << beamShiftX << " ";
    ostr << beamShiftY << " ";
    ostr << "-1507  mm";
    G4cout << ostr.str() << G4endl;
    UI->ApplyCommand(ostr.str());
    persistencyManager->readCommand(ostr.str().c_str());
  }

  const char* lastCmd[2] = {
    "/GsimDetectorManager/update",
    "/run/initialize"
  };
  
  for(int i=0;i<2;i++) {
    G4cout << lastCmd[i] << G4endl;
    UI->ApplyCommand(lastCmd[i]);
    persistencyManager->readCommand(lastCmd[i]);
  }
  
    
  if(eventNumber>0) {
    std::ostringstream ostr;
    ostr << "/run/beamOn ";
    ostr << eventNumber;
    G4cout << ostr.str() << G4endl;
    UI->ApplyCommand(ostr.str());
    persistencyManager->readCommand(ostr.str().c_str());
  }
  
  persistencyManager->close();
  delete persistencyManager;

  delete dm;
  // delete
  // (objects which visManager has are automatically deleted)
  
  
#ifdef G4VIS_USE
  delete visManager;
#endif

  //should be the last
  delete runManager;
  
  return 0;
}


