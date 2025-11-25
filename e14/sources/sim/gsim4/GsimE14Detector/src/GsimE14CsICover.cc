/**
 *  @file
 *  @brief  GsimE14CsICover
 *  $Id: $ 
 *  $Log: $
 * 
 *
 */
#include "GsimE14Detector/GsimE14CsICover.h"
//#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimPolyhedra2.h"
#include "GsimDetector/GsimTrap.h"
#include "GsimDetector/GsimTube.h"
#include "GsimDetector/GsimBox.h"
#include "GsimKernel/GsimSensitiveDetector.h"
#include "GsimKernel/GsimUtil.h"
#include "GsimPersistency/GsimMessage.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//using namespace E14;

GsimE14CsICover::GsimE14CsICover(std::string name0,
				 GsimDetector* motherDetector,
				 G4ThreeVector transV,G4ThreeVector rotV,
				 int userFlag)
  : GsimDetector(name0,motherDetector,transV,rotV,userFlag)
{
  
  m_className = "GsimE14CsICover";
  if(! (m_userFlag ==201304 ||
	m_userFlag ==201305 ||
	m_userFlag ==201503 ||
	m_userFlag ==201504 ||
	m_userFlag ==201605 ||
    	m_userFlag ==201902 ||
	m_userFlag ==202002 ||
	m_userFlag ==202102 ||
	m_userFlag == 202306)
     ) {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. CsICover is not constructed.");
    return;
  }

  char cname[64] = "";
  if(m_userFlag<=201605){
    //     
    //           208mm
    //       <---------->
    //       39mm
    //     <---->
    //      ------------
    //     |  __________|
    //     | | | |
    //     |_| | |CsI
    //       | | |
    //     <-| | |
    //  12mm | | |
    //      cover|
    //      20mm |

  
    double const BLKT1_Z=-39*mm;
    double const BLKT1_L=12*mm;
    double const BLKT2_L=208*mm;
  
    double const regionOuterRadius = 965*mm;
    double const regionInnerRadius = 120*mm;
    double const outerAlRadius = 895*mm;
    double const honeycombLength = 15*mm;
    double const honeycombThickness = 0.0254*mm;
    double const cfrpThickness = 2.5*mm;
    double const regionLength = cfrpThickness*2 + honeycombLength;//20*mm
    // origin is CsISurface
    double cellSize = 3.175*mm;
  
    // Insert honeycomb
    double const insertHoneycombLength = 10*mm;
    double const insertHoneycombThickness = 0.0508*mm;
    double const insertCellSize = 6.35*mm;
    double const insertCfrpOuterRadius = 150*mm;
    double const insertCfrpInnerRadius = 146.5*mm;
  
    double const dZ=BLKT1_Z+BLKT1_L+regionLength*0.5;//-17mm shift of cover cetner from CsI surface
  
    //cfrp max
    {
      G4ThreeVector trans( 0, 0, regionLength*0.5-cfrpThickness*0.5 );
      double param[5] =  { regionInnerRadius, regionOuterRadius, cfrpThickness, 0, 360*deg };
      std::vector<double> paramVec;
      for(int i=0;i<5;i++) paramVec.push_back(param[i]);
      std::string name = "cfrp0";
      GsimTube* cfrp0 = new GsimTube( name, this, trans+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
      cfrp0->setOuterMaterial( "GsimCFRP" );
      cfrp0->setOuterColor( "yellow" );
      cfrp0->setOuterVisibility( true );
      cfrp0->setParameters( paramVec );
      //cfrp0->setSensitiveDetector("csicover",0);
      addDaughter( cfrp0 );
    
      trans[2] *= -1;
      cfrp0->cloneDetector( trans+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
    }
    // insert cfrp
    {
      G4ThreeVector trans( 0, 0, (insertHoneycombLength+cfrpThickness)*0.5 );
      double param[5] = { regionInnerRadius, insertCfrpOuterRadius, cfrpThickness, 0, 360*deg };
      std::vector<double> paramVec;
      for(int i=0;i<5;i++) paramVec.push_back(param[i]);
      std::string name = "cfrp1";
      GsimTube* cfrp1 = new GsimTube( name, this, trans+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
      cfrp1->setOuterMaterial( "GsimCFRP" );
      cfrp1->setOuterColor( "yellow" );
      cfrp1->setOuterVisibility( true );
      cfrp1->setParameters( paramVec );
      //cfrp1->setSensitiveDetector("csicover",0);
      addDaughter( cfrp1 );
      trans[2] *= -1;
      //cfrp1->cloneDetector( trans, G4ThreeVector(0,0,0), 1 );
      cfrp1->cloneDetector( trans+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
    }
    // inser cfrp cylinder
    {
      double param[5] = { insertCfrpInnerRadius, insertCfrpOuterRadius, insertHoneycombLength, 0, 360*deg };
      std::vector<double> paramVec;
      for(int i=0;i<5;i++) paramVec.push_back(param[i]);
      std::string name = "cfrp2";
      GsimTube* cfrp2 = new GsimTube( name, this, G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
      cfrp2->setOuterMaterial( "GsimCFRP" );
      cfrp2->setOuterColor( "yellow" );
      cfrp2->setOuterVisibility( true );
      cfrp2->setParameters( paramVec );
      //cfrp2->setSensitiveDetector("csicover",0);
      addDaughter( cfrp2 );
    }
  
    // Most out Al parts
    {
      double param[5] = { outerAlRadius, regionOuterRadius, honeycombLength, 0, 360*deg };
      std::vector<double> paramVec;
      for(int i=0;i<5;i++) paramVec.push_back( param[i] );
      std::string name = "OuterAl";
      GsimTube* outerAl = new GsimTube( name, this, G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
      outerAl->setOuterMaterial( "G4_Al" );
      outerAl->setOuterColor( "green" );
      outerAl->setOuterVisibility( true );
      outerAl->setParameters( paramVec );
      //outerAl->setSensitiveDetector("csicover",0);
      addDaughter( outerAl );
    }
  


    //al_honeycomb main
    if(m_userFlag<0) {
      //realistic case --> too heavy
      double const rmin = insertCfrpOuterRadius + cellSize/sqrt(3);
      double const rmax = outerAlRadius - cellSize/sqrt(3);
      //param: phi_start,phi_end,#side,z,inner radius, outer radius
      double param[6]={ 0, 360*deg, 6, honeycombLength, cellSize/2, cellSize/2+honeycombThickness/2. };
      std::vector<double> paramVec;
      for(int i=0;i<6;i++) paramVec.push_back(param[i]);
      std::string name ="alHoneycomb";
    
      G4ThreeVector move[6];
      {
	G4ThreeVector tmp( 0, cellSize+honeycombThickness, 0);
	tmp.rotate( pi/3, G4ThreeVector( 0, 0, 1 ) );
	for(int i=0;i<6;i++){
	  move[i] = tmp.rotate( pi/3, G4ThreeVector( 0, 0, 1) );
	}
      }
    
      int ID = 0;
      int ilayer = 0;
      G4ThreeVector pos = -move[4];
      GsimPolyhedra2* cell = 0;
      while(1){
	pos += move[4];

	if( pos.perp() > rmax*2/sqrt(3) ) break;

	//if( pos.perp() < rmin ){
	for( int iline=0; iline<6; iline++ ){
	  for( int i=0; i<ilayer; i++ ){
	    pos += move[iline];

	    if( pos.perp()<rmin || rmax<pos.perp() ) continue;
	  
	    if( ID==0 ){
	      cell =  new GsimPolyhedra2( "cell", this, pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0));
	      cell->setOuterMaterial( "G4_Al" );
	      cell->setParameters( paramVec );
	      cell->setOuterColor( "green" );
	      //cell->setSensitiveDetector( "csicover", 2 );
	      addDaughter( cell );
	      ID++;
	    }else{
	      //cell->cloneDetector( pos, G4ThreeVector(0,0,0), 2 );
	      cell->cloneDetector( pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
	      ID++;
	    }
	  }
	}
	ilayer++;
      }
    } else {
      // Simplified al_honeycomb
      // Simplified honeycomb thickness is twice as original honeycomb
      double const rmin = insertCfrpOuterRadius + honeycombThickness*2;
      double const rmax = outerAlRadius - honeycombThickness*2;

      double param[3] = { 100*mm, honeycombThickness*2, 5*mm }; // length, thickness, width
      std::vector<double> paramVec;
      for( int i=0; i<3; i++ ){
	paramVec.push_back( param[i] );
      }

      int ID = 0;
      int ilayer = 0;
      double zcenter[3] = { -5*mm, 0*mm, 5*mm };
      G4ThreeVector pos( 0, 0, 0);
      GsimBox* alPlate[300];


      while( true ){

	if( ilayer*cellSize < rmin ){
	  double ycenter = ilayer*cellSize;
	  double xmax = sqrt( pow( rmax, 2) - pow( ycenter, 2) );
	  double xmin = sqrt( pow( rmin, 2) - pow( ycenter, 2) );
	  paramVec[0] = xmax - xmin;


	  for( int xindex=0; xindex<2; xindex++ ){
	    for( int yindex=0; yindex<2; yindex++ ){
	      if( ilayer==0 && yindex==1 ) continue;

	      pos[0] = ( xmax+xmin ) /2. * pow( -1, xindex);
	      pos[1] = ycenter * pow( -1, yindex);

	      for( int zindex=0; zindex<3; zindex++ ){
		pos[2] = zcenter[zindex];
	    
		if( ID==0 ){
		  sprintf( cname, "alplate%d", ilayer);
		  std::string name = std::string(cname);
		  //std::cout << __LINE__ << " " << name << std::endl;
		  alPlate[ilayer] = new GsimBox( name, this, pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
		  alPlate[ilayer]->setOuterMaterial( "G4_Al" );
		  alPlate[ilayer]->setParameters( paramVec );
		  alPlate[ilayer]->setOuterColor( "green" );
		  addDaughter( alPlate[ilayer] );
		  ID++;
		}else{
		  alPlate[ilayer]->cloneDetector( pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,2*pi/3*zindex) );
		  ID++;
		}
		pos.rotate( 2*pi/3, G4ThreeVector(0,0,1) );
	   
	      }
	    }
	  }
	}else if( ilayer*cellSize < rmax ){
	  double ycenter = ilayer * cellSize;
	  paramVec[0] = sqrt( pow( rmax, 2) - pow( ycenter, 2) ) * 2.;
	
	  pos[0] = 0;
	  for( int yindex=0; yindex<2; yindex++ ){
	    pos[1] = ycenter * pow( -1, yindex);
	  
	    for( int zindex=0; zindex<3; zindex++ ){
	      pos[2] = zcenter[zindex];

	      if( ID==0 ){
		sprintf( cname, "alplate%d", ilayer);
		std::string name = std::string(cname);
		//std::cout << __LINE__ << " " << name << std::endl;
		alPlate[ilayer] = new GsimBox( name, this, pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
		alPlate[ilayer]->setOuterMaterial( "G4_Al" );
		alPlate[ilayer]->setParameters( paramVec );
		alPlate[ilayer]->setOuterColor( "green" );
		addDaughter( alPlate[ilayer] );
		ID++;
	      }else{
		alPlate[ilayer]->cloneDetector( pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,2*pi/3*zindex) );
		ID++;
	      }
	      pos.rotate( 2*pi/3, G4ThreeVector(0,0,1) );
	    }
	  }
	}else{
	  break;
	}

	ilayer++;
	ID = 0;
      }
    }

	
    //al_honycomb inserted
    {
      double const rmin = regionInnerRadius + insertCellSize/sqrt(3);
      double const rmax = insertCfrpInnerRadius - insertCellSize/sqrt(3);
      //param: phi_start,phi_end,#side,z,inner radius, outer radius
      double param[6]={ 0, 360*deg, 6, insertHoneycombLength, insertCellSize/2, insertCellSize/2+insertHoneycombThickness/2. };
      std::vector<double> paramVec;
      for(int i=0;i<6;i++) paramVec.push_back(param[i]);
      std::string name ="insertAlHoneycomb";
    
      G4ThreeVector move[6];
      {
	G4ThreeVector tmp( 0, insertCellSize+insertHoneycombThickness, 0);
	tmp.rotate( pi/3, G4ThreeVector( 0, 0, 1 ) );
	for(int i=0;i<6;i++){
	  move[i] = tmp.rotate( pi/3, G4ThreeVector( 0, 0, 1) );
	}
      }
    
      int ID = 0;
      int ilayer = 0;
      G4ThreeVector pos = -move[4];
      GsimPolyhedra2* cell1 = 0;
      while(1){
	pos += move[4];

	if( pos.perp() > rmax*2/sqrt(3) ) break;

	//if( pos.perp() < rmin ){
	for( int iline=0; iline<6; iline++ ){
	  for( int i=0; i<ilayer; i++ ){
	    pos += move[iline];

	    if( pos.perp()<rmin || rmax<pos.perp() ) continue;
	  
	    if( ID==0 ){
	      cell1 =  new GsimPolyhedra2( "cell1", this, pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0));
	      cell1->setOuterMaterial( "G4_Al" );
	      cell1->setParameters( paramVec );
	      cell1->setOuterColor( "green" );
	      //cell1->setSensitiveDetector( "csicover", 2 );
	      addDaughter( cell1 );
	      ID++;
	    }else{
	      //cell->cloneDetector( pos, G4ThreeVector(0,0,0), 2 );
	      cell1->cloneDetector( pos+G4ThreeVector(0,0,dZ), G4ThreeVector(0,0,0) );
	      ID++;
	    }
	  }
	}
	ilayer++;
      }
    }


    {
      char nam[16] = "";
      GsimTube* csc1 =0;
      GsimTube* csc2 =0;
      //Csi Cover BLKT
      //start at 39 mm from CsI surface
      //R917-979  12 in Z
      //R967-979  220-12=208 inZ
  
      std::vector<double> paramCoverVec;
      paramCoverVec.push_back(917*mm);
      paramCoverVec.push_back(979*mm);
      paramCoverVec.push_back(12*mm);
      paramCoverVec.push_back(0.*deg);
      paramCoverVec.push_back(360.*deg);
  
      double cover1Z=
	+0                //CsI surface
	+BLKT1_Z          //BLKT start z
	+BLKT1_L/2.;      //+csc1_length/2.
  
      sprintf(nam,"CSC1");
      csc1 = new GsimTube(std::string(nam),this,
			  G4ThreeVector(0*cm,0*cm,cover1Z),
			  G4ThreeVector(0,0,0));
      csc1->setOuterMaterial("G4_Fe");
      csc1->setParameters(paramCoverVec);
      csc1->setOuterColor("gray70");
      addDaughter(csc1);
  
      paramCoverVec.clear();
      paramCoverVec.push_back(967*mm);
      paramCoverVec.push_back(979*mm);
      paramCoverVec.push_back(208*mm);
      paramCoverVec.push_back(0.*deg);
      paramCoverVec.push_back(360.*deg);
  
      double cover2Z=
	+0               //CsI surface
	+BLKT1_Z         //BLKT start z
	+BLKT1_L         //BLKT 2 start z
	+BLKT2_L/2;      //+csc2 length/2.

  
      sprintf(nam,"CSC2");
      csc2 = new GsimTube(std::string(nam),this,
			  G4ThreeVector(0*cm,0*cm,cover2Z),
			  G4ThreeVector(0,0,0));
      csc2->setOuterMaterial("G4_Fe");
      csc2->setParameters(paramCoverVec);
      csc2->setOuterColor("gray70");
      addDaughter(csc2);
    }
  } else if(m_userFlag>=201902){
    
    //          max 26 mm
    //        <---->
    //      CsI Cover
    //      | U.FL cable
    //      | | Acrylic cover
    //      | | |   Cylinder 
    //  ____' ' ' _|________
    //      : ||_|
    //      : ||_| CsI
    //      : ||_|
    //      : ||_|
    //      : ||_|
    //           |
    //           |

    double const cfrpplateThickness = 2.5*mm;
    double const regionOuterRadius = 965*mm;
    double const regionInnerRadius = 120*mm;
    double const ringInnerRadius = 890*mm;
    double const coverThickness = 30*mm;
		
    double const insertOuterRadius = 150*mm;
    double const insertThickness   = 2.5*mm;
    double const insert2Thickness  = 3.5*mm;
    double const insert2ThicknessZ = 24*mm;
    double const insertWallz       = 9.5*mm; // +/- insertWallZ corresponds to the two inner surfaces of the insert.
	   			
    // honeycomb parameters
    double const honeycombThickness = 0.0254*mm;
    double cellSize = 3.175*mm;
		
    // Offset of the z location.
    // dZ = 0 represents rear surface of CsI cover
    double const dZ= -26.5*mm; 


    // two cfrp plates 
    double param[5] =  { regionInnerRadius, regionOuterRadius, cfrpplateThickness, 0, 360*deg };
    std::vector<double> paramVec;
    for(int i=0;i<5;i++) paramVec.push_back(param[i]);
    std::string name = "cfrp";
    GsimTube* cfrp = new GsimTube( name, this, G4ThreeVector(0,0,dZ-cfrpplateThickness/2.0), G4ThreeVector(0,0,0) );
    cfrp->setOuterMaterial( "GsimCFRP" );
    cfrp->setOuterColor( "green" );
    cfrp->setOuterVisibility( true );
    cfrp->setParameters( paramVec );
    addDaughter( cfrp );
    cfrp->cloneDetector( G4ThreeVector(0,0,dZ-coverThickness+cfrpplateThickness/2), G4ThreeVector(0,0,0) , 1);
	
    // ring cfrp
    double paramring[5] =  { ringInnerRadius, regionOuterRadius, coverThickness-2*cfrpplateThickness, 0, 360*deg };
    paramVec.clear();
    for(int i=0;i<5;i++) paramVec.push_back(paramring[i]);
    name = "cfrpring";
    GsimTube* cfrpring = new GsimTube( name, this, G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,0) );
    cfrpring->setOuterMaterial( "GsimCFRP" );
    cfrpring->setOuterColor( "green" );
    cfrpring->setOuterVisibility( true );
    cfrpring->setParameters( paramVec );
    addDaughter( cfrpring );
	

    // insert2
    //
    //   ||
    //   ||               /\ +z
    //   \/               ||
    //
    //   __________________________
    //   |  ________________________   <- insert 
    //   | |
    //   | |
    //   | |                    ||
    //   | |                    \/
    //   | |________________________
    //   |__________________________   <- insert
    //   /,
    //   -->  <-- 3.5mm            ||  2.5mm


    // insert
    double paraminsert[5] =  { regionInnerRadius, insertOuterRadius-insert2Thickness, insertThickness, 0, 360*deg };
    paramVec.clear();
    for(int i=0;i<5;i++) paramVec.push_back(paraminsert[i]);
    name = "cfrpinsert";
    GsimTube* insert = new GsimTube( name, this, G4ThreeVector(0,0,dZ-coverThickness/2-insertWallz-insertThickness/2), G4ThreeVector(0,0,0) );
    insert->setOuterMaterial( "GsimCFRP" );
    insert->setOuterColor( "blue" );
    insert->setOuterVisibility( true );
    insert->setParameters( paramVec );
    addDaughter( insert );
    insert->cloneDetector( G4ThreeVector(0,0,dZ-coverThickness/2+insertWallz+insertThickness/2), G4ThreeVector(0,0,0) , 1);
	
    // insert2
    double paraminsert2[5] =  { insertOuterRadius-insert2Thickness, insertOuterRadius, insert2ThicknessZ, 0, 360*deg };
    paramVec.clear();
    for(int i=0;i<5;i++) paramVec.push_back(paraminsert2[i]);
    name = "cfrpinsert2";
    GsimTube* insert2 = new GsimTube( name, this, G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,0) );
    insert2->setOuterMaterial( "GsimCFRP" );
    insert2->setOuterColor( "blue" );
    insert2->setOuterVisibility( true );
    insert2->setParameters( paramVec );
    addDaughter( insert2 );
	



		
    // Simplified al_honeycomb 
    {
      // The thickness of the simplified honeycomb is sqrt(3) times as large as that of original honeycomb 
      const double fact_sh = sqrt(3);
      double const rmin = insertOuterRadius + honeycombThickness*fact_sh;
      double const rmax = ringInnerRadius - honeycombThickness*fact_sh;

      double honeycombthicknessZ_outer = coverThickness-2*cfrpplateThickness;
      paramVec.clear();
      for( int i=0; i<3; i++ ){
	paramVec.push_back( 0. );
      }

      paramVec[1] = honeycombThickness*fact_sh;
			
      int ID = 0;
      int ilayer = 0;
      G4ThreeVector pos(0, 0, 0);
      GsimBox* alPlate[300];

      double zcenter[3] = { -honeycombthicknessZ_outer/3, 0, honeycombthicknessZ_outer/3 };
      while( true ){

	if( ilayer*cellSize < rmin ){
			
	  double ycenter = ilayer*cellSize;
	  double xmax = sqrt( pow(rmax,2) - pow(ycenter,2) );
	  double xmin = sqrt( pow(rmin,2) - pow(ycenter,2) );
	  paramVec[0] = xmax - xmin;
	  paramVec[2] = honeycombthicknessZ_outer/3;


	  for( int xindex=0; xindex<2; xindex++ ){
	    for( int yindex=0; yindex<2; yindex++ ){
	      if( ilayer==0 && yindex==1 ) continue;

	      pos[0] = ( xmax+xmin ) /2. * pow( -1, xindex);
	      pos[1] = ycenter * pow( -1, yindex);

	      for( int zindex=0; zindex<3; zindex++ ){
		pos[2] = zcenter[zindex];

		if( ID==0 ){
		  sprintf( cname, "alplate%d", ilayer);
		  std::string name2 = std::string(cname);
		  //std::cout << __LINE__ << " " << name2 << std::endl;
		  alPlate[ilayer] = new GsimBox( name2, this, pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,0) );
		  alPlate[ilayer]->setOuterMaterial( "G4_Al" );
		  alPlate[ilayer]->setParameters( paramVec );
		  alPlate[ilayer]->setOuterColor( "red" );
		  addDaughter( alPlate[ilayer] );
		  ID++;
		}else{
		  alPlate[ilayer]->cloneDetector( pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,2*pi/3*zindex) );
		  ID++;
		}
		pos.rotate( 2*pi/3, G4ThreeVector(0,0,1) );

	      }
	    }
	  }
	}else if( ilayer*cellSize < rmax ){
					
	  double ycenter = ilayer * cellSize;
	  paramVec[0] = sqrt( pow( rmax, 2) - pow( ycenter, 2) ) * 2.;
	  paramVec[2] = honeycombthicknessZ_outer/3;

	  pos[0] = 0;
	  for( int yindex=0; yindex<2; yindex++ ){
	    pos[1] = ycenter * pow( -1, yindex);

	    for( int zindex=0; zindex<3; zindex++ ){
	      pos[2] = zcenter[zindex];

	      if( ID==0 ){
		sprintf( cname, "alplate%d", ilayer);
		std::string name3 = std::string(cname);
		//std::cout << __LINE__ << " " << name3 << std::endl;
		alPlate[ilayer] = new GsimBox( name3, this, pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,0) );
		alPlate[ilayer]->setOuterMaterial( "G4_Al" );
		alPlate[ilayer]->setParameters( paramVec );
		alPlate[ilayer]->setOuterColor("red");
		addDaughter( alPlate[ilayer] );
		ID++;
	      }else{
		alPlate[ilayer]->cloneDetector( pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,2*pi/3*zindex) );
		ID++;
	      }
	      pos.rotate( 2*pi/3, G4ThreeVector(0,0,1) );
	    }
	  }
	}else{
	  break;
	}

	ilayer++;
	ID = 0;
      }
    }

    // Simplified al_honeycomb in insert 
    {
      // The thickness of the simplified honeycomb is sqrt(3) times as large as that of original honeycomb 
      const double fact_sh = sqrt(3);
      double const rmin = regionInnerRadius + honeycombThickness*fact_sh;
      double const rmax = insertOuterRadius - insert2Thickness - honeycombThickness*fact_sh;

      double honeycombthicknessZ_inner = insertWallz*2;
      paramVec.clear();
      for( int i=0; i<3; i++ ){
	paramVec.push_back( 0 );
      }

      paramVec[1] = honeycombThickness*fact_sh;
			
      int ID = 0;
      int ilayer = 0;
      G4ThreeVector pos(0, 0, 0);
      GsimBox* alPlate[300];

      double zcenter[3] = { -honeycombthicknessZ_inner/3, 0, honeycombthicknessZ_inner/3 };
      while( true ){

	if( ilayer*cellSize < rmin ){
			
	  double ycenter = ilayer*cellSize;
	  double xmax = sqrt( pow(rmax,2) - pow(ycenter,2) );
	  double xmin = sqrt( pow(rmin,2) - pow(ycenter,2) );
	  paramVec[0] = xmax - xmin;
	  paramVec[2] = honeycombthicknessZ_inner/3;


	  for( int xindex=0; xindex<2; xindex++ ){
	    for( int yindex=0; yindex<2; yindex++ ){
	      if( ilayer==0 && yindex==1 ) continue;

	      pos[0] = ( xmax+xmin ) /2. * pow( -1, xindex);
	      pos[1] = ycenter * pow( -1, yindex);

	      for( int zindex=0; zindex<3; zindex++ ){
		pos[2] = zcenter[zindex];

		if( ID==0 ){
		  sprintf( cname, "alplate_insert%d", ilayer);
		  std::string name4 = std::string(cname);
		  //std::cout << __LINE__ << " " << name4 << std::endl;
		  alPlate[ilayer] = new GsimBox( name4, this, pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,0) );
		  alPlate[ilayer]->setOuterMaterial( "G4_Al" );
		  alPlate[ilayer]->setParameters( paramVec );
		  alPlate[ilayer]->setOuterColor( "red" );
		  addDaughter( alPlate[ilayer] );
		  ID++;
		}else{
		  alPlate[ilayer]->cloneDetector( pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,2*pi/3*zindex) );
		  ID++;
		}
		pos.rotate( 2*pi/3, G4ThreeVector(0,0,1) );

	      }
	    }
	  }
	}else if( ilayer*cellSize < rmax ){
					
	  double ycenter = ilayer * cellSize;
	  paramVec[0] = sqrt( pow( rmax, 2) - pow( ycenter, 2) ) * 2.;
	  paramVec[2] = honeycombthicknessZ_inner/3;

	  pos[0] = 0;
	  for( int yindex=0; yindex<2; yindex++ ){
	    pos[1] = ycenter * pow( -1, yindex);

	    for( int zindex=0; zindex<3; zindex++ ){
	      pos[2] = zcenter[zindex];

	      if( ID==0 ){
		sprintf( cname, "alplate_insert%d", ilayer);
		std::string name5 = std::string(cname);
		//std::cout << __LINE__ << " " << name5 << std::endl;
		alPlate[ilayer] = new GsimBox( name5, this, pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,0) );
		alPlate[ilayer]->setOuterMaterial( "G4_Al" );
		alPlate[ilayer]->setParameters( paramVec );
		alPlate[ilayer]->setOuterColor("red");
		addDaughter( alPlate[ilayer] );
		ID++;
	      }else{
		alPlate[ilayer]->cloneDetector( pos+G4ThreeVector(0,0,dZ-coverThickness/2), G4ThreeVector(0,0,2*pi/3*zindex) );
		ID++;
	      }
	      pos.rotate( 2*pi/3, G4ThreeVector(0,0,1) );
	    }
	  }
	}else{
	  break;
	}

	ilayer++;
	ID = 0;
      }
    }


    double const BLKTLength    = 303*mm;
    double const BLKTWidth     = 50*mm;
    double const BLKTThickness = 3*mm;
    double const BLKTR = regionOuterRadius + BLKTThickness/2;
    double const BLKTZoffset   = dZ - coverThickness + BLKTLength/2;

    std::vector<double> paramBLKT;
    paramBLKT.push_back(BLKTThickness);
    paramBLKT.push_back(BLKTWidth);
    paramBLKT.push_back(BLKTLength);

    // place 32 blackets		
    GsimBox* boxBLKT = 0;
    for(int i=0;i<32;i++){

      double BLKTang = 2*pi/32*(i+0.5);

      double x = BLKTR*cos(BLKTang);
      double y = BLKTR*sin(BLKTang);

      if(i==0){
	boxBLKT = new GsimBox("BLKT", this, G4ThreeVector(x,y,BLKTZoffset), G4ThreeVector(0, 0, BLKTang*radian));
	boxBLKT->setOuterMaterial("G4_Al");
	boxBLKT->setParameters(paramBLKT);
	boxBLKT->setOuterColor("gray75");
	addDaughter(boxBLKT);
      }else{
	boxBLKT->cloneDetector(G4ThreeVector(x,y,BLKTZoffset), G4ThreeVector(0, 0, BLKTang*radian), i);
      }
		
    }

		
  }


    
  setThisAndDaughterBriefName("CsICover");
  m_detMoveList.push_back(this);
}


GsimE14CsICover::~GsimE14CsICover()
{
  ;
}

void GsimE14CsICover::comment()
{
  std::cout << "GsimE14CsICover Z position:" << std::endl;
  std::cout << "    Origin is the upstream surface of CsI." << std::endl;
  std::cout << "    Place it 614.8 cm (2013.6)" << std::endl;
  std::cout << "    Place it (614.8+2-0.58) cm (201605)" << std::endl;
  std::cout << "    Place it (614.8+2) cm (201902)" << std::endl;

  std::cout << "Aluminum honeycomb is simplified using just plates." << std::endl;
  std::cout << "If you assign negative usrFlag realistic honeycomb is created." << std::endl;
  std::cout << "It is too heavy and too slow." << std::endl;  
}

void GsimE14CsICover::setFastSimulationLevel(int level)
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
