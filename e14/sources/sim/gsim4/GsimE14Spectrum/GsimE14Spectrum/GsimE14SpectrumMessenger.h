/**
 *  @file
 *  @brief  GsimE14SpectrumMessenger
 *  $Id$
 *  $Log$
 */

#ifndef GsimE14SpectrumMessenger_h
#define GsimE14SpectrumMessenger_h

// includes
#include "G4UImessenger.hh"
#include "globals.hh"

// forward declaration
class GsimE14Spectrum;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

/**
 *  @class GsimE14SpectrumMessenger.
 *  @brief Messenger for GsimE14Spectrum.
 *  This class is ...
 */


class GsimE14SpectrumMessenger : public G4UImessenger
{
public:
  /// Constructor.
  /**
   *  @param factory This parent factory class.
   */
  
  GsimE14SpectrumMessenger(GsimE14Spectrum* spectrum);

  /// Destructor.
  virtual ~GsimE14SpectrumMessenger();

  /// Define action for UI
  void SetNewValue(G4UIcommand*, G4String aValue);
  
protected:

  /// parent factory class.
  GsimE14Spectrum* m_spectrum;

  /// User Interface directory. 
  G4UIdirectory*          m_spectrumDir;

  /// A user Interface command under the directory.
  G4UIcmdWithAString*      m_cmdAddSpectrum;
  G4UIcmdWithAString*      m_cmdSetDetectorShiftZ;
  G4UIcmdWithAString*      m_cmdSetBeamCondition;
  G4UIcmdWithAString*      m_cmdSetParameter;
  G4UIcmdWithoutParameter* m_cmdClear;
};

#endif //GsimE14SpectrumMessenger_h



