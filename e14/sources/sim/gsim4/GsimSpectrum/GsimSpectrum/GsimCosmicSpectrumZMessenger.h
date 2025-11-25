/**
 *  @file
 *  @brief  GsimCosmicSpectrumZMessenger
 *  $Id$
 *  $Log$
 */

#ifndef GsimCosmicSpectrumZMessenger_h
#define GsimCosmicSpectrumZMessenger_h

// includes
#include "G4UImessenger.hh"
#include "globals.hh"

// forward declaration
class GsimCosmicSpectrumZ;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

/**
 *  @class GsimCosmicSpectrumZMessenger.
 *  @brief Messenger for GsimCosmicSpectrumZ.
 *  This class is ...
 */


class GsimCosmicSpectrumZMessenger : public G4UImessenger
{
public:
  /// Constructor.
  /**
   *  @param factory This parent factory class.
   */
  
  GsimCosmicSpectrumZMessenger(GsimCosmicSpectrumZ* spectrum);

  /// Destructor.
  virtual ~GsimCosmicSpectrumZMessenger();

  /// Define action for UI
  void SetNewValue(G4UIcommand*, G4String aValue);
  
protected:

  /// parent factory class.
  GsimCosmicSpectrumZ* m_spectrum;

  /// User Interface directory. 
  G4UIdirectory*          m_spectrumDir;

  /// A user Interface command under the directory.
  G4UIcmdWithAString*      m_cmdAddSpectrum;
  G4UIcmdWithAString*      m_cmdSetCosmicRayDX;
  G4UIcmdWithAString*      m_cmdSetCosmicRayDZ;
  G4UIcmdWithoutParameter* m_cmdClear;
};

#endif //GsimCosmicSpectrumZMessenger_h



