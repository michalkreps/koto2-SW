#ifndef GsimAprimeMessenger_h
#define GsimAprimeMessenger_h 1

// includes
#include "G4UImessenger.hh"
#include "globals.hh"


// forward declaration
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithADouble;


/**
 *  @class GsimAprimeMessenger
 *  @brief Messenger for GsimAprime.
 */
class GsimAprimeMessenger : public G4UImessenger
{
 public:
  GsimAprimeMessenger();
  virtual ~GsimAprimeMessenger();
  
 protected:
  G4UIcmdWithADouble* m_cmdSetAmass;
  void SetNewValue(G4UIcommand*, G4String);
};

#endif 
