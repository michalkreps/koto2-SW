/**
 *  @file
 *  @brief  GsimPhysicsListMessenger
 *  $Id: $ 
 *  $Log: $
 */

#ifndef GsimPhysicsListMessenger_h
#define GsimPhysicsListMessenger_h

// includes
#include "G4UImessenger.hh"
#include "globals.hh"

// forward declaration
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4ParticleDefinition;
class GsimXMessenger;
class GsimAprimeMessenger;
class GsimKLMessenger;

/**
 *  @class GsimPhysicsListMessenger.
 *  @brief Messenger for GsimPhysicsList.
 *  This class is ...
 */


class GsimPhysicsListMessenger : public G4UImessenger
{
public:
  /// Constructor.
  /**
   *  @param factory This parent factory class.
   */
  
  GsimPhysicsListMessenger();

  /// Destructor.
  virtual ~GsimPhysicsListMessenger();

  /// Define action for UI
  void SetNewValue(G4UIcommand*, G4String aValue);
  
protected:

  /// User Interface directory. 
  G4UIdirectory*          m_physicsListDir;

  /// set KL->pi0nunu decay model
  G4UIcmdWithAnInteger*   m_cmdSetKLPi0NuNuDecayModel;
  /// set KL->pi0nunu dalitz parameter, lambda+
  G4UIcmdWithADouble*     m_cmdSetKLPi0NuNuLambdaPlus;

  /// add Phasespace Decay
  G4UIcmdWithAString* m_cmdAddPhaseSpaceDecay;

  
  G4UIcmdWithABool*            m_cmdWithUserLimits;
  G4UIcmdWithoutParameter*     m_cmdAddCerenkov;


  /// add Phasespace Decay
  G4UIcmdWithAString* m_cmdSetDefaultCut;

  /// add Xparticle
  GsimXMessenger* m_XMessenger;

  /// add Aprime
  GsimAprimeMessenger* m_AprimeMessenger;

  /// interface to switch kaon0L decay table
  GsimKLMessenger* m_KLMessenger;

  G4UIcmdWithAString* m_cmdSelectParticle;
  G4UIcmdWithAString* m_cmdSetBR;
  G4UIcmdWithAString* m_cmdDumpParticle;
  
 private:
  G4ParticleDefinition* m_currentParticle;
};

#endif //GsimPhysicsListMessenger_h



