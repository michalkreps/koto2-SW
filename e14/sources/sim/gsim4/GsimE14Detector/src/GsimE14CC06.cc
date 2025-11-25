/**
 *  @file
 *  @brief  GsimE14CC06
 *  $Id: GsimE14CC06.cc,v 1.3 2006/12/06 19:04:45 nanjo Exp $ 
 *  $Log: GsimE14CC06.cc,v $
 *  Revision 1.3  2006/12/06 19:04:45  nanjo
 *  CVS variables.
 *
 *
 */
#include "GsimE14Detector/GsimE14CC06.h"
#include "GsimE14Detector/GsimE14UserGeom.h"
#include "GsimDetector/GsimBox.h"
#include "GsimDetector/GsimBoxWithoutABox.h"
#include "GsimPersistency/GsimMessage.h"

#include "G4SystemOfUnits.hh"

using namespace E14;

GsimE14CC06::GsimE14CC06(std::string name,
			 GsimDetector* motherDetector,
			 G4ThreeVector transV,G4ThreeVector rotV,
			 int userFlag)
  : GsimDetector(name,motherDetector,transV,rotV,userFlag)
{
  
  m_className = "GsimE14CC06";
  // CC06
  if(m_userFlag==201304 ||
     m_userFlag==201305 ||
     m_userFlag==201503 ||
     m_userFlag==201504 ||
     m_userFlag==201605 ||
     m_userFlag==201902 ||
     m_userFlag==202002 ||
     m_userFlag==202102 ||
     m_userFlag==202306
     ) {
    
    
    bool isDebugMode=false;
    
    //Takashima's geometry
    /*                   Beam hole size : 180 x 180 mm
  		       
  		       Y axis  
                                    :                                nomal
                          zone 2    :    zone 1                     ______________    
                      ______________:______________             70 /_____________/|
                     |______2_______|______2_______|           70 |______________|/
                     |______1_______|______1_______|                    300
                 ____|______0_______|______0_______|____
     zone3      |_______0_cut_ |    :    |______0_cut__ |   zone 0
   <------------|_______1______|----:----|______1_______|------------------
                |_______2_cut_ |____:____|______2_cut__ |            
                     |_______0______|______0_______|                 ______________
                     |_______1______|______1_______|             55 /_____________/|              
                     |_______2______|______2_______|            55 |_____________|/   
                                    :                                 300
                         zone 4     :      zone5                    cutting crystal
                                    : 
  				  
  				  zone 0 or 3 , num 0 & 2
  				  
                           # Plastic Scintilator	  
                           k = 3 , j = 0
			
                _______________________________________                                                 
               |         10t x 210 x 600 mm   i=3      |                                                
               |          center : x=0, y=210          |                                                
              _|_______________________________________|_                                               
             | 10t*230*300 mm |         | 10t*230*300 mm |                                              
             |       i=1      |         |       i=2      |                                              
             |__ x= 255, y=0 _!_________!_ x=-255, y=0 __|                                              
             |         10t x 210 x 600 mm            |                                                
             |               i=0                     |                                                
             |____________ x=0, y=-210 ______________|      

                      # acryl plate
                        zone 1,2,4,5 : 69.5 x 69.5 x 2 mm
                        zone 0,3     : 69.5 x 69.5 x 1 mm
    */


    //const int modN = 18;
    const int I=6, J=3, K=4;
  
    char nam[250] = "";
    char nam2[250] = "";
    char nam3[250] = "";
    char nam4[999] = "";
  
    std::vector<double> paramVec[I][J][K];
    std::vector<double> paramVec2[I][J][K];
    std::vector<double> paramVec3[I][J][K];
    std::vector<double> paramVec4[4][I][J][K];
  
    GsimBox* box[I][J][K];
    GsimBox* box2[I][J][K];
    GsimBox* box3[I][J][K];
    GsimBox* box4[4][I][J][K];
  
    int modID[I][J][K];
    int acryl_ID[I][J][K];
    int perm_ID[4][I][J][K];
  
    // CsI crystal
    double cr_size_x[I][J][K], cr_size_y[I][J][K], cr_size_z[I][J][K];
    double cr_pos_x[I][J][K],  cr_pos_y[I][J][K],  cr_pos_z[I][J][K];
    double cr_rotA[I][J][K],   cr_rotB[I][J][K],   cr_rotC[I][J][K];
  
    // CsI crystal (step)
    double cr_size_x2[I][J][K], cr_size_y2[I][J][K], cr_size_z2[I][J][K];
    double cr_pos_x2[I][J][K],  cr_pos_y2[I][J][K],  cr_pos_z2[I][J][K];
    double cr_rotA2[I][J][K],   cr_rotB2[I][J][K],   cr_rotC2[I][J][K];
  
    // Acryl plate
    double acryl_size_x[I][J][K], acryl_size_y[I][J][K], acryl_size_z[I][J][K];
    double acryl_pos_x[I][J][K],  acryl_pos_y[I][J][K],  acryl_pos_z[I][J][K];
    double acryl_rotA[I][J][K],   acryl_rotB[I][J][K],   acryl_rotC[I][J][K];
  
    // Permalloy
    double perm_size_x[4][I][J][K], perm_size_y[4][I][J][K], perm_size_z[4][I][J][K];
    double perm_pos_x[4][I][J][K],  perm_pos_y[4][I][J][K],  perm_pos_z[4][I][J][K];
    double perm_rotA[4][I][J][K],   perm_rotB[4][I][J][K],   perm_rotC[4][I][J][K];
  
  
    //================================================
    // Mean of CsI crystal size y,z
    double csi_size = 70.06;
    double cut_csi_size = 55;
  
    // Beam hole size
    double hole_size = 181;
  
    // Step of CsI crystal 
    double csi_step = 1.5;

    // Permalloy
    double perm_thickness = 1.0;
  
  
    for(int i=0; i<I; i++){
      for(int j=0; j<J; j++){
	for(int k=0; k<K; k++){
	
	  // CsI crystal
	  if( k<3 ){
	    if( ( i==0 || i==3 ) && j%2==0 ){
	      cr_size_x[i][j][k] = 200;
	      cr_size_y[i][j][k] = cut_csi_size;
	      cr_size_z[i][j][k] = csi_size;
	    
	      cr_size_x2[i][j][k] = 100;
	      cr_size_y2[i][j][k] = cut_csi_size;
	      cr_size_z2[i][j][k] = csi_size - 2*csi_step;
	    }
	  
	    else{
	      cr_size_x[i][j][k] = 200;
	      cr_size_y[i][j][k] = csi_size;
	      cr_size_z[i][j][k] = csi_size;
	    
	      cr_size_x2[i][j][k] = 100;
	      cr_size_y2[i][j][k] = csi_size - 2*csi_step;
	      cr_size_z2[i][j][k] = csi_size - 2*csi_step;
	    }
	  
	  } // k<3
	
	  // Large plastic scintilator
	  if( k==3 && ( i==0 || i==3 ) ){
	    cr_size_x[i][j][k] = 600;
	    cr_size_y[i][j][k] = 210;
	    cr_size_z[i][j][k] = 10;
	  }
	
	  // Small plastic scintilator
	  if( k==3 && ( i==1 || i==2 ) ){
	    cr_size_x[i][j][k] = 300;
	    cr_size_y[i][j][k] = 230;
	    cr_size_z[i][j][k] = 10;
	  }
	
	
	  // Acryl
	  if( k<3 ){
	    if( (i==0 || i==3 ) && j%2==0 ){
	      acryl_size_x[i][j][k] = 0;
	      acryl_size_y[i][j][k] = 0;
	      acryl_size_z[i][j][k] = 0;
	    } else {
	      acryl_size_x[i][j][k] = 1.0;
	      acryl_size_y[i][j][k] = 69.5;
	      acryl_size_z[i][j][k] = 69.5;
	    }
	  }
	
	
	  // Permalloy
	  if( k<3 ){
	    if( (i==0 || i==3) && j%2==0 ){
	      for(int l=0; l<4; l++){
		perm_size_x[l][i][j][k] = 0;
		perm_size_y[l][i][j][k] = 0;
		perm_size_z[l][i][j][k] = 0;
	      }
	    } else {  
	      perm_size_x[0][i][j][k] = 100;
	      perm_size_y[0][i][j][k] = cr_size_y2[i][j][k] + perm_thickness*2;
	      perm_size_z[0][i][j][k] = perm_thickness;
	    
	      perm_size_x[1][i][j][k] = 100;
	      perm_size_y[1][i][j][k] = perm_thickness;
	      perm_size_z[1][i][j][k] = cr_size_y2[i][j][k];
	    
	      perm_size_x[2][i][j][k] = 100;
	      perm_size_y[2][i][j][k] = cr_size_y2[i][j][k] + perm_thickness*2;
	      perm_size_z[2][i][j][k] = perm_thickness;
	    
	      perm_size_x[3][i][j][k] = 100;
	      perm_size_y[3][i][j][k] = perm_thickness;
	      perm_size_z[3][i][j][k] = cr_size_y2[i][j][k];
	    
	    } // if
	  
	  } // k<3
	
	
	
	  // initialization
	  box[i][j][k] = 0;
	  box2[i][j][k] = 0;
	  box3[i][j][k] = 0;		
	
	  cr_rotA[i][j][k]=0;     
	  cr_rotB[i][j][k]=0;  
	  cr_rotC[i][j][k]=0;
	
	  cr_rotA2[i][j][k]=0;     
	  cr_rotB2[i][j][k]=0;  
	  cr_rotC2[i][j][k]=0;
	
	  acryl_rotA[i][j][k]=0;     
	  acryl_rotB[i][j][k]=0;  
	  acryl_rotC[i][j][k]=0;
	
	  for(int l=0; l<4; l++){
	    box4[l][i][j][k] = 0;	
	    perm_rotA[l][i][j][k]=0;     
	    perm_rotB[l][i][j][k]=0;  
	    perm_rotC[l][i][j][k]=0;
	  }
	
	
	} // k loop
      } // j loop
    } // i loop 
  
  
    //=================     Beam  hole    180 mm     =====================
  
    double zigzag[3];
  
    zigzag[0] = 0;
    zigzag[1] = 20;  // move to North-side 20 mm
    zigzag[2] = -20; // move to South-side 20 mm

    if(m_userFlag>=201902) {
      zigzag[0] = 15; // move to North-side 15 mm
      zigzag[1] = -15; // move to South-side 15 mm
      zigzag[2] = 0; 
    }
  
    double x_space[I][J][K];
  
    for(int i=0; i<6; i++){  
      for(int k=0; k<3; k++){
	for(int j=0; j<J; j++){ 	
	  x_space[i][j][k] = 0.6;
	}
      }
    }
  
    double y_space = 0.29;
    double z_space = 0.29;
    
  
    for(int k=0; k<3; k++){ 
      for(int j=0; j<J; j++){
      
	// CsI crystals      
      
      
	// zone 0 --------------------------------------------------------------------------
	cr_pos_x[0][j][k] = -(hole_size/2 + 200/2 + acryl_size_x[0][j][k] + x_space[0][j][k]);
	cr_pos_y[0][j][k] = ( csi_size/2 + cut_csi_size/2 + y_space )*( 1-j );
	cr_pos_z[0][j][k] = ( csi_size + z_space )*( k-1 );
      
	cr_pos_x2[0][j][k] = -(hole_size/2 + 200 + 100/2 + acryl_size_x[0][j][k] + x_space[0][j][k]);
	cr_pos_y2[0][j][k] = cr_pos_y[0][j][k];
	cr_pos_z2[0][j][k] = cr_pos_z[0][j][k];
      
	modID[0][j][k] = 11 - 2*j + 18*k;
      
	acryl_pos_x[0][j][k] = -(hole_size/2 + acryl_size_x[0][j][k]/2 + x_space[0][j][k]);
	acryl_pos_y[0][j][k] = ( csi_size/2 + cut_csi_size/2 + y_space )*( 1-j );
	acryl_pos_z[0][j][k] = ( csi_size + z_space )*( k-1 );
	acryl_ID[0][j][k] = 11 - 2*j + 18*k;
      
	// zone 3 --------------------------------------------------------------------------
	cr_pos_x[3][j][k] = ( hole_size/2 + 200/2 + acryl_size_x[3][j][k] + x_space[3][j][k] );
	cr_pos_y[3][j][k] = ( csi_size/2 + cut_csi_size/2 + y_space )*( 1-j );
	cr_pos_z[3][j][k] = ( csi_size + z_space )*( k-1 );
      
	cr_pos_x2[3][j][k] = ( hole_size/2 + 200 + 100/2 + acryl_size_x[3][j][k] + x_space[3][j][k] );
	cr_pos_y2[3][j][k] = cr_pos_y[3][j][k];
	cr_pos_z2[3][j][k] = cr_pos_z[3][j][k];
      
	modID[3][j][k] = 10 - 2*j + 18*k;
      
	acryl_pos_x[3][j][k] = ( hole_size/2 + acryl_size_x[3][j][k]/2 + x_space[3][j][k] );
	acryl_pos_y[3][j][k] = ( csi_size/2 + cut_csi_size/2 + y_space )*( 1-j );
	acryl_pos_z[3][j][k] = ( csi_size + z_space )*( k-1 );
	acryl_ID[3][j][k] = 10 - 2*j + 18*k;

      
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      
      
	// zone 1 ---------------------------------------------------------------------------
	cr_pos_x[1][j][k] = zigzag[k] - (200/2 + acryl_size_x[1][j][k] + x_space[1][j][k]);
	cr_pos_y[1][j][k] = ( cut_csi_size - csi_size ) + ( csi_size + y_space )*( j+2 );
	cr_pos_z[1][j][k] = ( csi_size + z_space )*( k-1 );
      
	cr_pos_x2[1][j][k] = zigzag[k] - (200 + 100/2 + acryl_size_x[1][j][k] + x_space[1][j][k]);
	cr_pos_y2[1][j][k] = cr_pos_y[1][j][k];
	cr_pos_z2[1][j][k] = cr_pos_z[1][j][k];
      
	modID[1][j][k] = 13 + 2*j + 18*k;
      
	acryl_pos_x[1][j][k] = zigzag[k] - (acryl_size_x[1][j][k]/2 + x_space[1][j][k]);
	acryl_pos_y[1][j][k] = ( cut_csi_size - csi_size ) + ( csi_size + y_space )*( j+2 );
	acryl_pos_z[1][j][k] = ( csi_size + z_space )*( k-1 );
	acryl_ID[1][j][k] = 13 + 2*j + 18*k;
      
	// zone 2 ---------------------------------------------------------------------------
	cr_pos_x[2][j][k] = zigzag[k] + ( 200/2 + acryl_size_x[2][j][k] + x_space[2][j][k] );
	cr_pos_y[2][j][k] = ( cut_csi_size - csi_size ) + ( csi_size + y_space )*( j+2 );
	cr_pos_z[2][j][k] = ( csi_size + z_space )*( k-1 );
      
	cr_pos_x2[2][j][k] = zigzag[k] + ( 200 + 100/2 +  acryl_size_x[2][j][k] + x_space[2][j][k] );
	cr_pos_y2[2][j][k] = cr_pos_y[2][j][k];
	cr_pos_z2[2][j][k] = cr_pos_z[2][j][k];
      
	modID[2][j][k] = 12 + 2*j + 18*k;
      
	acryl_pos_x[2][j][k] = zigzag[k] + ( acryl_size_x[2][j][k]/2 + x_space[2][j][k] );
	acryl_pos_y[2][j][k] = ( cut_csi_size - csi_size ) + ( csi_size + y_space )*( j+2 );
	acryl_pos_z[2][j][k] = ( csi_size + z_space )*( k-1 );
	acryl_ID[2][j][k] = 12 + 2*j + 18*k;
      
	// zone 4 ---------------------------------------------------------------------------
	cr_pos_x[4][j][k] = zigzag[k] + ( 200/2 + acryl_size_x[4][j][k] + x_space[4][j][k] );
	cr_pos_y[4][j][k] = -( cut_csi_size - csi_size ) - ( csi_size + y_space )*( j+2 );
	cr_pos_z[4][j][k] = ( csi_size + z_space )*( k-1 );
      
	cr_pos_x2[4][j][k] = zigzag[k] + ( 200 + 100/2 + acryl_size_x[4][j][k] + x_space[4][j][k] );
	cr_pos_y2[4][j][k] = cr_pos_y[4][j][k];
	cr_pos_z2[4][j][k] = cr_pos_z[4][j][k];
      
	modID[4][j][k] = 4 - 2*j + 18*k;
      
	acryl_pos_x[4][j][k] = zigzag[k] + ( acryl_size_x[4][j][k]/2 + x_space[4][j][k] );
	acryl_pos_y[4][j][k] = -( cut_csi_size - csi_size ) - ( csi_size + y_space )*( j+2 );
	acryl_pos_z[4][j][k] = ( csi_size + z_space )*( k-1 );
	acryl_ID[4][j][k] = 4 - 2*j + 18*k;
      
	// zone 5 ---------------------------------------------------------------------------
	cr_pos_x[5][j][k] = zigzag[k] -( 200/2 + acryl_size_x[5][j][k] + x_space[5][j][k]);
	cr_pos_y[5][j][k] = -( cut_csi_size - csi_size ) - ( csi_size + y_space )*( j+2 );
	cr_pos_z[5][j][k] = ( csi_size + z_space )*( k-1 );
      
	cr_pos_x2[5][j][k] = zigzag[k] -( 200 + 100/2 + acryl_size_x[5][j][k] + x_space[5][j][k]);      
	cr_pos_y2[5][j][k] = cr_pos_y[5][j][k];
	cr_pos_z2[5][j][k] = cr_pos_z[5][j][k];
      
	modID[5][j][k] = 5 - 2*j + 18*k;
      
	acryl_pos_x[5][j][k] = zigzag[k] -( acryl_size_x[5][j][k]/2 + x_space[5][j][k]);
	acryl_pos_y[5][j][k] = -( cut_csi_size - csi_size ) - ( csi_size + y_space )*( j+2 );
	acryl_pos_z[5][j][k] = ( csi_size + z_space )*( k-1 );
	acryl_ID[5][j][k] = 5 - 2*j + 18*k;
      
      
      
      }
    }

    double dZscintiBottom = 93 - 15/2;//mm
    double dZscintiLeft   = 56 + 15/2;//mm
    double dZscintiRight  = 56 + 15/2;//mm
    double dZscintiTop    = 93 - 15/2;//mm
    if(m_userFlag>=201902) {
      dZscintiBottom = 35;        //mm
      dZscintiLeft   = 35 - 18.5; //mm
      dZscintiRight  = 35 - 18.5; //mm
      dZscintiTop    = 35;        //mm
    }
  
    // Large plastic scintilator ( bottom )
    cr_pos_x[0][0][3] = 0;
    cr_pos_y[0][0][3] = -( csi_size*2 + cut_csi_size );
    cr_pos_z[0][0][3] = -( csi_size*1.5 + z_space + dZscintiBottom );
    modID[0][0][3] = 60;
  
    // Small plastic scintilator ( left )
    cr_pos_x[1][0][3] = 182/2 + 300/2;
    cr_pos_y[1][0][3] = 0;
    cr_pos_z[1][0][3] = -( csi_size*1.5 + z_space + dZscintiLeft );
    modID[1][0][3] = 62;
 
    // Small plastic scintilator ( right )
    cr_pos_x[2][0][3] = -( 182/2 + 300/2 );
    cr_pos_y[2][0][3] = 0;
    cr_pos_z[2][0][3] = -( csi_size*1.5 + z_space + dZscintiRight );
    modID[2][0][3] = 63;
  
    // Large plastic scintilator ( top )
    cr_pos_x[3][0][3] = 0;
    cr_pos_y[3][0][3] = ( csi_size*2 + cut_csi_size );
    cr_pos_z[3][0][3] = -( csi_size*1.5 + z_space + dZscintiTop );
    modID[3][0][3] = 64; 
  


  
    // Permalloy
    for(int i=0; i<6; i++){  
      for(int j=0; j<3; j++){ 	
	for(int k=0; k<3; k++){
	
	  if( (i==0 || i==3) && j%2==0 ){
	    for(int l=0; l<4; l++){
	      perm_pos_x[l][i][j][k] = 0;
	      perm_pos_y[l][i][j][k] = 0;
	      perm_pos_z[l][i][j][k] = 0;
	      perm_ID[l][i][j][k] = 998;
	    }
	  
	  } 
	  else {
	    for(int l=0; l<4; l++){
	      perm_pos_x[l][i][j][k] = cr_pos_x2[i][j][k];
	      perm_ID[l][i][j][k] = modID[i][j][k];
	    }
	  
	    perm_pos_y[0][i][j][k] = cr_pos_y2[i][j][k];
	    perm_pos_y[1][i][j][k] = cr_pos_y2[i][j][k] + cr_size_y2[i][j][k]/2 + perm_size_y[1][i][j][k]/2;
	    perm_pos_y[2][i][j][k] = cr_pos_y2[i][j][k];
	    perm_pos_y[3][i][j][k] = cr_pos_y2[i][j][k] - ( cr_size_y2[i][j][k]/2 + perm_size_y[3][i][j][k]/2 );
	  
	    perm_pos_z[0][i][j][k] = cr_pos_z2[i][j][k] + cr_size_z2[i][j][k]/2 + perm_size_z[0][i][j][k]/2;
	    perm_pos_z[1][i][j][k] = cr_pos_z2[i][j][k];
	    perm_pos_z[2][i][j][k] = cr_pos_z2[i][j][k] - ( cr_size_z2[i][j][k]/2 + perm_size_z[2][i][j][k]/2 );
	    perm_pos_z[3][i][j][k] = cr_pos_z2[i][j][k];
	
	  } // else
      
	}
      }
    }
  
  
    ////////////////////////////////////////////////////////////////////
  

  
    // CsI crystals and Scintilators --------------------------------------------------
    for(int i=0; i<6; i++){
      for(int j=0; j<3; j++){
	for(int k=0; k<4; k++){
	
	  if( k<3 || ( k==3 && i<4 && j==0 ) ) {
	  
	    sprintf( nam,"CC06_%d",i*100+j*10+k );
	  
	    paramVec[i][j][k].push_back( cr_size_x[i][j][k] );      
	    paramVec[i][j][k].push_back( cr_size_y[i][j][k] );
	    paramVec[i][j][k].push_back( cr_size_z[i][j][k] );
	  
	    box[i][j][k] = new GsimBox
	      ( std::string( nam ),this,
		G4ThreeVector( cr_pos_x[i][j][k], cr_pos_y[i][j][k], cr_pos_z[i][j][k] ),
		G4ThreeVector( cr_rotA[i][j][k], cr_rotB[i][j][k], cr_rotC[i][j][k] )); 
	    
	    box[i][j][k]->setParameters( paramVec[i][j][k] );
	    box[i][j][k]->setSensitiveDetector("CC06",modID[i][j][k]);
	    if( k<3 ){
	      box[i][j][k]->setOuterMaterial("G4_CESIUM_IODIDE");
	      box[i][j][k]->setOuterColor("yellow");
	    }else if( k==3 ){
	      box[i][j][k]->setOuterMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	      box[i][j][k]->setOuterColor("red");
	    }
	  
	    addDaughter( box[i][j][k] );
	  
	  } // if 
	
	} // k 
      } // j
    } // i

  
    // CsI2 ----------------------------------------------------------------------------------
    for(int i=0; i<6; i++){
      for(int j=0; j<3; j++){
	for(int k=0; k<3; k++){
	
	  sprintf( nam2,"CC06_csi2_%d",i*100+j*10+k );
	
	  paramVec2[i][j][k].push_back( cr_size_x2[i][j][k] );      
	  paramVec2[i][j][k].push_back( cr_size_y2[i][j][k] );
	  paramVec2[i][j][k].push_back( cr_size_z2[i][j][k] );
	
	  box2[i][j][k] = new GsimBox
	    ( std::string( nam2 ),this,
	      G4ThreeVector( cr_pos_x2[i][j][k], cr_pos_y2[i][j][k], cr_pos_z2[i][j][k] ),
	      G4ThreeVector( cr_rotA2[i][j][k], cr_rotB2[i][j][k], cr_rotC2[i][j][k] ));
	  
	  box2[i][j][k]->setParameters( paramVec2[i][j][k] );
	  box2[i][j][k]->setSensitiveDetector("CC06",modID[i][j][k]);
	  box2[i][j][k]->setOuterMaterial("G4_CESIUM_IODIDE");
	  box2[i][j][k]->setOuterColor("yellow");
	  addDaughter( box2[i][j][k] );
	
	} // k 
      } // j
    } // i
  


    // Acryl --------------------------------------------------------------------------------
    for(int i=0; i<6; i++){
      for(int j=0; j<3; j++){
	for(int k=0; k<3; k++){
	
	  if( (i==0 || i==3) && j%2==0 ){
	    ;
	  }else{
	  
	    sprintf( nam3,"CC06_acryl_%d",i*100+j*10+k );
	  
	    paramVec3[i][j][k].push_back( acryl_size_x[i][j][k] );      
	    paramVec3[i][j][k].push_back( acryl_size_y[i][j][k] );
	    paramVec3[i][j][k].push_back( acryl_size_z[i][j][k] );
	  
	    box3[i][j][k] = new GsimBox
	      ( std::string( nam3 ),this,
		G4ThreeVector( acryl_pos_x[i][j][k], acryl_pos_y[i][j][k], acryl_pos_z[i][j][k] ),
		G4ThreeVector( acryl_rotA[i][j][k], acryl_rotB[i][j][k], acryl_rotC[i][j][k] ));

	    m_detList.push_back(box3[i][j][k]);
	  
	    box3[i][j][k]->setParameters( paramVec3[i][j][k] );
	    if(isDebugMode) {
	      box3[i][j][k]->setSensitiveDetector("CC06", 100 + acryl_ID[i][j][k]);
	    }
	    box3[i][j][k]->setOuterMaterial("G4_PLEXIGLASS");
	    box3[i][j][k]->setOuterColor("blue");
	    addDaughter( box3[i][j][k] );
	
	  } // if
	
	} // k 
      } // j
    } // i



    // Permalloy --------------------------------------------------------------------------------
    for(int i=0; i<6; i++){
      for(int j=0; j<3; j++){
	for(int k=0; k<3; k++){
	  for(int l=0; l<4; l++){
	  
	    if( (i==0 || i==3) && j%2==0 ){
	      ;
	    }else{
	    
	      sprintf( nam4,"CC06_permalloy_%d", i + j*7 + k*7*4 + l*7*4*4 );
	    
	      paramVec4[l][i][j][k].push_back( perm_size_x[l][i][j][k] );      
	      paramVec4[l][i][j][k].push_back( perm_size_y[l][i][j][k] );
	      paramVec4[l][i][j][k].push_back( perm_size_z[l][i][j][k] );
	    
	      box4[l][i][j][k] = new GsimBox
		( std::string( nam4 ),this,
		  G4ThreeVector( perm_pos_x[l][i][j][k], perm_pos_y[l][i][j][k], perm_pos_z[l][i][j][k] ),
		  G4ThreeVector( perm_rotA[l][i][j][k], perm_rotB[l][i][j][k], perm_rotC[l][i][j][k] ));

	      m_detList.push_back(box4[l][i][j][k]);
	    
	      box4[l][i][j][k]->setParameters( paramVec4[l][i][j][k] );
	      if(isDebugMode) {
		box4[l][i][j][k]->setSensitiveDetector("CC06", 200 + perm_ID[l][i][j][k]);
	      }
	      box4[l][i][j][k]->setOuterMaterial("G4_Fe");
	      box4[l][i][j][k]->setOuterColor("white");
	      addDaughter( box4[l][i][j][k] );
	    
	    } // if
	  
	  }// l
	} // k 
      } // j
    } // i
  
    //---------------------------------------------------------------------------------------

    if(m_userFlag>=201902) {
      // Shiomi added
      // CFRP in front of and behind CC06 crystals
      
      
      double CFRP_thickness=0.1; //cm
      double c_cc06_Front_CFRP_z0=-(csi_size/10.*1.5+z_space+0.01)-CFRP_thickness/2.;
      double c_cc06_Back_CFRP_z0=(csi_size/10.*1.5+z_space+0.01)+CFRP_thickness/2.;
      
      GsimBoxWithoutABox* FrontCFRP=0;
      GsimBoxWithoutABox* BackCFRP=0;
      
      
      double CFRPInnerHole=24;  //cm
      double BackCFRPInnerHole=32;  //cm
      double CFRPXLength=97;    //
      double CFRPYLength=60;    //

      std::vector<double> paramVecS;    
      {
	paramVecS.push_back(CFRPXLength*cm);
	paramVecS.push_back(CFRPYLength*cm);
	paramVecS.push_back(CFRP_thickness*cm);
	paramVecS.push_back(CFRPInnerHole*cm);
	paramVecS.push_back(CFRPInnerHole*cm);
	paramVecS.push_back(0);
	paramVecS.push_back(0);
	paramVecS.push_back(0);
	
	FrontCFRP = new GsimBoxWithoutABox("CC06_FrontCFRP",this,
					   G4ThreeVector(0*cm,0*cm,c_cc06_Front_CFRP_z0*cm),
					   G4ThreeVector(0,0,0));
	FrontCFRP->setOuterMaterial("GsimCFRP");
	FrontCFRP->setOuterColor("gray70");
	FrontCFRP->setParameters(paramVecS);
	addDaughter(FrontCFRP);
	m_detList.push_back(FrontCFRP);
	
	paramVecS.clear();
	paramVecS.push_back(CFRPXLength*cm);
	paramVecS.push_back(CFRPYLength*cm);
	paramVecS.push_back(CFRP_thickness*cm);
	paramVecS.push_back(BackCFRPInnerHole*cm);
	paramVecS.push_back(BackCFRPInnerHole*cm);
	paramVecS.push_back(0);
	paramVecS.push_back(0);
	paramVecS.push_back(0);
	
	
	BackCFRP = new GsimBoxWithoutABox("CC06_BackCFRP",this,
					  G4ThreeVector(0*cm,0*cm,c_cc06_Back_CFRP_z0*cm),
					  G4ThreeVector(0,0,0));
	BackCFRP->setOuterMaterial("GsimCFRP");
	BackCFRP->setOuterColor("gray70");
	BackCFRP->setParameters(paramVecS);
	addDaughter(BackCFRP);
	m_detList.push_back(BackCFRP);
	
      }
      
      ///////////////////////////////
      
      
      GsimBoxWithoutABox* FrontWall=0;
      GsimBoxWithoutABox* BackWall=0;
      
      //GsimBoxWithoutABox* HoleSocket;
      //GsimBoxWithoutABox* BackHoleSocket;
      
      double FrontWallXLength=180; //cm
      double FrontWallYLength=90;
      double FrontWallXHole=50;
      double FrontWallYHole=50;
      double WallThickness=0.3;
      
      double FrontWallCenterY=(30+1.5)-(50/2.+6.4)+(FrontWallYLength/2.-(30+1.5)); //cm
      double FrontWallCenterX=0;
      
      double FrontWallY=FrontWallYLength/2.-(30+1.5);
      double FrontWallZ=-17.-10.5-WallThickness/2.;
      
      double BackWallXLength=180; //cm
      double BackWallYLength=90;
      double BackWallXHole=30;
      double BackWallYHole=30;
      
      double BackWallCenterY=(30+1.5)-(30/2.+16.4)+(BackWallYLength/2.-(30+1.5)); //cm
      double BackWallCenterX=0;
      
      double BackWallY=BackWallYLength/2.-(30+1.5);
      double BackWallZ=21.7+1+WallThickness/2.;
      
      
      {
	paramVecS.clear();    
	paramVecS.push_back(FrontWallXLength*cm);
	paramVecS.push_back(FrontWallYLength*cm);
	paramVecS.push_back(WallThickness*cm);
	paramVecS.push_back(FrontWallXHole*cm);
	paramVecS.push_back(FrontWallYHole*cm);
	paramVecS.push_back(FrontWallCenterX*cm);
	paramVecS.push_back(FrontWallCenterY*cm);
	paramVecS.push_back(0);
	
	
	FrontWall = new GsimBoxWithoutABox("CC06_FrontWall",this,
					   G4ThreeVector(0*cm,FrontWallY*cm,FrontWallZ*cm),
					   G4ThreeVector(0,0,0));
	FrontWall->setOuterMaterial("G4_PLEXIGLASS");
	FrontWall->setOuterColor("gray70");
	FrontWall->setParameters(paramVecS);
	addDaughter(FrontWall);
	m_detList.push_back(FrontWall);
	
	
	paramVecS.clear();
	paramVecS.push_back(BackWallXLength*cm);
	paramVecS.push_back(BackWallYLength*cm);
	paramVecS.push_back(WallThickness*cm);
	paramVecS.push_back(BackWallXHole*cm);
	paramVecS.push_back(BackWallYHole*cm);
	paramVecS.push_back(BackWallCenterX*cm);
	paramVecS.push_back(BackWallCenterY*cm);
	paramVecS.push_back(0);
	
	
	BackWall = new GsimBoxWithoutABox("CC06_BackWall",this,
					  G4ThreeVector(0*cm,BackWallY*cm,BackWallZ*cm),
					  G4ThreeVector(0,0,0));
	BackWall->setOuterMaterial("G4_PLEXIGLASS");
	BackWall->setOuterColor("gray70");
	BackWall->setParameters(paramVecS);
	addDaughter(BackWall);
	m_detList.push_back(BackWall);
	
        
      }
      
      
      // Spacer between CC06 crystals //
      
      
      GsimBox* boxSpacer     =0;
      
      double spacer_xlength=5.0; //cm
      double spacer_ylength=0.05; //cm
      double spacer_zlength=20; //cm
      
      double spacer_z0=0;
      double spacer_y0 =-(csi_size/10.*2.5+cut_csi_size/10.+y_space/10.*3.5);
      double spacer_x0[2] ={ -(0.6/10.*0.5+30-spacer_xlength/2.),
			     (0.6/10.*0.5+30-spacer_xlength/2.)};
      
      int spacernum=0;
      for(int iy=0; iy<4; iy++){
	for(int ix=0; ix<2; ix++){
	  
	  if(iy>0&&ix==0) spacer_y0=spacer_y0+csi_size/10.+y_space/10.;
	  if(iy==2&&ix==0) spacer_y0=spacer_y0+csi_size*2/10.+cut_csi_size*2/10.+y_space*4/10.;
	  
	  //std::cout<<spacernum<<" "<<iy<<" "<<ix<<" "<<spacer_x0[ix]<<" "<<spacer_y0<<std::endl;
	  
	  if(boxSpacer==0){
	    std::vector<double> paramVecT;
	    paramVecT.push_back(spacer_xlength*cm);
	    paramVecT.push_back(spacer_ylength*cm);
	    paramVecT.push_back(spacer_zlength*cm);
	    
	    
	    boxSpacer = new GsimBox("CC06Spacer",this,
				    G4ThreeVector(spacer_x0[ix]*cm,spacer_y0*cm,spacer_z0*cm),
				    G4ThreeVector(0,0,0));
	    boxSpacer->setOuterMaterial("G4_PLEXIGLASS");
	    boxSpacer->setParameters(paramVecT);
	    boxSpacer->setOuterColor("yellow");
	    addDaughter(boxSpacer);
	  } else {
	    boxSpacer->cloneDetector(G4ThreeVector(spacer_x0[ix]*cm,spacer_y0*cm,spacer_z0*cm),
				     G4ThreeVector(0,0,0),spacernum);
	  }
	  spacernum+=1;            
	}
      }
      //enf of if m_userFlag>=201902
    }
    
    
  }  else if(m_userFlag==201303) {
    char nam[250] ="";
    //Takashima's geometry
    //=======================================================================  
    /*                   Beam hole size : 180 x 210 mm
  		       
                                    Y axis  
                                    :
                          zone 2    :    zone 1                     ______________    
                      ______________:______________             70 /_____________/|
                     |______2_______|______2_______|           70 |______________|/
                     |______1_______|______1_______|                    300
                 ____|______0_______|______0_______|____
     zone3      |_______0______|    :    |______0_______|   zone 0
   <------------|_______1______|----:----|______1_______|------------------
    X axis      |_______2______|____:____|______2_______|  
                     |_______0______|______0_______|
                     |_______1______|______1_______|
                     |_______2______|______2_______|
                                    :
                         zone 4     :      zone5
                                    : 
  				  
    */
    const int I=6, J=3, K=3;
    std::vector<double> paramVec[I][J][K];
    GsimBox* box[I][J][K];
  
    for(int i=0; i<I; i++){
      for(int j=0; j<J; j++){
	for(int k=0; k<K; k++){
	  box[i][j][k] = 0;
	}
      }
    }
  
    int cnt = 0;
  
    double cr_size_x = 300;
    double cr_size_y = 70;
    double cr_size_z = 70;
  
    double cr_pos_x[I][J][K],  cr_pos_y[I][J][K],  cr_pos_z[I][J][K];
    double cr_rotA[I][J][K],   cr_rotB[I][J][K],   cr_rotC[I][J][K];
  
    for(int i=0; i<I; i++){
      for(int j=0; j<J; j++){
	for(int k=0; k<K; k++){
	  cr_rotA[i][j][k]=0;    cr_rotB[i][j][k]=0;    cr_rotC[i][j][k]=0;
	}
      }
    }  
    
    //=================     Beam  hole    210 mm     =====================
    double zigzag[3];
    zigzag[0] = 0;
    zigzag[1] = 20;
    zigzag[2] = -20;

    double z0=-70;
  
    for(int j=0; j<J; j++){
      for(int k=0; k<K; k++){
	// zone 0
	cr_pos_x[0][j][k] = -240;
	cr_pos_y[0][j][k] = 70*( 1-j );
	cr_pos_z[0][j][k] = 70*k;
	// zone 3
	cr_pos_x[3][j][k] = 240;
	cr_pos_y[3][j][k] = 70*( 1-j );
	cr_pos_z[3][j][k] = 70*k;
      
	// zone 1
	cr_pos_x[1][j][k] = -150+zigzag[k];
	cr_pos_y[1][j][k] = 70*( j+2 );
	cr_pos_z[1][j][k] = 70*k;
	// zone 2
	cr_pos_x[2][j][k] = 150+zigzag[k];
	cr_pos_y[2][j][k] = 70*( j+2 );
	cr_pos_z[2][j][k] = 70*k;
	// zone 4
	cr_pos_x[4][j][k] = 150+zigzag[k];
	cr_pos_y[4][j][k] = -70*( j+2 );
	cr_pos_z[4][j][k] = 70*k;
	// zone 5
	cr_pos_x[5][j][k] = -150+zigzag[k];
	cr_pos_y[5][j][k] = -70*( j+2 );
	cr_pos_z[5][j][k] = 70*k;
      }
    }

  
  
    for(int i=0; i<I; i++){
      for(int j=0; j<J; j++){
	for(int k=0; k<K; k++){
	
	  sprintf( nam,"CC06_%d",i*100+j*10+k );
	
	  paramVec[i][j][k].push_back( cr_size_x );      
	  paramVec[i][j][k].push_back( cr_size_y );
	  paramVec[i][j][k].push_back( cr_size_z );
	
	
	  box[i][j][k] = new GsimBox(std::string( nam ),this,
				     G4ThreeVector( cr_pos_x[i][j][k]*mm,
						    cr_pos_y[i][j][k]*mm,
						    (z0+cr_pos_z[i][j][k])*mm
						    ),
				     G4ThreeVector( cr_rotA[i][j][k], cr_rotB[i][j][k], cr_rotC[i][j][k] )); 
	  box[i][j][k]->setOuterMaterial("G4_CESIUM_IODIDE");
	  box[i][j][k]->setParameters( paramVec[i][j][k] );
	  box[i][j][k]->setOuterColor("yellow");
	  box[i][j][k]->setSensitiveDetector("CC06",cnt);
	  addDaughter( box[i][j][k] );
	  cnt++;
	} 
      }
    }
  } else if(m_userFlag==2011) {
    char nam[250] ="";
    double cc06_len  = Csic_Len;
    double cc06_width = Csic_XY;

    double c_cc06_x_offset[3] = {-0.3, 0,0.3};
    double c_cc06_y_offset[3] = {-0.3, 0,0.3};

    double c_cc06_gap = Csic_Gap;
    double c_cc06_y0 = -(cc06_width+c_cc06_gap)*3.5;
    double c_cc06_z0 = -(cc06_width+c_cc06_gap)*1;
    double c_cc06_x[2] = { -(cc06_len+c_cc06_gap)*0.5,
			   (cc06_len+c_cc06_gap)*0.5 };

    int cnt=0;
    GsimBox* box =0;
    for(int iz=0;iz<3;iz++) {
      for(int iy=0;iy<8;iy++) {
	for(int ix=0;ix<2;ix++) {
	
	  double z = c_cc06_z0+(cc06_width+c_cc06_gap)*iz;
	  double y = c_cc06_y0+(cc06_width+c_cc06_gap)*iy+c_cc06_y_offset[iz];
	  double x=c_cc06_x[ix]+c_cc06_x_offset[iz];
	  if(iy==3 || iy==4) {
	    if(ix==0) x-=7;
	    else      x+=7;
	  }
	  sprintf(nam,"CC06CSI");
	  if(box==0) {
	  
	    std::vector<double> paramVec;
	    paramVec.push_back(cc06_len*cm);
	    paramVec.push_back(cc06_width*cm);
	    paramVec.push_back(cc06_width*cm);
	  
	    box = new GsimBox(std::string(nam),this,
			      G4ThreeVector(x*cm,y*cm,z*cm),
			      G4ThreeVector(0,0,0));
	    box->setOuterMaterial("G4_CESIUM_IODIDE");
	    box->setParameters(paramVec);
	    box->setSensitiveDetector("CC06",cnt);
	    addDaughter(box);
	  } else {
	    box->cloneDetector(G4ThreeVector(x*cm,y*cm,z*cm),
			       G4ThreeVector(0,0,0),cnt);
	  }
	  cnt++;
	}
      }
    }
  } else {
    GsimMessage::getInstance()
      ->report("warning",
	       "Invalid user flag is assigned. CC06 is not constructed.");
  }
  setThisAndDaughterBriefName("CC06");
}

