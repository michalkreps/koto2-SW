/**
 *  @file
 *  @brief  GsimE14BHGC
 *  $Id: $ 
 *  $Log: $
 */
// 20160328 bug fixed by S.Shinohara (l.340 and l.485~).

#include "GsimE14Detector/GsimE14BHGC.h"
#include "GsimDetector/GsimBox.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimSensitiveDetectorMessenger.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimKernel/GsimUtil.h"

#include "CLHEP/Random/Randomize.h"

#include "G4Step.hh"
#include "G4OpticalPhoton.hh"
#include "G4ThreeVector.hh"
#include "G4VTouchable.hh"
#include "G4NavigationHistory.hh"
#include "G4SystemOfUnits.hh"

GsimE14BHGC::GsimE14BHGC(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14BHGC";
  m_userFlag= userFlag;

  m_opticalMode=true;
  if(m_opticalMode) {
    GsimUtil::getUtil()->addCerenkov();
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  /*
    from upstream view

    front                                       rear

   North                    South                North              South
         ch1            ch3                   Top
        -----          -----                    ------------------------
       |     |        |     |              ch6 |   mod2                 | ch7
       |     |        |     |                  |                        |
       |     |        |     |                   ------------------------
       | mod0|        | mod1|          
       |     |        |     |
       |     |        |     |
       |     |        |     |                 Bottom
       |     |        |     |                   ------------------------
       |     |        |     |              ch4 |   mod3                 | ch5
       |     |        |     |                  |                        |
        -----          -----                    ------------------------
         ch0            ch2

        <----><------>                         <------------------------>
        120mm  260mm                                     500mm


  */
  ///////////////////////////////////////////////////////////////////////////////////////

  if(userFlag==1 || userFlag==2) {
    m_nModule=1;
  } else {
    m_nModule=4;
  }
  m_factor = new double [m_nModule];
  m_reflectance = new double [m_nModule];
  for(int i=0;i<m_nModule;i++) {
    m_factor[i]=1.;
    m_reflectance[i]=0.9999;
  }
  m_isInBeamPosition=false;
  
  
  m_width=500*mm;
  m_height=120*mm;
  m_thick =10*mm;
  double PbThick=9.6*mm;
  
  double posXY=190*mm;
  double posZ=0*mm;

  double posZ_Acrylic[2];
  posZ_Acrylic[0]=40*mm+94*mm+2*mm;
  posZ_Acrylic[1]=40*mm+183*mm+180*mm+94*mm;
  if(userFlag==1 || userFlag==2) {
    posZ_Acrylic[0]=0;
  }

  
  double poslead[2];
  //poslead[0]=40*mm+82.5*mm-PbThick/2.;
  //poslead[1]=40*mm+183*mm+180*mm+82.5*mm-PbThick/2.;
  poslead[0]=posZ_Acrylic[0]-11.5*mm-PbThick/2.;
  poslead[1]=posZ_Acrylic[1]-11.5*mm-PbThick/2.;


  double lecox=40*mm;
  double lecoy=40*mm;
  double lecoz=1500*mm;

  double posLECO[8] = 
    { 
       1,  1,
       1, -1,
      -1,  1,
      -1, -1
    };
  
  //sign of X,Y  
  double posMod[8]=
    {
       1,  0,
      -1,  0,
       0, -1,
       0, +1
    };
  
  
  // angle x 90deg rotation in X,Z
  double angle[8]=
    { 
      0, -1,
      0, -1,
      0, 0,
      0, 0
    };
  
  
  char nam[256] = "";
  
  if(userFlag==1 || userFlag==2 ||
     userFlag==201503 ||
     userFlag==201504 ||
     userFlag==201605 || 
     userFlag==201902 ||
     userFlag==202002 ||
     userFlag==202102 ||
     userFlag==202306
     ) {
    
    if(userFlag==1 || userFlag==2) {
      //do nothging and m_factor is one
    } else if(userFlag==201503 || userFlag==201504) {
      m_factor[0]=0.810295201;
      m_factor[1]=0.691637414;
      m_factor[2]=0.729446491;
      m_factor[3]=0.895380324;
      //First try by Shinohara. No iteration.
    } else if(userFlag==201605) {
      m_factor[0]=0.568;
      m_factor[1]=0.497;
      m_factor[2]=0.601;
      m_factor[3]=0.687;
    } else {
      //do nothging and m_factor is one
    }
    

    if(userFlag==1 || userFlag==2) {
      posXY=0*mm;
    } else if(userFlag==201503) {
      m_isInBeamPosition=true;
      posXY=92*mm;
    } else {
      m_isInBeamPosition=false;
      posXY=190*mm;
    }
    
    std::vector<double> paramVec;
    std::vector<double> paramVecLECO;
    
    GsimBox* bhgc=0;
    GsimBox* lead=0;
    GsimBox* lecoframe=0;

    for(int i=0;i<m_nModule;i++){

      //////////////////////////Acrylic////////////////////////
      std::sprintf( nam,"BHGCmod%d",i );

      paramVec.clear();
      paramVec.push_back(m_width);
      paramVec.push_back(m_height);
      paramVec.push_back(m_thick);

      if(!bhgc) {
	bhgc = new GsimBox( nam, this,
			    G4ThreeVector(posMod[2*i]*posXY,posMod[2*i+1]*posXY,posZ+posZ_Acrylic[i/2]),
			    G4ThreeVector(angle[2*i]*90*deg,0,angle[2*i+1]*90*deg) );
	
	
	bhgc->setParameters( paramVec );
	if(m_opticalMode){
	  bhgc->setOuterMaterial( "GsimOpticalPMMA" );
	} else {
	  bhgc->setOuterMaterial( "G4_PLEXIGLASS" );
	}
	bhgc->setSensitiveDetector( "BHGC", i, false, true, true);
	bhgc->setOuterCopyNo( i );
	addDaughter( bhgc );
      } else {
	bhgc->cloneDetector(
			    G4ThreeVector(posMod[2*i]*posXY,posMod[2*i+1]*posXY,posZ+posZ_Acrylic[i/2]),
			    G4ThreeVector(angle[2*i]*90*deg,0,angle[2*i+1]*90*deg),
			    i);
      }

      if(userFlag!=1) {
	//////////////////////////lead///////////////////////////
	sprintf( nam,"BHGClead%d",i );
	
	paramVec.clear();
	paramVec.push_back(m_width);
	paramVec.push_back(m_height);
	paramVec.push_back(PbThick);
	
	lead = new GsimBox( nam, this,
			    G4ThreeVector(posMod[2*i]*posXY,posMod[2*i+1]*posXY,posZ+poslead[i/2]),
			    G4ThreeVector(angle[2*i]*90*deg,0,angle[2*i+1]*90*deg) );
	lead->setParameters( paramVec );
	lead->setOuterMaterial( "G4_Pb" );
	lead->setOuterColor("yellow");
	addDaughter( lead );
	
	m_detMoveList.push_back( lead );
	
      }
    }

    if(userFlag==201504 || userFlag==201503) {
      ///////////////////////LECO Frame  Vertical  ////////////////////////////////
      for(int j=0;j<2;j++){
	for(int i=0;i<4;i++){
	  sprintf( nam,"LECO_%d",4*j+i );
	  
	  paramVecLECO.clear();
	  paramVecLECO.push_back(lecox);
	  paramVecLECO.push_back(lecoy);
	  paramVecLECO.push_back(lecoz);
	  
	  lecoframe = new GsimBox( nam, this,
				   G4ThreeVector(posMod[2*j]*posXY+posLECO[2*i]*71.5*mm,
						 0,
						 posZ+posZ_Acrylic[0]+posLECO[2*i+1]*71.5*mm),
				   G4ThreeVector(90*deg,0,0) );
	  lecoframe->setParameters( paramVecLECO );
	  lecoframe->setOuterMaterial( "GsimLowDensityAl" );
	  lecoframe->setOuterColor("green");
	  addDaughter( lecoframe );
	  
	  m_detMoveList.push_back( lecoframe );
	}    
      }
      
      
      /////////////////////LECO Frame  Horizontal  /////////////////////////////
      for(int j=0;j<2;j++){
	for(int i=0;i<4;i++){
	  sprintf( nam,"LECOHori_%d",4*j+i );
	  
	  paramVecLECO.clear();
	  paramVecLECO.push_back(lecox);
	  paramVecLECO.push_back(lecoy);
	  paramVecLECO.push_back(lecoz);
	  
	  lecoframe = new GsimBox( nam, this,
				   G4ThreeVector(0,
						 posMod[2*j]*posXY+posLECO[2*i]*71.5*mm,
						 posZ+posZ_Acrylic[1]+posLECO[2*i+1]*71.5*mm),
				   G4ThreeVector(0,90*deg,0) );
	  lecoframe->setParameters( paramVecLECO );
	  lecoframe->setOuterMaterial( "GsimLowDensityAl" );
	  lecoframe->setOuterColor("green");
	  addDaughter( lecoframe );
	  
	  m_detMoveList.push_back( lecoframe );
	}    
      }
    }
    
    setThisAndDaughterBriefName( "BHGC" );
    
    
    if( m_opticalMode ) {
      std::list<GsimSensitiveDetector*> sdList=getThisAndDaughterSensitiveDetectorList();
      for(std::list<GsimSensitiveDetector*>::iterator it=sdList.begin();
          it!=sdList.end();it++) {
	std::string sdName=(*it)->getName();
        if(sdName == "BHGC") {
	  (*it)->setTdcThreshold(0.5);
          (*it)->setSensitiveMode(opticalPhotonSensitiveFastMode);
          (*it)->setHitProcessDetector(this);
        }
      }
    }
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. BHGC is not constructed.");
  }
}

bool GsimE14BHGC::processHits(const G4Step* aStep,G4TouchableHistory* ROhist, std::list<int>& channelList)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  if(!m_opticalMode) return true;
  
  G4Track*              track    = aStep->GetTrack();
  G4ParticleDefinition* particle = track->GetDefinition();
  
  //Treat only optical photon
  if(particle!=G4OpticalPhoton::OpticalPhotonDefinition()) return false;
  
  bool isHit=trace(aStep,ROhist,channelList);

  return isHit;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}

