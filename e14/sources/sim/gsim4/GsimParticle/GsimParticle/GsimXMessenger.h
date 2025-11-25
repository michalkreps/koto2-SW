#ifndef GsimXMessenger_h
#define GsimXMessenger_h 1

// includes
#include "G4UImessenger.hh"
#include "globals.hh"


// forward declaration
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;


/**
 *  @class GsimXMessenger
 *  @brief Messenger for GsimX.
 */
class GsimXMessenger : public G4UImessenger
{
 public:
  GsimXMessenger();
  virtual ~GsimXMessenger();
  
 protected:
  G4UIcmdWithAString* m_cmdAddXParticle;
  void SetNewValue(G4UIcommand*, G4String);
};

#endif 