GsimE14CC06::~GsimE14CC06()
{
  ;
}

double GsimE14CC06::getCC06Length() {

  if(m_userFlag>=201304) {
    double cc06_width = 7.006;//cm
    double c_cc06_gap = 0.029;
    return (cc06_width+c_cc06_gap)*3.;
  } else if(m_userFlag==201303) {
    return 7.*3;
  } else if(m_userFlag==2011) {
    double cc06_width = Csic_XY;
    double c_cc06_gap = Csic_Gap;
    return (cc06_width+c_cc06_gap)*3.;
  }
  
  return 0;
}

void GsimE14CC06::comment()
{
  std::cout << "GsimE14CC06 Z position:" << std::endl;
  std::cout << "The center of central crystal in three crystals along z." << std::endl;
  std::cout << "For 201305 and 201504" << std::endl;
  std::cout << "   Place it ~295.5mm downstream from the downstream surface of beam pipe flange." << std::endl;
  std::cout << "   z = (614.8+242.5+155.5+2.+19.+(7.006+0.029)*3/2.)cm " << std::endl;
  std::cout << "For 201605, place it downstream by 2 cm." << std::endl;
  std::cout << "   z=(614.8+242.5+155.5+2.+19.+(7.006+0.029)*3/2.+2)*cm" << std::endl;
  std::cout << "For >=201902, place it at" << std::endl;
  std::cout << "   z=(614.8+242.5+155.5+18.8+(7.006+0.029)*3/2.+2)*cm" << std::endl;
}

void GsimE14CC06::setFastSimulationLevel(int level)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  //reset
  if(m_fastSimulationLevel==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->unsetSensitiveDetector();
    }
  }
  //set
  if(level==6) {
    for(std::list<GsimDetector*>::iterator it=m_detList.begin();
	it!=m_detList.end();it++) {
      (*it)->setSensitiveDetector("CC06",1000);
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
