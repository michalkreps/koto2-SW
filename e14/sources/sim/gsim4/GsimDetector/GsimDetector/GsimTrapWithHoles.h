/**
 *  @file
 *  @brief  GsimTrapWithHoles
 *  $Id: $
 *  $Log:$
 */
#ifndef GsimTrapWithHoles_h
#define GsimTrapWithHoles_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"


//forward declaration

/**
 *  @class GsimTrapWithHoles
 *  @brief This class provides a Gsim4 wrapper around G4Trap with a G4Tube subtracted.
 *
 *  This class is usually called by the geometry definition class to
 *  define a new daughter volume (e.g., GsimUserDetector).
 *
 * 
 */
class GsimTrapWithHoles : public GsimDetector
{
 public:
  /**
   *  @function GsimTrapWithHoles
   *  @brief Constructor for GsimTrapWithHoles
   *  @param name The name of the box.
   *  @param motherDetector A pointer to the mother volume.
   *  @param transV A Geant (CLHEP) translation vector.
   *  @param rotV A Geant (CLHEP) rotation vector.
   */
  GsimTrapWithHoles(std::string name,
		    GsimDetector* motherDetector,
		    G4ThreeVector transV,
		    G4ThreeVector rotV,
		    int userFlag=0);
  
  /**
   *  @function ~GsimTrapWithHoles
   *  @brief Destructor for GsimBox.
   */
  virtual ~GsimTrapWithHoles();
  virtual void extractTwoPlanesDXF(std::list<TVector3>& v0List,
				   std::list<TVector3>& v1List,
				   std::list<TVector3>& v2List,
				   std::list<TVector3>& v3List);

 protected:
  /**
   * @function constructPhysiDetector
   * @brief Creates a physical volume.
   */
  virtual void constructPhysiDetector();
  /**
   * @function updateParametersEtc
   * @brief Updates the parameters.
   */
  virtual void updateParametersEtc();
  /**
   *  @function GsimTrapWithHoles
   *  @brief Constructor for GsimTrapWithHoles
   */
  GsimTrapWithHoles();



  G4RotationMatrix* m_rotsub;
};


#endif // GsimTrapWithHoles_h
