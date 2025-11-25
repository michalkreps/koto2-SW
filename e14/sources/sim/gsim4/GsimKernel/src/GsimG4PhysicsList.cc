#include "GsimKernel/GsimG4PhysicsList.h"
#include "GsimKernel/GsimPhysicsList.h"
#include "GsimKernel/GsimPhysicsListFactory.h"

bool GsimG4PhysicsList::s_isConstructed=false;

GsimG4PhysicsList::GsimG4PhysicsList()
{
  ;
}

GsimG4PhysicsList::~GsimG4PhysicsList()
{
  ;
}


void GsimG4PhysicsList::construct()
{

  if(s_isConstructed) return;
  s_isConstructed=true;

  new GsimPhysicsListFactory<GsimOpticalQGSP_BERT>("GsimOpticalQGSP_BERT");
  new GsimPhysicsListFactory<GsimQGSP_BERT>("GsimQGSP_BERT");
  new GsimPhysicsListFactory<GsimXrayFluoPhysicsList>("GsimXrayFluoPhysicsList");
  new GsimPhysicsListFactory<LBE>("LBE");
#if ( G4VERSION_NUMBER/10 < 100 )
  new GsimPhysicsListFactory<LHEP>("LHEP");
  new GsimPhysicsListFactory<LHEP_EMV>("LHEP_EMV");
  new GsimPhysicsListFactory<QGSP>("QGSP");
  new GsimPhysicsListFactory<QGSP_BERT_EMV>("QGSP_BERT_EMV");
#endif
  new GsimPhysicsListFactory<QGSP_BERT>("QGSP_BERT");
  new GsimPhysicsListFactory<QGSP_BERT_HP>("QGSP_BERT_HP");
  new GsimPhysicsListFactory<QGSP_BIC>("QGSP_BIC");
  new GsimPhysicsListFactory<QGSP_BIC_HP>("QGSP_BIC_HP");
  ////
  
#if ( G4VERSION_NUMBER/10 >= 83 )
  new GsimPhysicsListFactory<QBBC>("QBBC");
#endif
  
#if ( G4VERSION_NUMBER/10 >= 90 )
#if ( G4VERSION_NUMBER/10 < 100 )
  new GsimPhysicsListFactory<QGSP_BERT_TRV>("QGSP_BERT_TRV");
  new GsimPhysicsListFactory<QGSP_QEL>("QGSP_QEL");
#endif
#endif
  
#if ( G4VERSION_NUMBER/10 >=91 )
  new GsimPhysicsListFactory<FTFP_BERT>("FTFP_BERT");
  new GsimPhysicsListFactory<FTF_BIC>("FTF_BIC");
#if ( G4VERSION_NUMBER/10 < 100 )
  new GsimPhysicsListFactory<QGSC_BERT>("QGSC_BERT");
#endif
  new GsimPhysicsListFactory<QGS_BIC>("QGS_BIC");
#endif
  
#if ( G4VERSION_NUMBER/10 >=93 )

#if ( G4VERSION_NUMBER/10 < 100 )
  new GsimPhysicsListFactory<FTFP_BERT_EMV>("FTFP_BERT_EMV");
  new GsimPhysicsListFactory<FTFP_BERT_EMX>("FTFP_BERT_EMX");
  new GsimPhysicsListFactory<QGSC_CHIPS>("QGSC_CHIPS");
  new GsimPhysicsListFactory<QGSP_BERT_EMX>("QGSP_BERT_EMX");
  new GsimPhysicsListFactory<QGSP_BERT_NOLEP>("QGSP_BERT_NOLEP");
  new GsimPhysicsListFactory<QGSP_BIC_EMY>("QGSP_BIC_EMY");
#endif

  new GsimPhysicsListFactory<FTFP_BERT_TRV>("FTFP_BERT_TRV");
  new GsimPhysicsListFactory<QGSP_FTFP_BERT>("QGSP_FTFP_BERT");
#if ( G4VERSION_NUMBER/10 < 96 )  
  new GsimPhysicsListFactory<QGSP_INCL_ABLA>("QGSP_INCL_ABLA");
#endif
#endif
  
#if ( G4VERSION_NUMBER/10 >=94 )
#if ( G4VERSION_NUMBER/10 < 100 )
  new GsimPhysicsListFactory<QGSP_BERT_CHIPS>("QGSP_BERT_CHIPS");
#endif
  new GsimPhysicsListFactory<Shielding>("Shielding");
#endif

#if ( G4VERSION_NUMBER/10 >=96 )
  new GsimPhysicsListFactory<FTFP_BERT_HP>("FTFP_BERT_HP");

#if ( G4VERSION_NUMBER/10 < 100 )
  new GsimPhysicsListFactory<QGSP_BERT_95>("QGSP_BERT_95");
  new GsimPhysicsListFactory<QGSP_BERT_95XS>("QGSP_BERT_95XS");
  new GsimPhysicsListFactory<QGSP_FTFP_BERT_95>("QGSP_FTFP_BERT_95");
  new GsimPhysicsListFactory<QGSP_FTFP_BERT_95XS>("QGSP_FTFP_BERT_95XS");
  new GsimPhysicsListFactory<CHIPS_HP>("CHIPS_HP");
#endif
  
  new GsimPhysicsListFactory<QGSP_INCLXX>("QGSP_INCLXX");
#endif
  

#if ( G4VERSION_NUMBER/10 >=95 )
  #if ( G4VERSION_NUMBER/10 < 100 )
  new GsimPhysicsListFactory<CHIPS>("CHIPS");  
  #endif
#endif
  
#if ( G4VERSION_NUMBER/10 >= 102 )
  new GsimPhysicsListFactory<FTFP_BERT_ATL>("FTFP_BERT_ATL");
#endif
#if ( G4VERSION_NUMBER/10 >= 101 )
  new GsimPhysicsListFactory<QGSP_BIC_AllHP>("QGSP_BIC_AllHP");
  new GsimPhysicsListFactory<NuBeam>("NuBeam");
#endif
#if ( G4VERSION_NUMBER/10 >= 100 )
  new GsimPhysicsListFactory<FTFP_INCLXX>("FTFP_INCLXX");
  new GsimPhysicsListFactory<FTFP_INCLXX>("FTFP_INCLXX_HP");
  new GsimPhysicsListFactory<QGSP_INCLXX_HP>("QGSP_INCLXX_HP");
#endif

  return;
}