bool GsimE14BHGC::trace(const G4Step* aStep,G4TouchableHistory* , std::list<int>& channelList)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  const G4VTouchable*   touch= aStep->GetPreStepPoint()->GetTouchable();
  
  int modID = touch->GetCopyNumber(0);
  
  double hbarc=197.327053;//ev nm
  double energy=aStep->GetPreStepPoint()->GetKineticEnergy()/eV;//eV
  double waveL=2.*M_PI*hbarc/energy;//nm

  G4ThreeVector globalPos=aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector globalMom=aStep->GetPreStepPoint()->GetMomentum();
  
  G4ThreeVector pos
    =touch->GetHistory()->GetTopTransform().TransformPoint(globalPos);
  G4ThreeVector vec
    =touch->GetHistory()->GetTopTransform().TransformAxis(globalMom);

  //normalization
  vec = vec.unit();

  //First check PMT hit
  double QE=getPMTQE(waveL);
  QE=QE*m_factor[modID];

  double rnd=CLHEP::RandFlat::shoot();
  if(rnd>QE) {
    //vanish due to the PMT QE
    return false;
  }
  
  //RayTrace 
  //Path length and number of reflections
  double pathLength=0;
  int nReflectionY=0;
  int nReflectionZ=0;

  getPath(pos,vec,pathLength,nReflectionY,nReflectionZ);

  //Total reflection ?
  double costY=vec.y();
  double costZ=vec.z();
  double n=getRefractiveIndex(waveL);
  double totalRefCosT=std::sqrt(1.-1./(n*n));

  if( (nReflectionY>0 && costY>totalRefCosT)
      ||
      (nReflectionZ>0 && costZ>totalRefCosT)
      ) {
    // non total ref.
    return false;
  }

  //Reflection loss of the surface
  double wReflection=std::pow(m_reflectance[modID],(nReflectionY+nReflectionZ));
  rnd=CLHEP::RandFlat::shoot();
  if(rnd>wReflection) {
    //vanish in the imperfect surface reflection
    return false;
  }


  //Attenuation 
  double T=getTransmittance(waveL);
  double attenuation=exp(-T*(pathLength/mm));
  rnd=CLHEP::RandFlat::shoot();
  if(rnd>attenuation) {
    //vanish due to the attenuation
    return false;
  }
  
  const_cast<G4Step*>(aStep)->SetTotalEnergyDeposit(1.*MeV);
  if(vec.x()>0) {
    channelList.push_back(modID*2+0);
  } else {
    channelList.push_back(modID*2+1);
  }

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  

  return true;
}


