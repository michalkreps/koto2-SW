#ifndef GsimKLMessenger_h
#define GsimKLMessenger_h 1

// includes
#include "G4UImessenger.hh"
#include "globals.hh"


// forward declaration
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAnInteger;


/**
 *  @class GsimKLMessenger
 *  @brief Messenger for GsimX.
 */
class GsimKLMessenger : public G4UImessenger
{
 public:
  GsimKLMessenger();
  virtual ~GsimKLMessenger();
  
 protected:
  G4UIcmdWithAnInteger* m_cmdSwitchKLDecayTable;
  void SetNewValue(G4UIcommand*, G4String);
};

#endif 
