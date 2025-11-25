/**
 *  $Id: GsimMaterialManager.cc,v 1.12 2006/12/06 18:44:00 nanjo Exp $
 *  $Log: GsimMaterialManager.cc,v $
 *  Revision 1.12  2006/12/06 18:44:00  nanjo
 *  Midas I/O is added and Root I/O is separated. etc.
 *
 */
#include "GsimKernel/GsimMaterialManager.h"
#include "GsimKernel/GsimMaterialManagerMessenger.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"

#include "GsimKernel/GsimUtil.h"
#include "G4Version.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4IonisParamMat.hh"

#include <sstream>

GsimMaterialManager::GsimMaterialManager()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_messenger = new GsimMaterialManagerMessenger(this);

#if ( G4VERSION_NUMBER < 820 )
  // for GsimDetectorConstruction
  useNistMaterial("G4_AIR");
  // for GsimDetector
  useNistMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
#endif
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif    
}

GsimMaterialManager::~GsimMaterialManager()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  delete m_messenger;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}

void GsimMaterialManager::useNistMaterial(std::string matname)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

#if ( G4VERSION_NUMBER >= 820 )
  GsimMessage::getInstance()
    ->report("warning","This is not needed from Geant4.8.2.");
#endif
  
  G4Material* mat=0;
  mat = G4Material::GetMaterial(G4String(matname.c_str()),false);
  if(!mat) {
    mat = G4NistManager::Instance()
      ->FindOrBuildMaterial(G4String(matname.c_str()),true);
  }
  if(!mat) {
    {
      std::ostringstream ostr;
      ostr << "The material, "
	   << matname
	   << " ,can not be used.";
      GsimMessage::getInstance()
	->report("warning",ostr.str());
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}



void GsimMaterialManager::defineMaterials()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  // ==================================================================
  /**
   * elements...
   *   G4NistManager stores many elements and materials.
   *   You can retrieve a G4Element* as
   *         nist->FindOrBuildElement("I");
   *   or a G4Material* as
   *         nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
   *   .
   */
  
  G4NistManager* nist = G4NistManager::Instance();

  
  // ================================================================== 
  /** materials...
   *    You should build materials which G4NistMaanger doesn't store
   *       and materials to which optical properties will be attached.
   */

  G4String name;
  G4double density, pressure,temperature;
  G4int nel,natom;

  // lead antimony (Pb includeing 6%Sb)
  density = 11.0945 *g/cm3;
  G4Material* PbSb
    = new G4Material(name="GsimPbSb",density,nel=2);
  PbSb->AddElement(nist->FindOrBuildElement("Pb"), 94.*perCent);
  PbSb->AddElement(nist->FindOrBuildElement("Sb"), 6.*perCent);

  // lead antimony (Pb includeing 4%Sb)
  // http://www.yoshizawa-ld.co.jp/products/pdf/item1_2.pdf
  density = 11.03 *g/cm3;
  G4Material* PbSb4
    = new G4Material(name="GsimPbSb4",density,nel=2);
  PbSb4->AddElement(nist->FindOrBuildElement("Pb"), 96.*perCent);
  PbSb4->AddElement(nist->FindOrBuildElement("Sb"), 4.*perCent);

  // brass 
  density = 8.61 *g/cm3;
  G4Material* Brass
    = new G4Material(name="GsimBrass",density,nel=4);
  Brass->AddElement(nist->FindOrBuildElement("Cu"), 60.*perCent);
  Brass->AddElement(nist->FindOrBuildElement("Pb"), 2.5*perCent);
  Brass->AddElement(nist->FindOrBuildElement("Fe"), 3.5*perCent);
  Brass->AddElement(nist->FindOrBuildElement("Zn"), 34.*perCent);
    

  // sainless steel
  density = 7.8 *g/cm3;
  G4Material* SUS
    = new G4Material(name="GsimSUS",density,nel=6);
  SUS->AddElement(nist->FindOrBuildElement("Fe"), 68.*perCent);
  SUS->AddElement(nist->FindOrBuildElement("Cr"), 20.*perCent);
  SUS->AddElement(nist->FindOrBuildElement("Ni"),  9.*perCent);
  SUS->AddElement(nist->FindOrBuildElement("Mn"),  1.*perCent);
  SUS->AddElement(nist->FindOrBuildElement("Si"),  1.*perCent);
  SUS->AddElement(nist->FindOrBuildElement("C"),   1.*perCent);

  // sainless steel
  density = 7.93 *g/cm3;
  G4Material* SUS304
    = new G4Material(name="GsimSUS304",density,nel=3);
  SUS304->AddElement(nist->FindOrBuildElement("Fe"), 74*perCent);
  SUS304->AddElement(nist->FindOrBuildElement("Cr"), 18*perCent);
  SUS304->AddElement(nist->FindOrBuildElement("Ni"),  8.*perCent);

  //tungsten alloy
  density = 17.9*g/cm3;
  G4Material* WNiCu
    = new G4Material(name="GsimWNiCu",density,nel=3);
  WNiCu->AddElement(nist->FindOrBuildElement("W"), 94*perCent);
  WNiCu->AddElement(nist->FindOrBuildElement("Ni"), 4*perCent);
  WNiCu->AddElement(nist->FindOrBuildElement("Cu"), 2.*perCent);

  //Low density Al for LECO frame
  // 2kg for 4x4x100cm
  density = 1.25 *g/cm3;
  G4Material* LowDensityAl
    = new G4Material(name="GsimLowDensityAl",density,nel=1);
  LowDensityAl->AddElement(nist->FindOrBuildElement("Al"), 100*perCent);


  //CFRP
  //  Wf=66% (carbon) 
  //  RC=33% (resin) 
  //  Epoxy: C8H8O=120 density:1.85
  //  Carbon: C10 =120 density:1.81
  //-------------------------------
  //    C28H8O
  density = 1.81*g/cm3;
  G4Material* CFRP
    = new G4Material(name="GsimCFRP",density,nel=3);
  CFRP->AddElement(nist->FindOrBuildElement("C"), natom=28);
  CFRP->AddElement(nist->FindOrBuildElement("H"), natom=8);
  CFRP->AddElement(nist->FindOrBuildElement("O"), natom=1);


  //G10 (from G4 testEM3)
  G4Material* G10 = 
    new G4Material("GsimG10", density= 1.700*g/cm3, nel=4);
  G10->AddElement(nist->FindOrBuildElement("Si"), natom=1);
  G10->AddElement(nist->FindOrBuildElement("O") , natom=2);
  G10->AddElement(nist->FindOrBuildElement("C") , natom=3);
  G10->AddElement(nist->FindOrBuildElement("H") , natom=3);


  // aerogel
  // n(SiO2)+2n(H2O)=SiO4H4
  density = 0.11*g/cm3;
  G4Material* Aerogel
    = new G4Material(name="GsimAerogel",density,nel=3);
  Aerogel->AddElement(nist->FindOrBuildElement("Si"), natom=1);
  Aerogel->AddElement(nist->FindOrBuildElement("H"),  natom=4);
  Aerogel->AddElement(nist->FindOrBuildElement("O"),  natom=4);


  // membrane
  // CH2
  density = 1*g/cm3;
  G4Material* Membrane
    = new G4Material(name="GsimMembrane",density,nel=2);
  Membrane->AddElement(nist->FindOrBuildElement("C"), natom=1);
  Membrane->AddElement(nist->FindOrBuildElement("H"), natom=2);

  // Scintillator with density larger by 10%
  density = 1.032*1.1*g/cm3;
  G4Material* GsimSC_Plus
    = new G4Material(name="GsimSC_Plus",density,nel=2);
  GsimSC_Plus->AddElement(nist->FindOrBuildElement("C"), natom=9);
  GsimSC_Plus->AddElement(nist->FindOrBuildElement("H"), natom=10);
  {
    G4IonisParamMat* ion = GsimSC_Plus->GetIonisation();
    ion->SetMeanExcitationEnergy(64.7*CLHEP::eV);
  }
  // Scintillator with density smaller by 10%
  density = 1.032*0.9*g/cm3;
  G4Material* GsimSC_Minus
    = new G4Material(name="GsimSC_Minus",density,nel=2);
  GsimSC_Minus->AddElement(nist->FindOrBuildElement("C"), natom=9);
  GsimSC_Minus->AddElement(nist->FindOrBuildElement("H"), natom=10);
  {
    G4IonisParamMat* ion = GsimSC_Minus->GetIonisation();
    ion->SetMeanExcitationEnergy(64.7*CLHEP::eV);
  }
  
  
  /// Vacuum
  density     = universe_mean_density;    //from PhysicalConstants.h
  pressure    = 3.e-18*pascal;
  temperature = 2.73*kelvin;
  G4Material* vacuum0 = 
    new G4Material(name="GsimVacuum", density, nel=1,
		   kStateGas,temperature,pressure);
  vacuum0->AddElement(nist->FindOrBuildElement("H"), 1);
  vacuum0->GetIonisation()->SetMeanExcitationEnergy(19.2*eV);


  
  pressure    = 0.1*pascal;
  temperature = 293*kelvin;
  density     = 14./(0.082*293)*1e-3*(0.1/1e5)*g/cm3;
  G4Material* vacuumL = 
    new G4Material(name="GsimLowVacuum", density, nel=1,
		   kStateGas,temperature,pressure);
  vacuumL->AddElement(nist->FindOrBuildElement("N"), 1);

  pressure    = 1.e-5*pascal;
  temperature = 293*kelvin;
  density     = 14./(0.082*293)*1e-3*(1.e-5/1e5)*g/cm3;
  G4Material* vacuumH = 
    new G4Material(name="GsimHighVacuum", density, nel=1,
		   kStateGas,temperature,pressure);
  vacuumH->AddElement(nist->FindOrBuildElement("N"), 1);

  //CF4:C5H12=55:45=11:9
  //C11F44+C45H108
  //C56F44H108=C14F11H27
  pressure    = 1.*atmosphere;
  temperature = 293*kelvin;
  density     = 1.177*mg/cm3*(0.55*3.05+0.45*2.49);
  G4Material* CF4nPentane = 
    new G4Material(name="GsimCF4nPentane", density, nel=3,
		   kStateGas,temperature,pressure);
  CF4nPentane->AddElement(nist->FindOrBuildElement("C"), 14);
  CF4nPentane->AddElement(nist->FindOrBuildElement("F"), 11);
  CF4nPentane->AddElement(nist->FindOrBuildElement("H"), 27);



  


  //Optical Material for KOTO

  G4Material* mat=0;
  GsimUtil* util=GsimUtil::getUtil();
    
  // Air
  name="GsimOpticalAir";
  mat=util->findMaterial(name,false);
  G4Material* air =0;
  if(!mat) {
    density=  0.00120479*g/cm3;
    air =
      new G4Material(name, density,nel=4, kStateGas);
    air->AddElement( nist->FindOrBuildElement("C"), 0.000124);
    air->AddElement( nist->FindOrBuildElement("N"), 0.755267);
    air->AddElement( nist->FindOrBuildElement("O"), 0.231781);
    air->AddElement( nist->FindOrBuildElement("Ar"), 0.012827);
    air->GetIonisation()->SetMeanExcitationEnergy(85.7*eV);
  }

  //Aerogel
  name="GsimOpticalAerogel";
  mat=util->findMaterial(name,false);
  G4Material* opAerogel=0;
  if(!mat) {
    // aerogel
    // n(SiO2)+2n(H2O)=SiO4H4
    density = 0.11*g/cm3;
    opAerogel
      = new G4Material(name,density,nel=3);
    opAerogel->AddElement(nist->FindOrBuildElement("Si"), natom=1);
    opAerogel->AddElement(nist->FindOrBuildElement("H"),  natom=4);
    opAerogel->AddElement(nist->FindOrBuildElement("O"),  natom=4);
  }

  //Aerogel101 n=1.01
  name="GsimOpticalAerogel101";
  mat=util->findMaterial(name,false);
  G4Material* opAerogel101=0;
  if(!mat) {
    // aerogel
    // n(SiO2)+2n(H2O)=SiO4H4
    density = 0.11*g/cm3;
    opAerogel101
      = new G4Material(name,density,nel=3);
    opAerogel101->AddElement(nist->FindOrBuildElement("Si"), natom=1);
    opAerogel101->AddElement(nist->FindOrBuildElement("H"),  natom=4);
    opAerogel101->AddElement(nist->FindOrBuildElement("O"),  natom=4);
  }

  //Aerogel103 n=1.03
  name="GsimOpticalAerogel103";
  mat=util->findMaterial(name,false);
  G4Material* opAerogel103=0;
  if(!mat) {
    // aerogel
    // n(SiO2)+2n(H2O)=SiO4H4
    density = 0.11*g/cm3;
    opAerogel103
      = new G4Material(name,density,nel=3);
    opAerogel103->AddElement(nist->FindOrBuildElement("Si"), natom=1);
    opAerogel103->AddElement(nist->FindOrBuildElement("H"),  natom=4);
    opAerogel103->AddElement(nist->FindOrBuildElement("O"),  natom=4);
  }

  // PMMA
  name="GsimOpticalPMMA";
  mat=util->findMaterial(name,false);
  G4Material* opPMMA =0;
  if(!mat) {
    density     = 1.19*g/cm3;
    opPMMA =
      new G4Material(name, density, nel=3);
    opPMMA->AddElement(nist->FindOrBuildElement("C"), 5);
    opPMMA->AddElement(nist->FindOrBuildElement("O"), 1);
    opPMMA->AddElement(nist->FindOrBuildElement("H"), 8);
  }
  
  


  if(air) {
    /// optical properties of Air
    /**
     *   one representative value for refractive index
     */
    const G4int nEntries=17;
    G4double photonWaveLength[nEntries] = {
      300.0, 325.0, 350.0, 375.0,
      400.0, 425.0, 450.0, 475.0,
      500.0, 525.0, 550.0, 575.0,
      600.0, 625.0, 650.0, 675.0,
      700.0
    };//[nm]
    G4double photonEnergy[nEntries];
    for(int i=0;i<nEntries;i++) {
      photonEnergy[i]=2.*M_PI*197.326/photonWaveLength[i]*eV;
    }
    G4double refractiveIndex[nEntries] = {
      1.000286349, 1.000283315, 1.000280985, 1.000279151,
      1.000277679, 1.000276478, 1.000275484, 1.000274653,
      1.000273949, 1.000273348, 1.000272831, 1.000272382,
      1.000271990, 1.000271646, 1.000271341, 1.000271071,
      1.000270830
    };
    //20 Celsius degree, 30% humidity, 101.325kPa, 450ppm CO2
    //uncertainty in calculation : 3e-8
    //based on Modifed Edlen Equation, Engineering Metrology Toolbox
    //http://emtoolbox.nist.gov/Wavelength/Edlen.asp
    
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX",       photonEnergy, refractiveIndex,nEntries);
    air->SetMaterialPropertiesTable(mpt);
  }

  if(opAerogel) {
    /// optical properties of Aerogel
    /**
     *   one representative value for refractive index
     */
    const G4int nEntries=2;
    G4double photonWaveLength[nEntries] = {
      300.0,
      700.0
    };//[nm]
    G4double photonEnergy[nEntries];
    for(int i=0;i<nEntries;i++) {
      photonEnergy[i]=2.*M_PI*197.326/photonWaveLength[i]*eV;
    }
    G4double refractiveIndex[nEntries];
    for(int i=0;i<nEntries;i++) {
      refractiveIndex[i]=1.05;
      //refractiveIndex[i]=1.046;
    }
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX",       photonEnergy, refractiveIndex,nEntries);
    opAerogel->SetMaterialPropertiesTable(mpt);
  }


  if(opAerogel101) {
    /// optical properties of Aerogel
    /**
     *   one representative value for refractive index
     */
    const G4int nEntries=2;
    G4double photonWaveLength[nEntries] = {
      300.0,
      700.0
    };//[nm]
    G4double photonEnergy[nEntries];
    for(int i=0;i<nEntries;i++) {
      photonEnergy[i]=2.*M_PI*197.326/photonWaveLength[i]*eV;
    }
    G4double refractiveIndex[nEntries];
    for(int i=0;i<nEntries;i++) {
      refractiveIndex[i]=1.01;
    }
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX",       photonEnergy, refractiveIndex,nEntries);
    opAerogel101->SetMaterialPropertiesTable(mpt);
  }

  if(opAerogel103) {
    /// optical properties of Aerogel
    /**
     *   one representative value for refractive index
     */
    const G4int nEntries=2;
    G4double photonWaveLength[nEntries] = {
      300.0,
      700.0
    };//[nm]
    G4double photonEnergy[nEntries];
    for(int i=0;i<nEntries;i++) {
      photonEnergy[i]=2.*M_PI*197.326/photonWaveLength[i]*eV;
    }
    G4double refractiveIndex[nEntries];
    for(int i=0;i<nEntries;i++) {
      refractiveIndex[i]=1.03;
    }
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX",       photonEnergy, refractiveIndex,nEntries);
    opAerogel103->SetMaterialPropertiesTable(mpt);
  }

  if(opPMMA) {
    
    const G4int nEntries=16;
    G4double photonWaveLength[nEntries] = {
      260.,
      280.,
      300.,
      320.,
      340.,
      360.,
      380.,
      400.,
      450.,
      500.,
      550.,
      600.,
      700.,
      800.,
      1000.,
      1200.
    };//[nm]                                                                                                                                                                         
    G4double photonEnergy[nEntries];
    for(int i=0;i<nEntries;i++) {
      photonEnergy[i]=2.*M_PI*197.326/photonWaveLength[i]*eV;
    }

    double a=1.491;
    double b=3.427e-3;
    double c=1.819e-4;
    
    G4double refractiveIndex[nEntries];
    for(int i=0;i<nEntries;i++) {
      double x=photonWaveLength[i]*1e-3;
      double nn=
	a+
	b*std::pow(x,-2.)+
	c*std::pow(x,-4.);
      refractiveIndex[i]=nn;
    }
    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    mpt->AddProperty("RINDEX",       photonEnergy, refractiveIndex,nEntries);
    opPMMA->SetMaterialPropertiesTable(mpt);
  }
  

  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  

}