double GsimE14BHGC::getPMTQE(double waveL)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //  Typical spectral response from Hamamatsu's spec sheet
  int np=12;
  double wlx[12]=
    {
      280.0,
      300.0,
      320.0,
      340.0,
      360.0,
      380.0,
      400.0,
      450.0,
      500.0,
      550.0,
      600.0,
      650.0
    };
  
  double QEwlx[12]=
    {
      0.00,
      0.050,
      0.13,
      0.18,
      0.21,
      0.22,
      0.22,
      0.17,
      0.11,
      0.060,
      0.022,
      0.000
    };

  double QEwl=0.0;
  if (waveL<wlx[0] || waveL>wlx[np-1]) {
    QEwl=0.0;
  } else {
    for(int k=0;k<np-1;k++) {
      if (waveL<wlx[k+1]) {
	QEwl = QEwlx[k]+
	  (QEwlx[k+1]-QEwlx[k])/(wlx[k+1]-wlx[k])* (waveL-wlx[k]);
	break;
      }
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return QEwl;
}

void GsimE14BHGC::getPath(G4ThreeVector& pos,
			  G4ThreeVector& vec,
			  double& pathLength,
			  int&    nReflectionY,
			  int&    nReflectionZ)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  double xMod=m_width;
  double yMod=m_height;
  double zMod=m_thick;
  
  double xPMT=+xMod/2.;
  if(vec.x()<0) xPMT=-xMod/2.;
  
  pathLength=std::abs( (xPMT-pos.x())/vec.x() );
  
  double yPMT=pathLength*vec.y()+pos.y();
  double zPMT=pathLength*vec.z()+pos.z();
  
  nReflectionY=int( std::abs(yPMT)/yMod +0.5 );
  nReflectionZ=int( std::abs(zPMT)/zMod +0.5 );

  yPMT-=yMod*nReflectionY*(vec.y()/std::abs(vec.y()));
  zPMT-=zMod*nReflectionZ*(vec.z()/std::abs(vec.z()));

  //update position
  pos.set(xPMT,yPMT,zPMT);

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
}


