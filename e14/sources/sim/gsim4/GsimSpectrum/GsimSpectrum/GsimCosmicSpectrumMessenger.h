/**
 *  @file
 *  @brief  GsimCosmicSpectrumMessenger
 *  $Id$
 *  $Log$
 */

#ifndef GsimCosmicSpectrumMessenger_h
#define GsimCosmicSpectrumMessenger_h

// includes
#include "G4UImessenger.hh"
#include "globals.hh"

// forward declaration
class GsimCosmicSpectrum;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

/**
 *  @class GsimCosmicSpectrumMessenger.
 *  @brief Messenger for GsimCosmicSpectrum.
 *  This class is ...
 */


class GsimCosmicSpectrumMessenger : public G4UImessenger
{
public:
  /// Constructor.
  /**
   *  @param factory This parent factory class.
   */
  
  GsimCosmicSpectrumMessenger(GsimCosmicSpectrum* spectrum);

  /// Destructor.
  virtual ~GsimCosmicSpectrumMessenger();

  /// Define action for UI
  void SetNewValue(G4UIcommand*, G4String aValue);
  
protected:

  /// parent factory class.
  GsimCosmicSpectrum* m_spectrum;

  /// User Interface directory. 
  G4UIdirectory*          m_spectrumDir;

  /// A user Interface command under the directory.
  G4UIcmdWithAString*      m_cmdAddSpectrum;
  G4UIcmdWithAString*      m_cmdSetCosmicRayDX;
  G4UIcmdWithAString*      m_cmdSetCosmicRayDZ;
  G4UIcmdWithoutParameter* m_cmdClear;
};

#endif //GsimCosmicSpectrumMessenger_h



