/**
 * Coded by Toru Matsumura
 * 2017 Oct
 */
#include "GsimE14Detector/GsimE14CsIBackPlate.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimTrap.h"
#include "GsimDetector/GsimTube.h"
#include "GsimDetector/GsimTubeWithAHole.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14CsIBackPlate::GsimE14CsIBackPlate(std::string name,
					 GsimDetector* motherDetector,
					 G4ThreeVector transV,G4ThreeVector rotV,
					 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  
  m_className = "GsimE14CsIBackPlate";

  if(
     ! (userFlag == 201212 ||
	userFlag == 201301 ||
	userFlag == 201303 ||
	userFlag == 201304 ||
	userFlag == 201305 ||
	userFlag == 201503 ||
	userFlag == 201504 ||
	userFlag == 201605 ||
	userFlag == 201902 ||
	userFlag == 202002 ||
	userFlag == 202102 ||
	userFlag == 202306
	)
     ) {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. CSI back plate is not constructed.");
    return;
  }
  
  std::vector<double> paramVec;
  double x, y;
  
  // This is CsI PMT holder plate (Fe 60 mmt) designed by Togawa-san 

  GsimTubeWithAHole *backplate = new GsimTubeWithAHole("CsI_backplate",this,
		  		 G4ThreeVector(0,0,0),G4ThreeVector(0,0,0));
  m_detMoveList.push_back(backplate);

  //paramVec.push_back( 1886.*mm/2 ); // increased by 1mm below to fix geometry overlap.
  paramVec.push_back( 1887.*mm/2 ); // Radius of the Fe back plate
  paramVec.push_back( 60.*mm ); // Thickness of the Fe back plate
  paramVec.push_back( 0.*mm ); //  X position of the hole
  paramVec.push_back( 0.*mm ); //  Y position of the hole
  paramVec.push_back( 199.*mm ); // X hole size of the back plate
  paramVec.push_back( 199.*mm ); // Y hole size of the back plate

  backplate->setParameters( paramVec );
  backplate->setOuterMaterial("G4_Fe");
  backplate->setOuterVisibility(true);
  backplate->setOuterColor("yellow");
  addDaughter( backplate );

  GsimTube *hole = 0;
  paramVec.clear();
  paramVec.push_back( 0.*mm );    // Rin
  paramVec.push_back( 23./2*mm ); // Rout
  paramVec.push_back( 60.*mm );   // length
  paramVec.push_back( 0.*rad );     // start angle
  paramVec.push_back( 2.*M_PI*rad );// opening angle 


  // PMT Holes for the small CsI crystals
  int id = 0;
  for( int ix=0; ix<48; ix++ ){
    for( int iy=0; iy<48; iy++ ){
      x = double( ix ) *25.05*mm - (25.05*mm*23.5);
      y = double( iy ) *25.05*mm - (25.05*mm*23.5);
      if( fabs(x)<100*mm && fabs(y)<100*mm ) continue; // beam hole region
      if( hole==0 ){
	hole = new GsimTube("CsI_backplate_hole", backplate,
			    G4ThreeVector( x, y, 0 ),
			    G4ThreeVector(0,0,0) );
	hole->setParameters( paramVec );
	hole->setOuterMaterial("G4_Galactic");
	hole->setOuterVisibility(true);
	hole->setOuterColor("yellow");
	backplate->addDaughter( hole );
      }else{
	hole->cloneDetector( G4ThreeVector( x, y, 0 ),
			     G4ThreeVector(0,0,0), id );
      }
      id++;
    }
  }

  // PMT Holes for the large CsI crystals
  
  id = 0;
  GsimTube *Lhole = 0;
  paramVec.clear();
  paramVec.push_back( 0.*mm );    // Rin
  paramVec.push_back( 45.6/2*mm ); // Rout
  paramVec.push_back( 60.*mm );   // length
  paramVec.push_back( 0.*rad );     // start angle
  paramVec.push_back( 2.*M_PI*rad );// opening angle 

  int nLhole[36] = {6,8,10,11,12,13,2,3,4,4,5,5,6,6,6,6,6,6,
                    6,6,6,6,6,6,5,5,4,4,3,2,13,12,11,10,8,6};
  for( int iy=0; iy<36; iy++ ){
    for( int ix=0; ix<nLhole[iy]; ix++ ){
      if( iy < 6 || iy > 29 ){ 
	x = double( ix ) *50.10*mm + (50.10*mm*0.5);
      }else{
	x = double( ix+12 ) *50.10*mm + (50.10*mm*0.5);
      }
      y = double( iy ) *50.10*mm - (50.10*mm*17.5);

      if( Lhole==0 ){
	Lhole = new GsimTube("CsI_backplate_large_hole", backplate,
			    G4ThreeVector( x, y, 0 ),
			    G4ThreeVector(0,0,0) );
	Lhole->setParameters( paramVec );
	Lhole->setOuterMaterial("G4_Galactic");
	Lhole->setOuterVisibility(true);
	Lhole->setOuterColor("yellow");
	backplate->addDaughter( Lhole );
	id++;
      }else{
	Lhole->cloneDetector( G4ThreeVector( x, y, 0 ),
			     G4ThreeVector(0,0,0), id++ );
      }
      Lhole->cloneDetector( G4ThreeVector( -x, y, 0 ),
			   G4ThreeVector(0,0,0), id++ );
    }
  }

  // Holes for the OEV counters
  
  id = 0;
  GsimTrap* oevhole[8];
  for( int i=0; i<8; i++ ){
    oevhole[i] = 0;
  }
  // for TYPE#0
  std::vector<double> paramVecOEV[8];

  paramVecOEV[0].push_back(60.*mm);         // pDz
  paramVecOEV[0].push_back(0*deg);           // pTheta
  paramVecOEV[0].push_back(0*deg);           // pPhi
  paramVecOEV[0].push_back(79.0*mm);         // pDy1
  paramVecOEV[0].push_back(30.9*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[0].push_back(26.7*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[0].push_back(1.52*deg); // pAlp1
  paramVecOEV[0].push_back(79.0*mm);         // pDy2
  paramVecOEV[0].push_back(30.9*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[0].push_back(26.7*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[0].push_back(1.52*deg); // pAlp2

  paramVecOEV[1].push_back(60.*mm);         // pDz
  paramVecOEV[1].push_back(0*deg);           // pTheta
  paramVecOEV[1].push_back(0*deg);           // pPhi
  paramVecOEV[1].push_back(144.0*mm);         // pDy1
  paramVecOEV[1].push_back(28.5*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[1].push_back(1e-6*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[1].push_back(5.65*deg); // pAlp1
  paramVecOEV[1].push_back(144.0*mm);         // pDy2
  paramVecOEV[1].push_back(28.5*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[1].push_back(1e-6*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[1].push_back(5.65*deg); // pAlp2

  paramVecOEV[2].push_back(60.*mm);         // pDz
  paramVecOEV[2].push_back(0*deg);           // pTheta
  paramVecOEV[2].push_back(0*deg);           // pPhi
  paramVecOEV[2].push_back(71.4*mm);         // pDy1
  paramVecOEV[2].push_back(28.2*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[2].push_back(1e-6*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[2].push_back(11.19*deg); // pAlp1
  paramVecOEV[2].push_back(71.4*mm);         // pDy2
  paramVecOEV[2].push_back(28.2*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[2].push_back(1e-6*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[2].push_back(11.19*deg); // pAlp2

  paramVecOEV[3].push_back(60.*mm);         // pDz
  paramVecOEV[3].push_back(0*deg);           // pTheta
  paramVecOEV[3].push_back(0*deg);           // pPhi
  paramVecOEV[3].push_back(68.5*mm);         // pDy1
  paramVecOEV[3].push_back(36.7*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[3].push_back(1e-6*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[3].push_back(15.01*deg); // pAlp1
  paramVecOEV[3].push_back(68.5*mm);         // pDy2
  paramVecOEV[3].push_back(36.7*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[3].push_back(1e-6*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[3].push_back(15.01*deg); // pAlp2

  paramVecOEV[4].push_back(60.*mm);         // pDz
  paramVecOEV[4].push_back(0*deg);           // pTheta
  paramVecOEV[4].push_back(0*deg);           // pPhi
  paramVecOEV[4].push_back(44.5*mm);         // pDy1
  paramVecOEV[4].push_back(30.6*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[4].push_back(1e-6*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[4].push_back(18.93*deg); // pAlp1
  paramVecOEV[4].push_back(44.5*mm);         // pDy2
  paramVecOEV[4].push_back(30.6*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[4].push_back(1e-6*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[4].push_back(18.93*deg); // pAlp2

  paramVecOEV[5].push_back(60.*mm);         // pDz
  paramVecOEV[5].push_back(0*deg);           // pTheta
  paramVecOEV[5].push_back(0*deg);           // pPhi
  paramVecOEV[5].push_back(30.0*mm);         // pDy1
  paramVecOEV[5].push_back(57.3*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[5].push_back(21.0*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[5].push_back(31.20*deg); // pAlp1
  paramVecOEV[5].push_back(30.0*mm);         // pDy2
  paramVecOEV[5].push_back(57.3*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[5].push_back(21.0*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[5].push_back(31.20*deg); // pAlp2

  paramVecOEV[6].push_back(60.*mm);         // pDz
  paramVecOEV[6].push_back(0*deg);           // pTheta
  paramVecOEV[6].push_back(0*deg);           // pPhi
  paramVecOEV[6].push_back(30.0*mm);         // pDy1
  paramVecOEV[6].push_back(59.5*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[6].push_back(28.6*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[6].push_back(27.29*deg); // pAlp1
  paramVecOEV[6].push_back(30.0*mm);         // pDy2
  paramVecOEV[6].push_back(59.5*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[6].push_back(28.6*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[6].push_back(27.29*deg); // pAlp2

  paramVecOEV[7].push_back(60.*mm);         // pDz
  paramVecOEV[7].push_back(0*deg);           // pTheta
  paramVecOEV[7].push_back(0*deg);           // pPhi
  paramVecOEV[7].push_back(30.0*mm);         // pDy1
  paramVecOEV[7].push_back(30.0*mm);        // pDx1(y=-pDy1 of the face at -pDz)
  paramVecOEV[7].push_back(30.0*mm);        // pDx2(y=+pDy1 of the face at -pDz)
  paramVecOEV[7].push_back(0.*deg); // pAlp1
  paramVecOEV[7].push_back(30.0*mm);         // pDy2
  paramVecOEV[7].push_back(30.0*mm);        // pDx3(y=-pDy2 of the face at +pDz)
  paramVecOEV[7].push_back(30.0*mm);        // pDx4(y=+pDy2 of the face at +pDz)
  paramVecOEV[7].push_back(0.*deg); // pAlp2

  std::vector<double> paramXoffset;
  std::vector<double> paramYoffset;
  paramXoffset.push_back(  50.0*mm );
  paramXoffset.push_back( 182.4*mm );
  paramXoffset.push_back( 346.6*mm );
  paramXoffset.push_back( 445.1*mm );
  paramXoffset.push_back( 533.2*mm );
  paramXoffset.push_back( 580.5*mm );
  paramXoffset.push_back( 634.0*mm );
  paramXoffset.push_back( 676.0*mm );

  paramYoffset.push_back( -926.4*mm );
  paramYoffset.push_back( -915.1*mm );
  paramYoffset.push_back( -869.1*mm );
  paramYoffset.push_back( -821.2*mm );
  paramYoffset.push_back( -769.6*mm );
  paramYoffset.push_back( -727.0*mm );
  paramYoffset.push_back( -677.0*mm );
  paramYoffset.push_back( -626.0*mm );

  for( int itype = 0; itype<8; itype++){
    for( int k=0; k<8; k++ ){
      // k = 0(BL): normal image
      // k = 1(BR): mirror image
      // k = 2(TL): normal image
      // k = 3(TR): mirror image
      // k = 4(RU): normal image
      // k = 5(LU): mirror image
      // k = 6(RD): normal image
      // k = 7(LD): mirror image    
      
      if( k >= 4 && itype == 6 )continue;
      if( k >= 4 && itype == 7 )continue;

      //rotation vector for Type#0-#5 (itype<=4)
      G4ThreeVector rot0( 0.*deg,180.*deg,-90.*deg ); 
      //rotation vector for Type#6-#8
      G4ThreeVector rot1( 0.*deg,  0.*deg,180.*deg ); 

      double x_offset = paramXoffset.at( itype );
      double y_offset = paramYoffset.at( itype );

      if( k >= 4 ){
        double dtmp = x_offset;
        x_offset = y_offset;
        y_offset = dtmp;
      }

      switch( k%4 ){
      case 0:
        break;
      case 1:
        x_offset = - x_offset;
        break;
      case 2:
        y_offset = - y_offset; 
        break;
      case 3:
        x_offset = - x_offset;
        y_offset = - y_offset; 
        break;
      }

     switch( k ){
      case 0:
        break;
      case 1:
        rot0.set( 0.*deg,0.*deg,90.*deg );
        rot1.set( 0.,180.*deg,180.*deg );
        break;
      case 2:
        rot0.set( 0.*deg,0.*deg,-90.*deg );
        rot1.set( 180.*deg,0.*deg,180.*deg );
        break;
      case 3:
        rot0.set( 0.*deg,180.*deg,90.*deg );
        rot1.set( 180.*deg,180.*deg,180.*deg );
        break;
      case 4:
        rot0.set( 0.*deg,0.*deg,0.*deg );
	rot1.set( 0.,180.*deg,90.*deg );
        break;
      case 5:
        rot0.set( 0.*deg,180.*deg,0.*deg );
	rot1.set( 0.,0.*deg,-90.*deg );
        break;
      case 6:
        rot0.set( 0.*deg,180.*deg,180.*deg );
        rot1.set( 0.,0.*deg,90.*deg );
        break;
      case 7:
        rot0.set( 0.*deg,0.*deg,180.*deg );
        rot1.set( 0.,180.*deg,-90.*deg );
        break;
      } 

      if( k==0 ){
	if( itype <= 4 ){
	  oevhole[itype] = new GsimTrap(Form("OEV_hole_%d",itype), backplate, 
					G4ThreeVector(x_offset,y_offset,0),rot0);
	}else{
	  oevhole[itype] = new GsimTrap(Form("OEV_hole_%d",itype), backplate, 
					G4ThreeVector(x_offset,y_offset,0),rot1);
	}
	oevhole[itype]->setParameters( paramVecOEV[itype] );
	oevhole[itype]->setOuterColor("yellow");
	oevhole[itype]->setOuterMaterial("G4_Galactic");
	oevhole[itype]->setOuterVisibility(true);
	backplate->addDaughter( oevhole[itype] );
      }else{
	if( itype <= 4 ){
	  oevhole[itype]->cloneDetector( G4ThreeVector( x_offset, y_offset, 0 ),
					 rot0 , id++ );
	}else{
	  oevhole[itype]->cloneDetector( G4ThreeVector( x_offset, y_offset, 0 ),
					 rot1 , id++ );
	}
      }
    }
    
  }

  // -----------------------------------------------------------------
  // Following part is for the CC03 PMT holder plate (POM 60 mmt) 
  // designed by Togawa-san 
  // -----------------------------------------------------------------

  GsimPolyhedra2 *CC03backplate = 
    new GsimPolyhedra2("CC03_backplate",this,
		       G4ThreeVector(0,0,0),G4ThreeVector(0,0,0));
  m_detMoveList.push_back(CC03backplate);

  double CC03backplate_length = 60.*mm;
  double CC03backplate_innerWidth = 160.*mm;
  double CC03backplate_outerWidth = 198.8*mm;
  
  paramVec.clear();
  paramVec.push_back( 45.*deg );      // startPhi
  paramVec.push_back( 360.*deg );    // openingAngle
  paramVec.push_back( 4 );           // numSide
  paramVec.push_back( CC03backplate_length );        // Z length
  paramVec.push_back( CC03backplate_innerWidth/2. ); // rInner
  paramVec.push_back( CC03backplate_outerWidth/2. ); // rOuter

  CC03backplate->setParameters( paramVec );
  CC03backplate->setOuterMaterial("G4_POLYOXYMETHYLENE");
  CC03backplate->setOuterVisibility(true);
  CC03backplate->setOuterColor("white");
  addDaughter( CC03backplate );

  // CC03 backplate holes (32 holes, D=16mm)

  GsimTube *CC03hole = 0;
  paramVec.clear();
  paramVec.push_back( 0.*mm );    // Rin
  paramVec.push_back( 16./2.*mm ); // Rout
  paramVec.push_back( 60.*mm );   // length
  paramVec.push_back( 0.*rad );     // start angle
  paramVec.push_back( 2.*M_PI*rad );// opening angle 

  id = 0;
  for(id=0; id<32; id++ ){
    int i, j;
    double d;
    i = id%8;
    j = id/8;

    if( i == 0 ) d = 0.;

    switch( j ){
    case 0: 
      x = -CC03backplate_outerWidth/2. + 10.8*mm + d;
      y = -CC03backplate_outerWidth/2. +  9.4*mm;
      break;
    case 1: 
      x = +CC03backplate_outerWidth/2. -  9.4*mm;
      y = -CC03backplate_outerWidth/2. + 10.8*mm + d;
      break;
    case 2: 
      x = +CC03backplate_outerWidth/2. - 10.8*mm - d;
      y = +CC03backplate_outerWidth/2. -  9.4*mm;
      break;
    case 3: 
      x = -CC03backplate_outerWidth/2. +  9.4*mm;
      y = +CC03backplate_outerWidth/2. - 10.8*mm - d;
      break;
    }
	
    if( CC03hole==0 ){
      CC03hole = new GsimTube("CC03_backplate_hole", CC03backplate,
			      G4ThreeVector( x, y, 0 ),
			      G4ThreeVector(0,0,0) );
      CC03hole->setParameters( paramVec );
      CC03hole->setOuterMaterial("G4_Galactic");
      CC03hole->setOuterVisibility(true);
      CC03hole->setOuterColor("white");
      CC03backplate->addDaughter( CC03hole );
    }else{
      CC03hole->cloneDetector( G4ThreeVector( x, y, 0 ),
			       G4ThreeVector(0,0,0), id );
    }

    if( i%2 == 0 ){
      d += 22.4*mm;
    }else{
      d += 23.1*mm;
    }

  }

  setThisAndDaughterBriefName("CsIBackPlate");

}


GsimE14CsIBackPlate::~GsimE14CsIBackPlate()
{
  ;
}

void GsimE14CsIBackPlate::comment()
{
  std::cout << "GsimE14CsIBackPlate Z position:" << std::endl;
  std::cout << "    Place at Downstream end of CSI + 6/2 cm" << std::endl;
  std::cout << "    When the upstream end is 614.8 cm," << std::endl;
  std::cout << "    z=(614.8+50.+6./2.)*cm. " << std::endl;
  std::cout << "When userFlag is >=201605, place it downstream by 2 cm."  << std::endl;
  std::cout << "       z=(614.8+50.+6./2.+2)*cm" << std::endl;
}


void GsimE14CsIBackPlate::setFastSimulationLevel(int level)
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