double GsimE14BHGC::getRefractiveIndex(double lambda)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif  
  //lambda in nm
  /*
    static const G4double A = 1.4788;
    static const G4double B = 4465.0;
    G4double n = A + B / sqr(lambda/nanometer);

    From K.Maeda the following parameters are used.
    static const double A = 1.5180;
    static const double B = -4.4444e-5;
    double n = A + B * lambda;
  */


  double a=1.491;
  double b=3.427e-3;
  double c=1.819e-4;
  double x=lambda*1e-3;
  
  double n=
    a+
    b*std::pow(x,-2.)+
    c*std::pow(x,-4.);


  //  double n=1.491;

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif  
  
  return n;
}


double GsimE14BHGC::getTransmittance(double waveL)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  

  //Transmittance
  int np=15;

  //lambda in nm
  double wlx[15]=
    {260,270,280,290,300,
     310,325,350,375,390,
     400,450,550,650,700};//UV transmitting acryl
  // {375.0,380.0,385.0,390.0,395.0,
  //     400.0,405.0,410.0,415.0,420.0,
  //     425.0,430.0,435.0,450.0,500.0,};//beamtest Acryl(none-UV transmitting)

  //transmittance in 1/cm
  double TRwlx[15]=
    {6.09,3.48,2.56,0.959,0.278,
     0.123,0.0897,0.0576,0.0195,0.00804,
     0.00481,0.0016,0.0016,0,0};//UV transmitting acryl
      // {6.35,4.43,1.96,0.744,0.27,
      //  0.10,0.0399,0.0189,0.0113,0.00819,
      //  0.00709,0.00654,0.00611,0.00523,0.0025};//beamtest Acryl(none-UV transmitting)


  double TRwl=0.0;
  if (waveL<wlx[0]) {
    TRwl=1e4;
  } else if (waveL>wlx[np-1]) {
    TRwl=0;
  } else {
    for(int k=0;k<np-1;k++) {
      if (waveL<wlx[k+1]) {
	TRwl = TRwlx[k]+
	  (TRwlx[k+1]-TRwlx[k])/(wlx[k+1]-wlx[k])* (waveL-wlx[k]);
	break;
      }
    }
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif

  return TRwl*0.1;//1/mm
}


