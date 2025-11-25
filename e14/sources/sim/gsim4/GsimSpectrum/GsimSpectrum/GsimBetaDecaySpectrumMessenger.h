/**
 *  @file
 *  @brief  GsimBetaDecaySpectrumMessenger
 *  $Id$
 *  $Log$
 */

#ifndef GsimBetaDecaySpectrumMessenger_h
#define GsimBetaDecaySpectrumMessenger_h

// includes
#include "G4UImessenger.hh"
#include "globals.hh"

// forward declaration
class GsimBetaDecaySpectrum;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

/**
 *  @class GsimBetaDecaySpectrumMessenger.
 *  @brief Messenger for GsimBetaDecaySpectrum.
 *  This class is ...
 */


class GsimBetaDecaySpectrumMessenger : public G4UImessenger
{
public:
  /// Constructor.
  /**
   *  @param factory This parent factory class.
   */
  
  GsimBetaDecaySpectrumMessenger(GsimBetaDecaySpectrum* spectrum);

  /// Destructor.
  virtual ~GsimBetaDecaySpectrumMessenger();

  /// Define action for UI
  void SetNewValue(G4UIcommand*, G4String aValue);
  
protected:

  /// parent factory class.
  GsimBetaDecaySpectrum* m_spectrum;

  /// User Interface directory. 
  G4UIdirectory*          m_spectrumDir;

  /// A user Interface command under the directory.
  G4UIcmdWithAString*      m_cmdAddSpectrum;
  G4UIcmdWithAString*      m_cmdSetBetaDecay;
  G4UIcmdWithAString*      m_cmdSetBetaDecayR;
  G4UIcmdWithAString*      m_cmdSetBetaDecayTheta;
  G4UIcmdWithoutParameter* m_cmdClear;
};

#endif //GsimBetaDecaySpectrumMessenger_h