void GsimMaterialManager::mixElement(std::string name,
				     double density,
				     double pressure,
				     double temperature,
				     std::vector<std::string> elementVec,
				     std::vector<double> weightVec)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4NistManager* nist = G4NistManager::Instance();
  
  int nEle=elementVec.size();
  if(nEle!=int(weightVec.size())) {
    std::ostringstream ostr;
    ostr << "The material, "
	 << name
	 << " ,is not created due to invalid number.";
    GsimMessage::getInstance()
      ->report("warning",ostr.str());
    return;
  }

  double totW=0;
  for(std::vector<double>::iterator itW=weightVec.begin();
      itW!=weightVec.end();itW++) {
    totW+=(*itW);
  }
  if(std::abs(totW-1.)>0.001) {
    std::ostringstream ostr;
    ostr << "The material, "
	 << name
	 << " ,is not created due to total fraction error.";
    GsimMessage::getInstance()
      ->report("warning",ostr.str());
    return; 
  }

  for(std::vector<std::string>::iterator itN=elementVec.begin();
      itN!=elementVec.end();itN++) {
    G4Element* elm=nist->FindOrBuildElement((*itN).c_str());
    if(!elm) {
      std::ostringstream ostr;
      ostr << "The material, "
	   << name
	   << " ,is not created due to unknown element,"
	   << (*itN)
	   << ".";
      GsimMessage::getInstance()
	->report("warning",ostr.str());
      return; 
    }
  }
  
  
  G4String matName=name.c_str();
  G4Material* mat
    = new G4Material(matName,density,nEle,
		     kStateUndefined,temperature,pressure);

  std::vector<std::string>::iterator itN=elementVec.begin();
  std::vector<double>::iterator      itW=weightVec.begin();
  
  for(;itN!=elementVec.end();itN++,itW++) {
    mat->AddElement(nist->FindOrBuildElement((*itN).c_str()), (*itW));
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}