GsimE14BHGC::~GsimE14BHGC()
{
  ;
}

void GsimE14BHGC::setFastSimulationLevel(int level)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //reset
  if(m_fastSimulationLevel==6) {
    for(std::list<GsimDetector*>::iterator it=m_detMoveList.begin();
	it!=m_detMoveList.end();it++) {
      G4ThreeVector pos=(*it)->getTranslationVector();
      double z=pos.getZ();
      pos.setZ(z+29*m);
      (*it)->setTranslationVector(pos);
      (*it)->setOuterVisibility(true);
    }
  }
  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detMoveList.begin();
	it!=m_detMoveList.end();it++) {
      G4ThreeVector pos=(*it)->getTranslationVector();
      double z=pos.getZ();
      pos.setZ(z-29*m);
      (*it)->setTranslationVector(pos);
      (*it)->setOuterVisibility(false);
    }
  }
  
  
  {
    m_fastSimulationLevel=level;
    for(GsimDetectorContainer::iterator it=m_daughterDetectorContainer.begin();
	it!=m_daughterDetectorContainer.end();it++) {
      GsimDetector* daughter =  (*it).second;
      daughter->setFastSimulationLevel(level);
    }
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif 
}



void GsimE14BHGC::comment()
{
  {
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::cout << "Z origin is upstream surface of the BHGC mount." << std::endl;
  std::cout << " ,which was the same as the downstream surface of BHPV mount." << std::endl;
  std::cout << "z=(614.8+479.2+100.+52)*cm+573.5*cm" << std::endl;
  std::cout << "If userFlag==1, only single module with the center of the acrylic for the origin is created." << std::endl;
  std::cout << "If userFlag==2, only single module with the center of the acrylic for the origin is created." << std::endl;
  std::cout << "userFlag : 201504 normal position." << std::endl;
  std::cout << "userFlag : 201503 in-beam position." << std::endl;
  std::cout << "userFlag : 201605 and later normal position. Place it downstream by 2 cm." << std::endl;
  std::cout << "z=(614.8+479.2+100.+52)*cm+535.6*cm+2*cm" << std::endl;
  if(m_isInBeamPosition) {
    std::cout << "Now in-beam position" << std::endl;
  } else {
    std::cout << "Now out-beam position" << std::endl;
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif 
}



void GsimE14BHGC::setUserParameters(std::string tag,std::vector<double> vec)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  if(tag == "OverallFactor") {
    
    if(vec.size()==0) {
      std::cout << "Print for " << tag << std::endl;
      for(int i=0;i<m_nModule;i++) {
	std::cout << i << "\t" << m_factor[i] << std::endl;
      }
    } else if(vec.size()==static_cast<size_t>(m_nModule)) {
      for(int i=0;i<m_nModule;i++) {
	m_factor[i] = vec[i];
      }
    } else {
      std::cout << "Number of parameters is not euqal to nMod, " << m_nModule << std::endl;
      return;
    }
    
  } else if(tag == "Reflectance") {

    if(vec.size()==0) {
      std::cout << "Print for " << tag << std::endl;
      for(int i=0;i<m_nModule;i++) {
	std::cout << i << "\t" << m_reflectance[i] << std::endl;
      }
    } else if(vec.size()==static_cast<size_t>(m_nModule)) {
      for(int i=0;i<m_nModule;i++) {
	m_reflectance[i] = vec[i];
      }
    } else {
      std::cout << "Number of parameters is not euqal to nMod, " << m_nModule << std::endl;
      return;
    }
    
  } else {
    
    std::cout << "Tag, " << tag << ", does not match." << std::endl;
    std::cout << "Candidates are " << std::endl;
    std::cout << "OverallFactor X" << std::endl;
    std::cout << "Reflectance X" << std::endl;
    return;
  }

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif 
}
