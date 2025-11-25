/**
 *  @file
 *  @brief  GsimE14UCV
 *  $Id$
 *  $Log$
 */
#include "GsimE14Detector/GsimE14UCV.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimTrap.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14UCV::GsimE14UCV(std::string name,
		       GsimDetector* motherDetector,
		       G4ThreeVector transV,G4ThreeVector rotV,
		       int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  m_className = "GsimE14UCV";
  if(userFlag<202002){
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. UCV is not constructed.");
    return;
  }
  if(! (userFlag==202002 ||
	userFlag==202102 ||
	userFlag==202306
	)
     ) {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. UCV is not constructed.");
    return;
    
  }
  if(userFlag==202002) {
    const double yLen = 140.0*mm;
    const double yedgepos = -42*mm;
    const double zPos = 0;
    //cladding is 5.8% for 1mm fiber, 4% for 0.5mm fiber including both sides
    //from Kuraray private commnuication
    
    double wCore = 1*(1.-5.8e-2)*mm;
    GsimBox* box = nullptr;
    for(int i=0;i<84;i++){  
      
      std::vector<double> paramVec;
      //South
      paramVec.clear();
      paramVec.push_back(wCore);
      paramVec.push_back(yLen);
      paramVec.push_back(wCore);
      
      if(box==nullptr){
    	box = new GsimBox("UCVSci",this,
			  G4ThreeVector(1.0*mm*(i-42+0.5),
					(yedgepos + yLen/2),
					zPos),
			  G4ThreeVector(0,0,0));
	
	box->setParameters(paramVec);
	box->setOuterMaterial("G4_POLYSTYRENE");
	box->setOuterColor("blue");
	box->setSensitiveDetector("UCV",i/7);
	addDaughter(box);
	m_detMoveList.push_back(box);
      }else{
	box->cloneDetector(G4ThreeVector(1.0*mm*(i-42+0.5),
					 (yedgepos + yLen/2),
					 zPos),
			   G4ThreeVector(0,0,0), i/7);
      }
    }
  } else if(userFlag==202102) {
    //Fiber UCV
    const float angle = -25*M_PI/180.0;
    const float cladThickness = 0.5*mm*2e-2;//2% for single side
    const float moduleGap = 0.02*mm;
    const float diaFib = 0.5*mm - 2*cladThickness;
    const int NcoreMod = 18;
    const int NeachHaloMod = 3;
    const int NfibPerMod = 16;
    const int Ncorefiber = NcoreMod * NfibPerMod;
    const int NeachHalofiber = NeachHaloMod * NfibPerMod;
    const float coreCatcherWidth = Ncorefiber*0.5*mm + (NcoreMod-1)*moduleGap;
    const float haloCatcherWidth = NeachHalofiber*0.5*mm + (NeachHaloMod-1)*moduleGap;
    GsimBox* box = nullptr;
    
    // Generates boxes for fibers in core region
    for(int i=0;i<Ncorefiber;i++){
      //Creates the fiber and sets with the position and rotation
      double loc = (-(Ncorefiber-1)*0.5/2*mm - (NcoreMod-1)*moduleGap/2 + std::floor(i/NfibPerMod)*moduleGap + i*0.5*mm);
      double y = loc*cos(angle);
      double z = loc*sin(angle);
      double x = 45.75*mm;
      int moduleID = i/(NfibPerMod*2);
      if(box==nullptr){
	box = new GsimBox("UCVSci",this,
			  G4ThreeVector(x, y, z),
			  G4ThreeVector(angle,0,0));
	
	// Sets dimensions of the first fiber
	std::vector<double> paramVec;
	paramVec.clear();
	paramVec.push_back(250*mm);
	paramVec.push_back(diaFib);
	paramVec.push_back(diaFib);
	box->setParameters(paramVec);
	box->setOuterMaterial("G4_POLYSTYRENE");
	box->setOuterColor("blue");
	box->setSensitiveDetector("UCV",moduleID);
	addDaughter(box);
	m_detMoveList.push_back(box);
      }else{
	box->cloneDetector(
			   G4ThreeVector(x, y, z),
			   G4ThreeVector(angle,0,0), moduleID);
      }
    }
    
    // Generates boxes for fibers in lower halo region
    for(int i=0;i<NeachHalofiber;i++){
      int moduleID = 9;
      if(i>=2*NfibPerMod) moduleID = 10;
      double loc = (-(NeachHalofiber-1)*0.5*mm/2 - (NeachHaloMod-1)*moduleGap/2 + std::floor(i/NfibPerMod)*moduleGap + i*0.5*mm);
      G4ThreeVector pos(0,loc,0);
      pos.rotateX(45*degree);
      pos += G4ThreeVector(-45.75*mm, -coreCatcherWidth/2, 0*mm); //moves center of halo to end of core 
      pos += G4ThreeVector(0*mm, -haloCatcherWidth/(2*sqrt(2)), -haloCatcherWidth/(2*sqrt(2))); // moves end of halo to end of core
      pos += G4ThreeVector(0*mm, -0.5*mm/2*sqrt(2) + 3/sqrt(2)*mm, 0.5*mm/2 + 3/sqrt(2)*mm); //prevent fiber in same space, add 3mm core halo overlap
      pos.rotateX(angle);
      box->cloneDetector( pos, G4ThreeVector(angle+45*M_PI/180.,0,0),moduleID);
    }
    
    
    // Generates boxes for fibers in upper halo region
    for(int i=0;i<NeachHalofiber;i++){
      int moduleID = 10;
      if(i>=NfibPerMod) moduleID = 11;
      double loc = (-(NeachHalofiber-1)*0.5*mm/2 - (NeachHaloMod-1)*moduleGap/2 + std::floor(i/NfibPerMod)*moduleGap + i*0.5*mm);
      G4ThreeVector pos(0,loc,0);
      pos.rotateX(45*degree);
      pos += G4ThreeVector(-45.75*mm, coreCatcherWidth/2, 0*mm); //moves center of halo to end of core
      pos += G4ThreeVector(0*mm, haloCatcherWidth/(2*sqrt(2)), haloCatcherWidth/(2*sqrt(2))); // moves end of halo to end of core
      pos += G4ThreeVector(0*mm, 0.5*mm/2*sqrt(2) - 3/sqrt(2)*mm, -0.5*mm/2 - 3/sqrt(2)*mm); //prevent fiber in same space, add 3mm core halo overlap
      pos.rotateX(angle);
      box->cloneDetector( pos, G4ThreeVector(angle+45*M_PI/180.,0,0),moduleID);
    }
  } else if (userFlag>=202306) {
    /////////////////////
    //scintillator film//
    /////////////////////
    const double ylen = 160*mm;
    const double xlen = 160*mm;
    const double zlen = 0.2*mm;
    
    GsimBox* box = new GsimBox("UCVsci", this, G4ThreeVector(0, 0, 0), G4ThreeVector(0, 0, 0));
    
    std::vector<double> paramVec;
    paramVec.clear();
    paramVec.push_back(xlen);
    paramVec.push_back(ylen);
    paramVec.push_back(zlen);
    box->setParameters(paramVec);
    box->setOuterMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    box->setOuterColor("blue");
    box->setSensitiveDetector("UCV", 0);
    addDaughter(box);
    m_detMoveList.push_back(box);
    
    //////////////////////
    //Al mylar Trapezoid//
    //////////////////////
    double eps=0.01*mm;//just for tolerance, but not zero
    double hHex=166*mm;
    double tMylar=0.012*mm;

    //         ... ^z
    //      ...  |
    //   ...     |sqrt(3)/2 => 40.89 deg
    //   --------- ->x
    //       1
    //
    double x0Trap=hHex;
    double x1Trap=hHex/2.;
    double yTrap=hHex/2.*std::sqrt(7)/2.;
    double rotTrap=std::atan(sqrt(3)/2.)*rad;

    paramVec.clear();
    paramVec.push_back(tMylar);//Full-length along the z-axis
    paramVec.push_back(0*deg);//Polar angle of the line joining the centres of the faces at -/+pDz
    paramVec.push_back(0*deg);//Azimuthal angle of the line joing the centre of the face at -pDz to the centre of the face at +pDz
    paramVec.push_back(yTrap);//Full-length along y of the face at -pDz
    paramVec.push_back(x0Trap);//Full-length along x of the side at y=-pDy1 of the face at -pDz
    paramVec.push_back(x1Trap);//Full-length along x of the side at y=+pDy1 of the face at -pDz
    paramVec.push_back(0*deg);//Angle with respect to the y axis from the centre of the side at y=-pDy1 to the centre at y=+pDy1 of the face at -pDz 
    paramVec.push_back(yTrap);//Full-length along y of the face at +pDz
    paramVec.push_back(x0Trap);//Full-length along x of the side at y=-pDy2 of the face at +pDz
    paramVec.push_back(x1Trap);//Full-length along x of the side at y=+pDy2 of the face at +pDz
    paramVec.push_back(0*deg);//Angle with respect to the y axis from the centre of the side at y=-pDy2 to the centre at y=+pDy2 of the face at +pDz

    double dx=hHex/4.                +tMylar/2.*std::sqrt(3)/std::sqrt(7);
    double dz=hHex/4.*std::sqrt(3)/2.+tMylar/2.*2*std::sqrt(7);
    double TrapShift[4][3] = {
      {-dx-eps,0,-dz-zlen/2.-eps},
      {+dx+eps,0,-dz-zlen/2.-eps},
      {-dx-eps,0,+dz+zlen/2.+eps},
      {+dx+eps,0,+dz+zlen/2.+eps}
    };
    
    double TrapRot[4][3] = {
      {-rotTrap, 0, +90*deg},
      {-rotTrap, 0, -90*deg},
      {+rotTrap, 0, +90*deg},
      {+rotTrap, 0, -90*deg}
    };
    GsimTrap* trapezoid =0;
    for (int i=0;i<4;i++){
      if(i==0) {
	trapezoid = new GsimTrap("UCVMylar_trapezoid", this,
				 G4ThreeVector(TrapShift[i][0], TrapShift[i][1], TrapShift[i][2]),
				 G4ThreeVector(TrapRot[i][0], TrapRot[i][1], TrapRot[i][2]));
	trapezoid->setParameters(paramVec);
	trapezoid->setOuterMaterial("G4_MYLAR"); 
	trapezoid->setOuterColor("gray70"); 
	addDaughter(trapezoid);
	m_detMoveList.push_back(trapezoid);
      } else {
	trapezoid->cloneDetector(
				 G4ThreeVector(TrapShift[i][0], TrapShift[i][1], TrapShift[i][2]),
				 G4ThreeVector(TrapRot[i][0], TrapRot[i][1], TrapRot[i][2]),
				 i);
      }
    }
    
    /////////////////////
    //Al mylar Triangle1//
    /////////////////////
    //       e/2+e/2
    //        ___
    //        |.|\.\   ,.
    // hHex   |.| \.\  45*deg
    //        |.|  \.\ ..
    //        ------
    //       e/2+hHex/2+e/2

    
    paramVec.clear();
    paramVec.push_back(tMylar);//Full-length along the z-axis
    paramVec.push_back(0*deg);//Polar angle of the line joining the centres of the faces at -/+pDz
    paramVec.push_back(0*deg);//Azimuthal angle of the line joing the centre of the face at -pDz to the centre of the face at +pDz
    paramVec.push_back(hHex/2.);//Full-length along y of the face at -pDz
    paramVec.push_back(hHex/2.+eps);//Full-length along x of the side at y=-pDy1 of the face at -pDz
    paramVec.push_back(eps);//Full-length along x of the side at y=+pDy1 of the face at -pDz
    paramVec.push_back(atan(1./2.)*rad);//Angle with respect to the y axis from the centre of the side at y=-pDy1 to the centre at y=+pDy1 of the face at -pDz 
    paramVec.push_back(hHex/2.);//Full-length along y of the face at +pDz
    paramVec.push_back(hHex/2.+eps);//Full-length along x of the side at y=-pDy2 of the face at +pDz
    paramVec.push_back(eps);//Full-length along x of the side at y=+pDy2 of the face at +pDz
    paramVec.push_back(atan(1./2.)*rad);//Angle with respect to the y axis from the centre of the side at y=-pDy2 to the centre at y=+pDy2 of the face at +pDz

    double dxT=hHex*3./8.+eps/2.;
    double dyT=hHex*3./8.+tMylar/2.*std::sqrt(3)/2.;
    double dzT=hHex/4.*std::sqrt(3)/2.+tMylar/4.+zlen/2.;
    
    double TriangleShift[8][3] =
      {
	{-dxT, -dyT,-dzT-eps},
	{+dxT, -dyT,-dzT-eps},
	{-dxT, +dyT,-dzT-eps},
	{+dxT, +dyT,-dzT-eps},
	{-dxT, -dyT,+dzT+eps},
	{+dxT, -dyT,+dzT+eps},
	{-dxT, +dyT,+dzT+eps},
	{+dxT, +dyT,+dzT+eps}
      };
    
    double TriangleRot[8][3] = {
      { +60*deg, 180*deg, 0*deg},//-z,-y,-x
      { -60*deg,   0*deg, 0*deg},//-z,-y,+x
      {+120*deg, 180*deg, 0*deg},//-z,+y,-x
      {-120*deg,   0*deg, 0*deg},//-z,+y,+x
      { -60*deg, 180*deg, 0*deg},//+z,-y,-x
      { +60*deg,   0*deg, 0*deg},//+z,-y,+x
      {-120*deg, 180*deg, 0*deg},//+z,+y,-x
      {+120*deg,   0*deg, 0*deg}//+z,+y,+x
    };

    
    GsimTrap* triangle = 0;
    for (int i=0;i<8;i++){
      if(i==0) {
	triangle = new GsimTrap("UCVMylar_triangle1", this,
				G4ThreeVector(TriangleShift[i][0], TriangleShift[i][1], TriangleShift[i][2]),
				G4ThreeVector(TriangleRot[i][0], TriangleRot[i][1], TriangleRot[i][2]));
	triangle->setParameters(paramVec);
	triangle->setOuterMaterial("G4_MYLAR"); 
	triangle->setOuterColor("gray70"); 
	addDaughter(triangle);
	m_detMoveList.push_back(triangle);
      } else {
	triangle->cloneDetector(G4ThreeVector(TriangleShift[i][0], TriangleShift[i][1], TriangleShift[i][2]),
				G4ThreeVector(TriangleRot[i][0], TriangleRot[i][1], TriangleRot[i][2]),
				i);
      }
    }
    setThisAndDaughterBriefName( "UCV" );
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. UCV is not constructed.");
  }
}


GsimE14UCV::~GsimE14UCV()
{
  ;
}

void GsimE14UCV::comment()
{
  if(m_userFlag==202002){
    std::cout << "GsimE14UCV Z position:" << std::endl;
    std::cout << "Plastic scintilation fiber center: z = -607 mm" << std::endl;
  } else if(m_userFlag==202102) {
    std::cout << "GsimE14UCV Z position:" << std::endl;
    std::cout << "Plastic scintilation fiber center: z = -923.5 mm" << std::endl;
  } else if(m_userFlag>=202306) {
    std::cout << "GsimE14UCV Z position:" << std::endl;
    std::cout << "Plastic scintilatior plate center: z = -921.5 mm (-914-7.5)" << std::endl;
  }
}


void GsimE14UCV::setFastSimulationLevel(int level)
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