void GsimMaterialManager::mixElement(std::string name,
				     double density,
				     double pressure,
				     double temperature,
				     std::vector<std::string> elementVec,
				     std::vector<int> numVec)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4NistManager* nist = G4NistManager::Instance();
  
  int nEle=elementVec.size();
  if(nEle!=int(numVec.size())) {
    std::ostringstream ostr;
    ostr << "The material, "
	 << name
	 << " ,is not created due to invalid number.";
    GsimMessage::getInstance()
      ->report("warning",ostr.str());
    return;
  }

  for(std::vector<std::string>::iterator itN=elementVec.begin();
      itN!=elementVec.end();itN++) {
    G4Element* elm=nist->FindOrBuildElement((*itN).c_str());
    if(!elm) {
      std::ostringstream ostr;
      ostr << "The material, "
	   << name
	   << " ,is not created due to unknown element,"
	   << (*itN)
	   << ".";
      GsimMessage::getInstance()
	->report("warning",ostr.str());
      return; 
    }
  }
  
  
  G4String matName=name.c_str();
  G4Material* mat
    = new G4Material(matName,density,nEle,
		     kStateUndefined,temperature,pressure);

  std::vector<std::string>::iterator itN=elementVec.begin();
  std::vector<int>::iterator         itW=numVec.begin();
  
  for(;itN!=elementVec.end();itN++,itW++) {
    mat->AddElement(nist->FindOrBuildElement((*itN).c_str()), (*itW));
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}


