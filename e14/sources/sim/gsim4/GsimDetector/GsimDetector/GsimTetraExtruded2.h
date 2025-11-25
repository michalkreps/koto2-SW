/**
 *  @file
 *  @brief  GsimTetraExtruded2
 *  $Id: 
 *  $Log:
 */
#ifndef GsimTetraExtruded2_h
#define GsimTetraExtruded2_h

//includes
#include <string>
#include "GsimKernel/GsimDetector.h"
#include "G4Version.hh"

//forward declaration
class G4ExtrudedSolid;


/**
 *  @class GsimTetraExtruded2
 *  @brief This class provides a Gsim4 wrapper for G4ExtrudedSolid
 *  @see GsimDetector
 *
 *  This class is usually called by the geometry definition class to
 *  define a new daughter volume (e.g., GsimUserDetector).
 *
 *  The default material is G4_PLASTIC_SC_VINYLTOLUENE.
 *  The default color is blue.
 *  The GsimDetector::SetParameters call takes a vector of 6 doubles.
 *  They are (with default values in parenthesis):
 *  - x0 in z=0
 *  - y0 in z=0
 *  - x1 in z=0
 *  - y1 in z=0
 *  - x2 in z=0
 *  - y2 in z=0
 *  - x3 in z=0
 *  - y3 in z=0
 *  - full z length
 *  - center x in -z/2.
 *  - center y in -z/2.
 *  - scale in -z/2.
 *  - center x in +z/2.
 *  - center y in +z/2.
 *  - scale in +z/2.
 *
 *  Unlike when working with Geant4 directly, any lengths should
 *  be full-lengths, not half-lengths.
 */

class GsimTetraExtruded2 : public GsimDetector
{
 public:
  /**
   *  @function GsimTetraExtruded2
   *  @brief Constructor for GsimTetraExtruded2
   *  @param name The name of the polyhedra.
   *  @param motherDetector A pointer to the mother volume.
   *  @param transV A Geant (CLHEP) translation vector.
   *  @param rotV A Geant (CLHEP) rotation vector.
   */
  GsimTetraExtruded2(std::string name,
		     GsimDetector* motherDetector,
		     G4ThreeVector transV,
		     G4ThreeVector rotV,
		     int userFlag=0);
  
  /**
   *  @function ~GsimTetraExtruded2
   *  @brief Destructor for GsimTetraExtruded2.
   */
  virtual ~GsimTetraExtruded2();
  virtual void extractTwoPlanesDXF(std::list<TVector3>& v0List,
				   std::list<TVector3>& v1List,
				   std::list<TVector3>& v2List,
				   std::list<TVector3>& v3List
				   );

 protected:
  /**
   * @function constructPhysiDetector
   * @brief Creates a physical volume.
   */
  virtual void constructPhysiDetector();
  /**
   * @function constructSolidDetector
   * @brief Creates a solid volume.
   */
  virtual G4ExtrudedSolid* constructSolid(std::string fullName);
  /**
   * @function updateParametersEtc
   * @brief Updates the parameters.
   */
  virtual void updateParametersEtc();
  /**
   *  @function isSameDimension
   *  @brief check the dimension of the solid is the same as m_parameterArray
   */
  virtual bool isSameDimension(G4ExtrudedSolid* extSolid) const;
  /**
   *  @function GsimTetraExtruded2
   *  @brief Constructor for GsimTetraExtruded2.
   */
  GsimTetraExtruded2();
  

};


#endif // GsimTetraExtruded2_h
