/**
 *  @file
 *  @brief  GsimDetectorMessenger
 *  @author Hajime Nanjo <nanjo@scphys.kyoto-u.ac.jp>
 *  @date   2005.10.5
 *  $Id$
 *  $Log$
 */

#ifndef GsimDetectorMessenger_h
#define GsimDetectorMessenger_h

// includes
#include "G4UImessenger.hh"
#include "globals.hh"
#include <string>

// forward declaration
class GsimDetector;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
/**
 *  @class GsimDetectorMessenger
 *  @brief Messenger for GsimDetector.
 *
 *  This class is owned by GsimDetector and
 *  provide ...
 */


class GsimDetectorMessenger : public G4UImessenger
{
public:
  /// Constructor.
  /**
   *  @param factory This parent factory class.
   */
  GsimDetectorMessenger(GsimDetector* detector);

  /// Destructor.
  virtual ~GsimDetectorMessenger();

  /// Define action for UI
  void SetNewValue(G4UIcommand*, G4String aValue);
  
protected:

  /// parent factory class.
  GsimDetector* m_detector;

  /// User Interface directory. 
  G4UIdirectory*          m_detectorDir;

  /// A user Interface command under the directory.
  G4UIcmdWithAString*        m_cmdSetLocalCenterVector;
  G4UIcmdWithAString*        m_cmdSetTranslationVector;
  G4UIcmdWithAString*        m_cmdSetRotationVector;
  G4UIcmdWithAString*        m_cmdSetThisMagneticField;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterMagneticField;

  G4UIcmdWithAString*        m_cmdSetThisUserStepMax;
  G4UIcmdWithAString*        m_cmdSetThisUserTrackMax;
  G4UIcmdWithAString*        m_cmdSetThisUserTimeMax;
  G4UIcmdWithAString*        m_cmdSetThisUserEkinMin;
  G4UIcmdWithAString*        m_cmdSetThisUserRangeMin;
  G4UIcmdWithoutParameter*   m_cmdResetThisUserLimits;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterUserStepMax;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterUserTrackMax;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterUserTimeMax;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterUserEkinMin;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterUserRangeMin;
  G4UIcmdWithoutParameter*   m_cmdResetThisAndDaughterUserLimits;
  
  G4UIcmdWithAString*        m_cmdSetOuterMaterial;
  G4UIcmdWithAString*        m_cmdSetOuterColor;
  G4UIcmdWithABool*          m_cmdSetOuterVisibility;
  G4UIcmdWithABool*          m_cmdSetThisAndDaughterOuterVisibility;
  G4UIcmdWithAString*        m_cmdSetParameters;
  G4UIcmdWithAString*        m_cmdSetThisAsARootRegion;
  G4UIcmdWithAnInteger*      m_cmdSetFastSimulationLevel;
  G4UIcmdWithAnInteger*      m_cmdSetDaughterFastSimulationLevel;
  G4UIcmdWithAString*        m_cmdSetSensitiveVolume;
  G4UIcmdWithoutParameter*   m_cmdSetSensitiveDetector;
  G4UIcmdWithAString*        m_cmdSetSensitiveDetectorWithName;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterSensitiveDetectorWithName;
  G4UIcmdWithABool*          m_cmdSetHitsStore;
  G4UIcmdWithABool*          m_cmdSetThisAndDaughterHitsStore;
  G4UIcmdWithABool*          m_cmdSetDigiStore;
  G4UIcmdWithABool*          m_cmdSetThisAndDaughterDigiStore;
  G4UIcmdWithABool*          m_cmdSetMtimeStore;
  G4UIcmdWithABool*          m_cmdSetThisAndDaughterMtimeStore;
  G4UIcmdWithAString*        m_cmdPlaceDetector;
  G4UIcmdWithAString*        m_cmdCloneDetector;
  G4UIcmdWithoutParameter*   m_cmdClearThisClone;
  G4UIcmdWithAString*        m_cmdSetThisAndDaughterBriefName;
  G4UIcmdWithoutParameter*   m_cmdPrint;
  G4UIcmdWithoutParameter*   m_cmdUpdate;
  

  G4UIcmdWithAString*        m_cmdSetUserParameters;
  G4UIcmdWithoutParameter*   m_cmdDumpDaughters;

  //DXF
  G4UIcmdWithAString*        m_cmdOpenDXF;
  G4UIcmdWithoutParameter*   m_cmdCloseDXF;
  G4UIcmdWithAString*        m_cmdAddDXFLayer;
  G4UIcmdWithoutParameter*   m_cmdDumpThisAndDaughterDXF;
  G4UIcmdWithAString*        m_cmdSetExportModeDXF;
  G4UIcmdWithAString*        m_cmdSetCoordinateDXF;
  G4UIcmdWithAString*        m_cmdSetCutSurfaceDXF;
  G4UIcmdWithAString*        m_cmdSetScaleDXF;
  
};

#endif //GsimDetectorMessenger_h



