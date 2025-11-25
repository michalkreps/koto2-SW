/**
 *  @file GsimG4PhysicsList
 *  @brief Common Geant4 Physics list
 */
#ifndef GsimG4PhysicsList_h
#define GsimG4PhysicsList_h

#include "G4Version.hh"

#include "GsimPhysicsList/GsimOpticalQGSP_BERT.h"
#include "GsimPhysicsList/GsimQGSP_BERT.h"
#include "GsimPhysicsList/GsimXrayFluoPhysicsList.h"

/////////////////////////
#if ( G4VERSION_NUMBER/10 < 100 )
#include "LHEP.hh"
#include "LHEP_EMV.hh"
#include "QGSP.hh"
#include "QGSP_BERT_EMV.hh"
#endif

#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BIC_HP.hh"
/////////////////////////


#if ( G4VERSION_NUMBER/10 >= 83 )
#include "QBBC.hh"
#endif

#if ( G4VERSION_NUMBER/10 >= 90 )

#if ( G4VERSION_NUMBER/10 < 100 )
#include "QGSP_BERT_TRV.hh"
#include "G4HadronQElasticPhysics.hh" // needed by QGSP_QEL
#include "QGSP_QEL.hh"
#endif

#endif

#if ( G4VERSION_NUMBER/10 >= 91 )
#include "FTFP_BERT.hh"
#include "FTF_BIC.hh"

#if ( G4VERSION_NUMBER/10 < 100 )
#include "QGSC_BERT.hh"
#endif

#include "QGS_BIC.hh"
#endif

#if ( G4VERSION_NUMBER/10 >= 93 )

#if ( G4VERSION_NUMBER/10 < 100 )
#include "FTFP_BERT_EMV.hh"
#include "FTFP_BERT_EMX.hh"
#include "QGSC_CHIPS.hh"
#include "QGSP_BERT_EMX.hh"
#include "QGSP_BERT_NOLEP.hh"
#include "QGSP_BIC_EMY.hh"
#endif

#include "FTFP_BERT_TRV.hh"
#include "QGSP_FTFP_BERT.hh"

#if ( G4VERSION_NUMBER/10 < 96 )
//unkown removal
#include "QGSP_INCL_ABLA.hh"
#endif

#endif

#if ( G4VERSION_NUMBER/10 >= 94 )

#if ( G4VERSION_NUMBER/10 < 100 )
#include "QGSP_BERT_CHIPS.hh"
#endif

#include "Shielding.hh"
#endif

#if ( G4VERSION_NUMBER/10 >= 96 )
//written in release note
#include "FTFP_BERT_HP.hh"

#if ( G4VERSION_NUMBER/10 < 100 )
#include "QGSP_BERT_95.hh"
#include "QGSP_BERT_95XS.hh"
#include "QGSP_FTFP_BERT_95.hh"
#include "QGSP_FTFP_BERT_95XS.hh"
#include "CHIPS_HP.hh"
#endif

#include "QGSP_INCLXX.hh"
#endif

#if ( G4VERSION_NUMBER/10 >= 95 )
#if ( G4VERSION_NUMBER/10 < 100 )
#include "CHIPS.hh"
#endif
#endif



#if ( G4VERSION_NUMBER/10 >= 102 )
#include "FTFP_BERT_ATL.hh"
#endif
#if ( G4VERSION_NUMBER/10 >= 101 )
#include "QGSP_BIC_AllHP.hh"
#include "NuBeam.hh"
#endif
#if ( G4VERSION_NUMBER/10 >= 100 )
#include "FTFP_INCLXX.hh"
#include "FTFP_INCLXX_HP.hh"
#include "QGSP_INCLXX_HP.hh"
#endif

/////////////////////////
#include "LBE.hh"
/////////////////////////


/**
 *  @class GsimG4PhysicsList
 *  @brief Interface to prepare common G4 physics list
 *
 *  This class provides an interface to create G4 common physics lists.
 */

class GsimG4PhysicsList
{
 public:
  GsimG4PhysicsList();
  ~GsimG4PhysicsList();
  void construct();
  
 private:
  static bool s_isConstructed;
  
};




#endif //GsimG4PhysicsList_h