void GsimMaterialManager::setProperty(std::string matName,std::string type,
				      std::vector<double> photonWaveLengthVector,
				      std::vector<double> propertyVector)
{
  G4Material* mat=0;
  mat = G4Material::GetMaterial(G4String(matName.c_str()),false);
  if(!mat) {
    std::ostringstream ostr;
    ostr << "No such material : " << matName << ".";
    GsimMessage::getInstance()
      ->report("warning",ostr.str());
    return;
  }

  G4MaterialPropertiesTable* MPT=mat->GetMaterialPropertiesTable();
  if(!MPT) {
    MPT = new G4MaterialPropertiesTable();
    mat->SetMaterialPropertiesTable(MPT);
  }
  

  if(photonWaveLengthVector.size() !=propertyVector.size()) {
    std::ostringstream ostr;
    ostr << "Vector sizes are different. ";
    GsimMessage::getInstance()
      ->report("warning",ostr.str());
    return;
  }

  int num=photonWaveLengthVector.size();
  if(num>0) {
    double* pe = new double[num];
    double* re = new double[num];
    for(int i=0;i<num;i++) {
      pe[i] = 2.*M_PI*197.326/photonWaveLengthVector[i]*eV;
      re[i] = propertyVector[i];
    }
    MPT->AddProperty(type.c_str(),pe,re,num);
    delete [] pe;
    delete [] re;
  } else {
    MPT->RemoveProperty(type.c_str());
  }
}



