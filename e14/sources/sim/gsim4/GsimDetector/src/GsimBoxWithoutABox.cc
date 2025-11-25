/**
 *  @file
 *  @brief  GsimBoxWithoutABox
 *  $Id: GsimBoxWithoutABox.cc,v 1.3 Toru Matsumura
 *  $Log: GsimBoxWithoutABox.cc,v $
 *  Revision 1.3  2006/12/06 19:14:30  nanjo
 *  CVS variables.
 * 
 */
#include "GsimDetector/GsimBoxWithoutABox.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

GsimBoxWithoutABox::GsimBoxWithoutABox(std::string name,
		 GsimDetector* motherDetector,
		 G4ThreeVector transV,G4ThreeVector rotV,
                 int nParameterized)
  : GsimDetector(name,motherDetector,transV,rotV,nParameterized)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  m_className = "GsimBoxWithoutABox";

  m_numberOfParameters=8;
  m_parameterArray= new double[m_numberOfParameters];
  m_parameterArray[0] =10.*mm; // total x length
  m_parameterArray[1] =10.*mm; // total y length
  m_parameterArray[2] =10.*mm; // total z length
  m_parameterArray[3] =3.*mm;  // total x length of the false box
  m_parameterArray[4] =3.*mm;  // total y length of the false box
  m_parameterArray[5] =0.*mm;  // x position of the false box
  m_parameterArray[6] =0.*mm;  // y position of the false box
  m_parameterArray[7] =0.*deg; // rotation angle of the false box (0 deg)
  
  m_outerMaterialName = "G4_PLASTIC_SC_VINYLTOLUENE";
  m_outerColorName    ="yellow";
  
  update();
  

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimBoxWithoutABox::~GsimBoxWithoutABox()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimBoxWithoutABox::constructPhysiDetector()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4VPhysicalVolume* physiMother=getMotherPhysiDetector();
  std::string fullName = getFullName();
  if(physiMother) {


    G4Box* solidBoxT = new G4Box(G4String((fullName+"_boxT").c_str()),
				 m_parameterArray[0]/2.,
				 m_parameterArray[1]/2.,
				 m_parameterArray[2]/2.);

    G4Box* solidBoxF = new G4Box(G4String((fullName+"_boxF").c_str()),
				 m_parameterArray[3]/2.,
				 m_parameterArray[4]/2.,
				 m_parameterArray[2]/2.*1.01);

    G4ThreeVector armVec= G4ThreeVector(-m_parameterArray[5],
					-m_parameterArray[6],
					0.);
    G4RotationMatrix armRot= G4RotationMatrix();

    armRot.rotateZ(m_parameterArray[7]);
    armVec.rotateZ(m_parameterArray[7]);

    
    G4SubtractionSolid* solidDetector
      = new G4SubtractionSolid( G4String(fullName.c_str()),
				solidBoxT,
				solidBoxF, 
				G4Transform3D(armRot, armVec) );
    


    G4LogicalVolume* logicDetector
      = new G4LogicalVolume(solidDetector,
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

void GsimBoxWithoutABox::updateParametersEtc()
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

    G4Box* solidBoxT = new G4Box(G4String((fullName+"_boxT").c_str()),
				 m_parameterArray[0]/2.,
				 m_parameterArray[1]/2.,
				 m_parameterArray[2]/2.);

    G4Box* solidBoxF = new G4Box(G4String((fullName+"_boxF").c_str()),
				 m_parameterArray[3]/2.,
				 m_parameterArray[4]/2.,
				 m_parameterArray[2]/2.*1.01);

    G4ThreeVector armVec= G4ThreeVector(-m_parameterArray[5],
					-m_parameterArray[6],
					0.);
    G4RotationMatrix armRot= G4RotationMatrix();

    armRot.rotateZ(m_parameterArray[7]);
    armVec.rotateZ(m_parameterArray[7]);

    solidDetector
      = new G4SubtractionSolid( G4String(fullName.c_str()),
				solidBoxT,
				solidBoxF, 
				G4Transform3D(armRot, armVec) );
    
  
  logicDetector->SetSolid(solidDetector);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}


void GsimBoxWithoutABox::extractTwoPlanesDXF(std::list<TVector3>& v0List,
					     std::list<TVector3>& v1List,
					     std::list<TVector3>& v2List,
					     std::list<TVector3>& v3List)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  {
    double dx=m_parameterArray[0]/2.;
    double dy=m_parameterArray[1]/2.;
    double dz=m_parameterArray[2]/2.;
    
    TVector3 v[2][4];
    v[0][0]=TVector3(+dx,+dy,+dz);
    v[0][1]=TVector3(-dx,+dy,+dz);
    v[0][2]=TVector3(-dx,-dy,+dz);
    v[0][3]=TVector3(+dx,-dy,+dz);
    v[1][0]=TVector3(+dx,+dy,-dz);
    v[1][1]=TVector3(-dx,+dy,-dz);
    v[1][2]=TVector3(-dx,-dy,-dz);
    v[1][3]=TVector3(+dx,-dy,-dz);
    
    for(int i=0;i<4;i++) {
      v0List.push_back(v[0][i]);
      v1List.push_back(v[1][i]);
    }
  }

  {
    double dx=m_parameterArray[3]/2.;
    double dy=m_parameterArray[4]/2.;
    double dz=m_parameterArray[2]/2.;
    double sx=m_parameterArray[5];
    double sy=m_parameterArray[6];
    double phi=m_parameterArray[7];

    TVector3 v[2][4];
    v[0][0]=TVector3(+dx,+dy,+dz);
    v[0][1]=TVector3(-dx,+dy,+dz);
    v[0][2]=TVector3(-dx,-dy,+dz);
    v[0][3]=TVector3(+dx,-dy,+dz);
    v[1][0]=TVector3(+dx,+dy,-dz);
    v[1][1]=TVector3(-dx,+dy,-dz);
    v[1][2]=TVector3(-dx,-dy,-dz);
    v[1][3]=TVector3(+dx,-dy,-dz);

    TVector3 shiftV = TVector3(+sx,+sy,0);
    for(int i=0;i<4;i++) {
      v[0][i]=v[0][i]-shiftV;
      v[1][i]=v[1][i]-shiftV;
      
      v[0][i].RotateZ(phi);
      v[1][i].RotateZ(phi);
      
      
    }
    
    for(int i=0;i<4;i++) {
      v2List.push_back(v[0][i]);
      v3List.push_back(v[1][i]);
    }
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
