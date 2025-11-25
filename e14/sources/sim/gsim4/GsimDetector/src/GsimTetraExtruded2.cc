/**
 *  @file
 *  @brief  GsimTetraExtruded2
 *  $Id: 
 *  $Log: 
 */
#include "GsimDetector/GsimTetraExtruded2.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4ExtrudedSolid.hh"
#include "G4TwoVector.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

GsimTetraExtruded2::GsimTetraExtruded2(std::string name,
			     GsimDetector* motherDetector,
			     G4ThreeVector transV,G4ThreeVector rotV,
			     int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  m_className = "GsimTetraExtruded2";

  m_numberOfParameters=15;
  m_parameterArray= new double[m_numberOfParameters];
  m_parameterArray[0]  = 10*mm; // x0 in z=0
  m_parameterArray[1]  = 10*mm; // y0 in z=0
  m_parameterArray[2]  =-10*mm; // x1 in z=0
  m_parameterArray[3]  = 10*mm; // y1 in z=0
  m_parameterArray[4]  =-10*mm; // x2 in z=0
  m_parameterArray[5]  =-10*mm; // y2 in z=0
  m_parameterArray[6]  = 22*mm; // x3 in z=0
  m_parameterArray[7]  = -6*mm; // y3 in z=0
  m_parameterArray[8]  =  5*mm; // full z length
  m_parameterArray[9]  =  0*mm; // center x in -z/2
  m_parameterArray[10] =  0*mm; // center y in -z/2
  m_parameterArray[11] =  1.;   // scale in -z/2
  m_parameterArray[12] =  0*mm; // center x in +z/2
  m_parameterArray[13] =  0*mm; // center y in +z/2
  m_parameterArray[14] =  1.;   // scale in +z/2

  m_outerMaterialName = "G4_PLASTIC_SC_VINYLTOLUENE";
  m_outerColorName    ="blue";
  
  update();
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimTetraExtruded2::~GsimTetraExtruded2()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  ;
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

void GsimTetraExtruded2::constructPhysiDetector()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  G4VPhysicalVolume* physiMother=getMotherPhysiDetector();
  if(physiMother) {
    std::string fullName = getFullName();
    G4ExtrudedSolid* solidDetector=constructSolid(fullName);
    
    G4LogicalVolume* logicDetector
      = new G4LogicalVolume(solidDetector,
			    GsimUtil::getUtil()->findMaterial(m_outerMaterialName),
			    G4String(fullName.c_str()));
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

G4ExtrudedSolid* GsimTetraExtruded2::constructSolid(std::string fullName)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  std::vector<G4TwoVector> polygon;
  polygon.push_back( G4TwoVector(m_parameterArray[0],m_parameterArray[1]) );
  polygon.push_back( G4TwoVector(m_parameterArray[2],m_parameterArray[3]) );
  polygon.push_back( G4TwoVector(m_parameterArray[4],m_parameterArray[5]) );
  polygon.push_back( G4TwoVector(m_parameterArray[6],m_parameterArray[7]) );
  
  double halfZ=m_parameterArray[8]/2.;

  G4TwoVector off1(m_parameterArray[9],m_parameterArray[10]);
  double scale1=m_parameterArray[11];
  G4TwoVector off2(m_parameterArray[12],m_parameterArray[13]);
  double scale2=m_parameterArray[14];
  
  G4ExtrudedSolid* solidDetector
    = new G4ExtrudedSolid(G4String(fullName.c_str()),
			  polygon,
			  halfZ,
			  off1,scale1,
			  off2,scale2);
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return solidDetector;
}

void GsimTetraExtruded2::updateParametersEtc()
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
  G4ExtrudedSolid* extsolid = dynamic_cast<G4ExtrudedSolid*>(solidDetector);
  if( isSameDimension(extsolid) ) {
    //Parameters are the same
    //Do nothing
    return;
  };
  delete solidDetector;
  solidDetector=0;
  std::string fullName = getFullName();
  solidDetector=constructSolid(fullName);
  logicDetector->SetSolid(solidDetector);
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

bool GsimTetraExtruded2::isSameDimension(G4ExtrudedSolid* extSolid) const
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  std::vector<G4TwoVector> polygon =  extSolid->GetPolygon();
  std::vector<G4ExtrudedSolid::ZSection> zsections  =  extSolid->GetZSections();
  
  if( polygon.size()!=4 ) {
    GsimMessage::getInstance()
      ->report("warning","Size of polygon is not 4. Dimesions are not updated.");
    return false;
  }
  if( zsections.size()!=2 ) {
    GsimMessage::getInstance()
      ->report("warning","Size of zsections is not 2. Dimesions are not updated.");
    return false;
  }

  bool isMatch=true;
  for(int i=0;i<4;i++) {
    if(m_parameterArray[2*i]!=polygon[i].x() ||
       m_parameterArray[2*i+1]!=polygon[i].y()) {
      isMatch=false;
      break;
    }
  }

  if(zsections[0].fZ!=-m_parameterArray[8]/2. ||
     zsections[1].fZ!=+m_parameterArray[8]/2. ||
     zsections[0].fOffset.x()!=m_parameterArray[9] ||
     zsections[0].fOffset.y()!=m_parameterArray[10] ||
     zsections[0].fScale!=m_parameterArray[11] ||
     zsections[1].fOffset.x()!=m_parameterArray[12] ||
     zsections[1].fOffset.y()!=m_parameterArray[13] ||
     zsections[1].fScale!=m_parameterArray[14]
     ) isMatch=false;
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return isMatch;
}

void GsimTetraExtruded2::extractTwoPlanesDXF(std::list<TVector3>& v0List,
					     std::list<TVector3>& v1List,
					     std::list<TVector3>&,
					     std::list<TVector3>&
					     )
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
  G4ExtrudedSolid* extSolid = dynamic_cast<G4ExtrudedSolid*>(solidDetector);

  std::vector<G4TwoVector> polygon =  extSolid->GetPolygon();
  std::vector<G4ExtrudedSolid::ZSection> zsections  =  extSolid->GetZSections();
  if( polygon.size()!=4 ) {
    GsimMessage::getInstance()
      ->report("warning","Size of polygon is not 4. Dimesions are not updated.");
    return;
  }
  if( zsections.size()!=2 ) {
    GsimMessage::getInstance()
      ->report("warning","Size of zsections is not 2. Dimesions are not updated.");
    return;
  }

  for(int i=0;i<4;i++) {
    double x0=polygon[i].x();
    double y0=polygon[i].y();

    for(int j=0;j<2;j++) {
      double z=zsections[j].fZ;
      double dx=zsections[j].fOffset.x();
      double dy=zsections[j].fOffset.y();
      double sc=zsections[j].fScale;
      
      double x=x0*sc+dx;
      double y=y0*sc+dy;
      if(j==0) {
	v0List.push_back( TVector3(x,y,z) );
      } else {
	v1List.push_back( TVector3(x,y,z) );
      }
    }
  }
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}
