/**
 *  $Id:
 *  $Log: 
 */
#include "GsimPersistency/GsimPersistencyManager.h"
#include "GsimPersistency/GsimMessage.h"
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

#include "GsimPhysicsList/GsimLowEnergyEMOp.h"

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


#include "GsimE391Detector/GsimE391.h"
#include "GsimE391Detector/GsimE391BCV.h"
#include "GsimE391Detector/GsimE391CC07.h"
#include "GsimE391Detector/GsimE391CC06.h"
#include "GsimE391Detector/GsimE391BHCV.h"
#include "GsimE391Detector/GsimE391CC03.h"
#include "GsimE391Detector/GsimE391BA.h"
#include "GsimE391Detector/GsimE391CC00.h"
#include "GsimE391Detector/GsimE391CV.h"
#include "GsimE391Detector/GsimE391SAND.h"
#include "GsimE391Detector/GsimE391CBAR.h"
#include "GsimE391Detector/GsimE391FBAR.h"
#include "GsimE391Detector/GsimE391CSI.h"
#include "GsimE391Detector/GsimE391CC02.h"
#include "GsimE391Detector/GsimE391CC04.h"
#include "GsimE391Detector/GsimE391CC05.h"

#include "GsimE391Detector/GsimE391Beamline.h"


#include "GsimE391Spectrum/GsimE391Spectrum.h"
#include "GsimE391Spectrum/GsimE391HalonSpectrum.h"
#include "GsimE391Spectrum/GsimE391CorenSpectrum.h"

#include "GsimE14add.h"

#include "GsimUIbash/GsimUIbash.h"
#include "GsimKernel/GsimUtil.h"

//Geant4
#include "G4RunManager.hh"
#include "G4RunManagerKernel.hh"
#include "G4UIdirectory.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4ios.hh"


#include "G4Navigator.hh"
#include "G4TouchableHandle.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"


#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int argc, char** argv)
{

  std::ofstream fout("/dev/null");
  G4cout.rdbuf(fout.rdbuf()); // redirect 'G4cout' to a 'fout'
  G4cerr.rdbuf(fout.rdbuf()); // redirect 'G4cout' to a 'fout'


  GsimMessage* msg = GsimMessage::getInstance();
  msg->setVerboseLevel(0);
  

  // arguments
  G4String macroFileName="";
  G4String positionFileName="";
  char* rootFileName=0;
  int rndSeed=0;
  int runNumberBase=0;

  double xPos=0;
  double yPos=0;
  double zPos=0;
  bool isFileInput=false;
  if(argc ==5) {
    isFileInput=false;
    macroFileName = argv[1];
    xPos  = std::atof(argv[2]);
    yPos  = std::atof(argv[3]);
    zPos  = std::atof(argv[4]);
  } else if(argc==3) {
    isFileInput=true;
    macroFileName = argv[1];
    positionFileName = argv[2];
  } else {
    std::cout << "Wrong number of arguments : "
	      <<  argc
	      << std::endl;
    std::cout << "gsim4VolumeFinder macroFile X Y Z (in mm)" << std::endl;
    std::cout << "gsim4VolumeFinder macroFile positionFile" << std::endl;
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

  G4VPhysicalVolume* worldPhys=pl->Construct();
  G4RunManagerKernel::GetRunManagerKernel()
    ->DefineWorldVolume( worldPhys ,false);
  
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

  new GsimDetectorFactory<GsimE391>(dm,"GsimE391");
  
  new GsimDetectorFactory<GsimE391BCV>(dm,"GsimE391BCV");
  new GsimDetectorFactory<GsimE391CC07>(dm,"GsimE391CC07");
  new GsimDetectorFactory<GsimE391CC06>(dm,"GsimE391CC06");
  new GsimDetectorFactory<GsimE391BHCV>(dm,"GsimE391BHCV");
  new GsimDetectorFactory<GsimE391CC03>(dm,"GsimE391CC03");
  new GsimDetectorFactory<GsimE391BA>(dm,"GsimE391BA");
  new GsimDetectorFactory<GsimE391CC00>(dm,"GsimE391CC00");
  new GsimDetectorFactory<GsimE391CV>(dm,"GsimE391CV");
  new GsimDetectorFactory<GsimE391SAND>(dm,"GsimE391SAND");
  new GsimDetectorFactory<GsimE391CBAR>(dm,"GsimE391CBAR");
  new GsimDetectorFactory<GsimE391FBAR>(dm,"GsimE391FBAR");
  new GsimDetectorFactory<GsimE391CSI>(dm,"GsimE391CSI");
  new GsimDetectorFactory<GsimE391CC02>(dm,"GsimE391CC02");
  new GsimDetectorFactory<GsimE391CC04>(dm,"GsimE391CC04");
  new GsimDetectorFactory<GsimE391CC05>(dm,"GsimE391CC05");

  new GsimDetectorFactory<GsimE391Beamline>(dm,"GsimE391Beamline");
  
  new GsimSpectrumFactory<GsimSpectrum>("GsimSpectrum");
  new GsimDetectorFactory<GsimPhotoNuclearCounter>(dm,"GsimPhotoNuclearCounter");
#include "GsimE14add.icc"
  
  G4UImanager* UI = G4UImanager::GetUIpointer();
  {
    // batch mode
    G4String command = "/control/execute ";
    UI->ApplyCommand(command+macroFileName);
    // std::ostringstream ostr;
    //   ostr << "/run/beamOn 1";
    //   UI->ApplyCommand(ostr.str());
    // 
  }

  GsimUtil* util = GsimUtil::getUtil();

  
  G4Navigator* aNavigator = new G4Navigator();
  aNavigator->SetWorldVolume(worldPhys);
  //G4TransportationManager::GetTransportationManager()->
  //GetNavigatorForTracking()->GetWorldVolume());

  if(!isFileInput){
    G4ThreeVector vec(xPos,yPos,zPos);
    std::string name = util->getVolumeName(vec);
    
    // aNavigator->LocateGlobalPointAndSetup(vec);
    // G4TouchableHistoryHandle aTouchable =
    //   aNavigator->CreateTouchableHistoryHandle();
    // G4VPhysicalVolume* physvol = aTouchable->GetVolume();
    // std::string name = physvol->GetName();
    std::cout << vec.x() << "\t" 
	      << vec.y() << "\t" 
	      << vec.z() << "\t" 
	      << name << std::endl;
  } else {
    std::ifstream fi(positionFileName);
    while(fi >> xPos >> yPos >> zPos) {
      G4ThreeVector vec(xPos,yPos,zPos);
      std::string name = util->getVolumeName(vec);
      std::cout << vec.x() << "\t" 
		<< vec.y() << "\t" 
		<< vec.z() << "\t" 
		<< name << std::endl;
    }
    fi.close();
  }

    
  persistencyManager->close();
  delete persistencyManager;

  delete dm;
  //should be the last
  delete runManager;

  return 0;
}