void GsimMaterialManager::setConstProperty(std::string matName,std::string type,
					   double value)
{
  G4Material* mat=0;
  mat = G4Material::GetMaterial(G4String(matName.c_str()),false);
  if(!mat) {
    std::ostringstream ostr;
    ostr << "No such material : " << matName << ".";
    GsimMessage::getInstance()
      ->report("warning",ostr.str());
    return;
  }

  G4MaterialPropertiesTable* MPT=mat->GetMaterialPropertiesTable();
  if(!MPT) {
    MPT = new G4MaterialPropertiesTable();
    mat->SetMaterialPropertiesTable(MPT);
  }
  
  if(value!=-999) {
    MPT->AddConstProperty(type.c_str(),value);
  } else {
    MPT->RemoveConstProperty(type.c_str());
  }
}


void GsimMaterialManager::dumpProperty(std::string matName)
{
  G4Material* mat=0;
  mat = G4Material::GetMaterial(G4String(matName.c_str()),false);
  if(!mat) {
    std::ostringstream ostr;
    ostr << "No such material : " << matName << ".";
    GsimMessage::getInstance()
      ->report("warning",ostr.str());
    return;
  }

  G4MaterialPropertiesTable* MPT=mat->GetMaterialPropertiesTable();
  if(MPT) MPT->DumpTable();
}

void GsimMaterialManager::listMaterial()
{
  G4cout << "--- Materials ---" << G4endl;
  const G4MaterialTable* tab=G4Material::GetMaterialTable();
  for(G4MaterialTable::const_iterator it=tab->begin();
      it!=tab->end();it++) {
    G4cout << (*it)->GetName() << G4endl;
  }
  G4cout << "--- -------------" << G4endl;
}
