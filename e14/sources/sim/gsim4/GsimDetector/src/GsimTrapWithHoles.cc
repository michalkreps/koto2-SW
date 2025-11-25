/**
 *  @file
 *  @brief  GsimTrapWithHoles
 *  $Id:$ 
 *  $Log:$
 */
#include "GsimDetector/GsimTrapWithHoles.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4SubtractionSolid.hh"

#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

GsimTrapWithHoles::GsimTrapWithHoles(std::string name,
				     GsimDetector* motherDetector,
				     G4ThreeVector transV,
				     G4ThreeVector rotV,
				     int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_className = "GsimTrapWithAHoles";

  m_rotsub = new G4RotationMatrix;
  m_rotsub->rotateX(-90*deg);

  m_numberOfParameters=23;
  m_parameterArray= new double[m_numberOfParameters];
  m_parameterArray[0] =10.*mm; // pDz
  m_parameterArray[1] =0; // pTheta
  m_parameterArray[2] =0; // pPhi
  m_parameterArray[3] =10.*mm;  // pDy1
  m_parameterArray[4] =10.*mm;  // pDx1
  m_parameterArray[5] =10.*mm;  // pDx2
  m_parameterArray[6] =0;  // pAlp1
  m_parameterArray[7] =10.*mm; // pDy2
  m_parameterArray[8] =10.*mm; // pDx3
  m_parameterArray[9] =10.*mm; // pDx4
  m_parameterArray[10] =0;  // pAlp2
  
  m_parameterArray[11] =10.*mm; //
  m_parameterArray[12] =10.*mm; //
  m_parameterArray[13] =10.*mm; //
  m_parameterArray[14] =0.*mm; //
  m_parameterArray[15] =0.*mm; //
  m_parameterArray[16] =0.*mm; //
  
  m_parameterArray[17] =-10.*mm; //
  m_parameterArray[18] =-10.*mm; //
  m_parameterArray[19] =-10.*mm; //
  m_parameterArray[20] =0.*mm; //
  m_parameterArray[21] =0.*mm; //
  m_parameterArray[22] =0.*mm; //



  m_outerMaterialName = "G4_PLASTIC_SC_VINYLTOLUENE";
  m_outerColorName    ="yellow";

  
  
  update();
  

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimTrapWithHoles::~GsimTrapWithHoles()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  delete m_rotsub;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimTrapWithHoles::constructPhysiDetector()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4VPhysicalVolume* physiMother=getMotherPhysiDetector();
  std::string fullName = getFullName();
  
  if(physiMother) {
    
    
    
    G4Trap* solidtrap = new G4Trap(G4String( (fullName+"_trap").c_str()),
				   m_parameterArray[0],  // pDz
				   m_parameterArray[1],  // pTheta
				   m_parameterArray[2],  // pPhi
				   m_parameterArray[3],  // pDy1
				   m_parameterArray[4],  // pDx1
				   m_parameterArray[5],  // pDx2
				   m_parameterArray[6],  // pAlp1
				   m_parameterArray[7],  // pDy2
				   m_parameterArray[8],  // pDx3
				   m_parameterArray[9],  // pDx4
				   m_parameterArray[10]  // pAlp2
				   );
    
    
    G4Box* solidhole1 = 0; 
    if(m_parameterArray[11]>0 && m_parameterArray[12]>0 && m_parameterArray[13]>0){
      solidhole1 = new G4Box(G4String((fullName+"_holebox").c_str()),
			     m_parameterArray[11],
			     m_parameterArray[12],
			     m_parameterArray[13]
			     );
    }
    G4VSolid* solidhole2 = 0; 
    if(m_parameterArray[17]>=0 && m_parameterArray[18]>0 && m_parameterArray[19]>0){
      
      if(solidhole1!=0){
	
	solidhole2 = new G4Box(G4String((fullName+"_holebox2").c_str()),
			       m_parameterArray[17],
			       m_parameterArray[18],
			       m_parameterArray[19]
			       );
      }else{

	solidhole2 = new G4Tubs(G4String((fullName+"_holebox2").c_str()),
				m_parameterArray[17], m_parameterArray[18],
				m_parameterArray[19], 0, 360*radian
				);
	
      }
    }
			 
    std::string sssub = fullName.c_str();
    sssub += std::string("_") + G4String(fullName+"_holebox");
  
    G4VSolid* solid;

    G4VSolid* solidDetector;
    
    if(solidhole1==0) {
      solidDetector = solidtrap;
    } else {
      solidDetector = new G4SubtractionSolid( sssub.c_str(),
					      solidtrap,
					      solidhole1,
					      0,
					      G4ThreeVector( m_parameterArray[14], m_parameterArray[15], m_parameterArray[16] ) );
    }
  
    solid = solidDetector;

    if(solidhole2!=0){
      sssub += std::string("_") + G4String(fullName+"_holebox2").c_str();
      G4VSolid* solidDetector2 = new G4SubtractionSolid( sssub.c_str(),
							 solidDetector,
							 solidhole2,
							 m_rotsub,
							 G4ThreeVector( m_parameterArray[20], m_parameterArray[21], m_parameterArray[22] ) );

      solid = solidDetector2;
    }
  
    G4LogicalVolume* logicDetector
      = new G4LogicalVolume(solid,
			    GsimUtil::getUtil()->findMaterial(m_outerMaterialName),
			    G4String(fullName.c_str()));
    G4VisAttributes* visAtt
      = new G4VisAttributes(GsimUtil::getUtil()->findColor(m_outerColorName));
    logicDetector->SetVisAttributes(visAtt);
    
    m_physiDetector = 
      new G4PVPlacement(m_rotationMatrix,
			G4ThreeVector(),
			G4String(fullName.c_str()),
			logicDetector,
			physiMother,
			false,0
			);
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}








void GsimTrapWithHoles::updateParametersEtc()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  if(!m_physiDetector) return;
  
  G4LogicalVolume* logicDetector = m_physiDetector->GetLogicalVolume();
  if(!logicDetector) {
    GsimMessage::getInstance()
      ->report("warning","No logicDetector is constructed.");
    return; 
  }
  G4VSolid* solidDetector = logicDetector->GetSolid();
  if(!solidDetector) {
    GsimMessage::getInstance()
      ->report("warning","No solidDetector is constructed.");
    return;
  }


  delete solidDetector;
  solidDetector=0;
  
  std::string fullName = getFullName();

  G4Trap* solidtrap = new G4Trap(G4String( (fullName+"_trap").c_str()),
				 m_parameterArray[0],  // pDz
				 m_parameterArray[1],  // pTheta
				 m_parameterArray[2],  // pPhi
				 m_parameterArray[3],  // pDy1
				 m_parameterArray[4],  // pDx1
				 m_parameterArray[5],  // pDx2
				 m_parameterArray[6],  // pAlp1
				 m_parameterArray[7],  // pDy2
				 m_parameterArray[8],  // pDx3
				 m_parameterArray[9],  // pDx4
				 m_parameterArray[10]  // pAlp2
				 );
  
  G4Box* solidhole1 = 0;
 
  if(m_parameterArray[11]>0 && m_parameterArray[12]>0 && m_parameterArray[13]>0){
    solidhole1 = new G4Box(G4String((fullName+"_holebox").c_str()),
			   m_parameterArray[11],
			   m_parameterArray[12],
			   m_parameterArray[13]
			   );

  }
  G4VSolid* solidhole2 = 0; 
  if(m_parameterArray[17]>=0 && m_parameterArray[18]>0 && m_parameterArray[19]>0){
  	
    if(solidhole1!=0){
      solidhole2 = new G4Box(G4String((fullName+"_holebox2").c_str()),
			     m_parameterArray[17],
			     m_parameterArray[18],
			     m_parameterArray[19]
			     );
    }else{

      solidhole2 = new G4Tubs(G4String((fullName+"_holebox2").c_str()),
			      m_parameterArray[17], m_parameterArray[18],
			      m_parameterArray[19], 0, 360*radian
			      );
    }
  }
			 
  std::string sssub = fullName.c_str();
  sssub += std::string("_") + G4String(fullName+"_holebox").c_str();
  
  if(solidhole1==0) {
    solidDetector = solidtrap;
  } else  {
    solidDetector
      = new G4SubtractionSolid( sssub.c_str(),
				solidtrap,
				solidhole1,
				0,
				G4ThreeVector( m_parameterArray[14], m_parameterArray[15], m_parameterArray[16] ) );
  }
  
  if(solidhole2!=0){
    sssub += std::string("_") + G4String(fullName+"_holebox2").c_str();
    G4VSolid* solidDetector2 = new G4SubtractionSolid( sssub.c_str(),
						       solidDetector,
						       solidhole2,
						       m_rotsub,
						       G4ThreeVector( m_parameterArray[20], m_parameterArray[21], m_parameterArray[22] ) );

    logicDetector->SetSolid(solidDetector2);
  }else{
    logicDetector->SetSolid(solidDetector);
  }
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimTrapWithHoles::extractTwoPlanesDXF(std::list<TVector3>& v0List,
					    std::list<TVector3>& v1List,
					    std::list<TVector3>& ,
					    std::list<TVector3>& )
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  double dz=m_parameterArray[0];
  double pTheta=m_parameterArray[1];
  double pPhi=m_parameterArray[2];
  double pDy1=m_parameterArray[3];
  double pDx1=m_parameterArray[4];
  double pDx2=m_parameterArray[5];
  double pAlp1=m_parameterArray[6];
  double pDy2=m_parameterArray[7];
  double pDx3=m_parameterArray[8];
  double pDx4=m_parameterArray[9];
  double pAlp2=m_parameterArray[10];


  for(int i=0;i<4;i++) {

    double r=dz/cos(pTheta);

    double z=-dz;
    double xCenter = -r*sin(pTheta)*cos(pPhi);
    double yCenter = -r*sin(pTheta)*sin(pPhi);

    v0List.push_back( TVector3( xCenter-tan(pAlp1)*pDy1+pDx1,yCenter-pDy1,z) );
    v0List.push_back( TVector3( xCenter+tan(pAlp1)*pDy1+pDx2,yCenter+pDy1,z) );
    v0List.push_back( TVector3( xCenter+tan(pAlp1)*pDy1-pDx2,yCenter+pDy1,z) );
    v0List.push_back( TVector3( xCenter-tan(pAlp1)*pDy1-pDx1,yCenter-pDy1,z) );

    z=+dz;
    xCenter = r*sin(pTheta)*cos(pPhi);
    yCenter = r*sin(pTheta)*sin(pPhi);
    v1List.push_back( TVector3( xCenter-tan(pAlp2)*pDy2+pDx3,yCenter-pDy2,z) );
    v1List.push_back( TVector3( xCenter+tan(pAlp2)*pDy2+pDx4,yCenter+pDy2,z) );
    v1List.push_back( TVector3( xCenter+tan(pAlp2)*pDy2-pDx4,yCenter+pDy2,z) );
    v1List.push_back( TVector3( xCenter-tan(pAlp2)*pDy2-pDx3,yCenter-pDy2,z) );
  }
  

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
