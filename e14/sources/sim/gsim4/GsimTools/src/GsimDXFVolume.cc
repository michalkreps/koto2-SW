#include "GsimTools/GsimDXFVolume.h"
#include <algorithm>
#include <iostream>
#include "TRotation.h"

std::ofstream GsimDXFVolume::s_dxfStream;
int GsimDXFVolume::s_dxfCount=0;
bool GsimDXFVolume::s_dxfFirstDump=false;
std::list<std::string> GsimDXFVolume::s_dxfLayerNameList;

GsimDXFVolume* GsimDXFVolume::s_topVolume=0;

std::string GsimDXFVolume::s_colorName="black";
TVector3    GsimDXFVolume::s_xVec(0,0,1);
TVector3    GsimDXFVolume::s_yVec(0,1,0);
int         GsimDXFVolume::s_layerTruncateLevel=0;
TVector3    GsimDXFVolume::s_cutSurfaceNormal(1,0,0);
TVector3    GsimDXFVolume::s_cutSurfacePoint(1,0,0);
double      GsimDXFVolume::s_scaleX=1.;
double      GsimDXFVolume::s_scaleY=1.;
int         GsimDXFVolume::s_exportMode=1;


GsimDXFVolume::GsimDXFVolume()
{
  m_fullName="";
  //m_traVList;
  //m_rotVList;
  //m_surfaceLoop0;
  //m_surfaceLoop1;
  //m_surfaceLoop2;
  //m_surfaceLoop3;
  //m_daughterList;
}

GsimDXFVolume::~GsimDXFVolume()
{
  std::list<GsimDXFVolume*>::iterator it=m_daughterList.begin();
  for(;it!=m_daughterList.end();it++) {
    delete (*it);
    (*it)=0;
  }
  m_daughterList.clear();
}


void GsimDXFVolume::openDXF(std::string fileName)
{
  if(s_dxfStream.is_open()) {
    //fstream is already opened.
    std::cerr << "DXF ofstream is already opend.close it and reopen." << std::endl;
    s_dxfStream.close();
    s_dxfStream.clear();
  }
  
  s_dxfStream.open(fileName.c_str());

  if(!s_dxfStream.is_open()) {
    std::cerr << "DXF ofstream open failed." << std::endl;
    return;
  }
  s_dxfFirstDump=false;
  s_dxfLayerNameList.clear();
  s_dxfCount=67;

  
  std::string header = 
    "999\n"
    "dxflib 2.0.4.8\n"
    "  0\n"
    "SECTION\n"
    "  2\n"
    "HEADER\n"
    "  9\n"
    "$ACADVER\n"
    "  1\n"
    "AC1015\n"
    "  9\n"
    "$HANDSEED\n"
    "  5\n"
    "FFFF\n"
    "  9\n"
    "$DIMASZ\n"
    " 40\n"
    "2.5\n"
    "  9\n"
    "$DIMGAP\n"
    " 40\n"
    "0.625\n"
    "  9\n"
    "$INSUNITS\n"
    " 70\n"
    "4\n"
    "  9\n"
    "$DIMEXO\n"
    " 40\n"
    "0.625\n"
    "  9\n"
    "$DIMTXT\n"
    " 40\n"
    "2.5\n"
    "  9\n"
    "$DIMSTYLE\n"
    "  2\n"
    "Standard\n"
    "  9\n"
    "$PSVPSCALE\n"
    " 40\n"
    "0.5\n"
    "  9\n"
    "$PLIMMIN\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    "  9\n"
    "$PLIMMAX\n"
    " 10\n"
    "297.0\n"
    " 20\n"
    "210.0\n"
    "  9\n"
    "$DIMEXE\n"
    " 40\n"
    "1.25\n"
    "  0\n"
    "ENDSEC\n"
    "  0\n"
    "SECTION\n"
    "  2\n"
    "TABLES\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "VPORT\n"
    "  5\n"
    "8\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "1\n"
    "  0\n"
    "VPORT\n"
    "  5\n"
    "30\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbViewportTableRecord\n"
    "  2\n"
    "*Active\n"
    " 70\n"
    "0\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 11\n"
    "1.0\n"
    " 21\n"
    "1.0\n"
    " 12\n"
    "286.3055555555554861\n"
    " 22\n"
    "148.5\n"
    " 13\n"
    "0.0\n"
    " 23\n"
    "0.0\n"
    " 14\n"
    "10.0\n"
    " 24\n"
    "10.0\n"
    " 15\n"
    "10.0\n"
    " 25\n"
    "10.0\n"
    " 16\n"
    "0.0\n"
    " 26\n"
    "0.0\n"
    " 36\n"
    "1.0\n"
    " 17\n"
    "0.0\n"
    " 27\n"
    "0.0\n"
    " 37\n"
    "0.0\n"
    " 40\n"
    "297.0\n"
    " 41\n"
    "1.92798353909465\n"
    " 42\n"
    "50.0\n"
    " 43\n"
    "0.0\n"
    " 44\n"
    "0.0\n"
    " 50\n"
    "0.0\n"
    " 51\n"
    "0.0\n"
    " 71\n"
    "0\n"
    " 72\n"
    "100\n"
    " 73\n"
    "1\n"
    " 74\n"
    "3\n"
    " 75\n"
    "1\n"
    " 76\n"
    "1\n"
    " 77\n"
    "0\n"
    " 78\n"
    "0\n"
    "281\n"
    "0\n"
    " 65\n"
    "1\n"
    "110\n"
    "0.0\n"
    "120\n"
    "0.0\n"
    "130\n"
    "0.0\n"
    "111\n"
    "1.0\n"
    "121\n"
    "0.0\n"
    "131\n"
    "0.0\n"
    "112\n"
    "0.0\n"
    "122\n"
    "1.0\n"
    "132\n"
    "0.0\n"
    " 79\n"
    "0\n"
    "146\n"
    "0.0\n"
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "LTYPE\n"
    "  5\n"
    "5\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "21\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "14\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "ByBlock\n"
    " 70\n"
    "0\n"
    "  3\n"
    "\n"
    " 72\n"
    "65\n"
    " 73\n"
    "0\n"
    " 40\n"
    "0.0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "15\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "ByLayer\n"
    " 70\n"
    "0\n"
    "  3\n"
    "\n"
    " 72\n"
    "65\n"
    " 73\n"
    "0\n"
    " 40\n"
    "0.0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "16\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "CONTINUOUS\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Solid line\n"
    " 72\n"
    "65\n"
    " 73\n"
    "0\n"
    " 40\n"
    "0.0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "31\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DOT\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dot . . . . . . . . . . . . . . . . . . . . . .\n"
    " 72\n"
    "65\n"
    " 73\n"
    "2\n"
    " 40\n"
    "6.3499999999999996\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "32\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DOT2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dot (.5x) .....................................\n"
    " 72\n"
    "65\n"
    " 73\n"
    "2\n"
    " 40\n"
    "3.1749999999999998\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "33\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DOTX2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dot (2x) .  .  .  .  .  .  .  .  .  .  .  .  .\n"
    " 72\n"
    "65\n"
    " 73\n"
    "2\n"
    " 40\n"
    "12.6999999999999993\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "34\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DASHED\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dashed __ __ __ __ __ __ __ __ __ __ __ __ __ _\n"
    " 72\n"
    "65\n"
    " 73\n"
    "2\n"
    " 40\n"
    "19.0500000000000007\n"
    " 49\n"
    "12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "35\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DASHED2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dashed (.5x) _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n"
    " 72\n"
    "65\n"
    " 73\n"
    "2\n"
    " 40\n"
    "9.5250000000000004\n"
    " 49\n"
    "6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "36\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DASHEDX2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dashed (2x) ____  ____  ____  ____  ____  ___\n"
    " 72\n"
    "65\n"
    " 73\n"
    "2\n"
    " 40\n"
    "38.1000000000000014\n"
    " 49\n"
    "25.3999999999999986\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "37\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DASHDOT\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dash dot __ . __ . __ . __ . __ . __ . __ . __\n"
    " 72\n"
    "65\n"
    " 73\n"
    "4\n"
    " 40\n"
    "25.3999999999999986\n"
    " 49\n"
    "12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "38\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DASHDOT2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dash dot (.5x) _._._._._._._._._._._._._._._.\n"
    " 72\n"
    "65\n"
    " 73\n"
    "4\n"
    " 40\n"
    "12.6999999999999993\n"
    " 49\n"
    "6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "39\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DASHDOTX2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Dash dot (2x) ____  .  ____  .  ____  .  ___\n"
    " 72\n"
    "65\n"
    " 73\n"
    "4\n"
    " 40\n"
    "50.7999999999999972\n"
    " 49\n"
    "25.3999999999999986\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "3A\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DIVIDE\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Divide ____ . . ____ . . ____ . . ____ . . ____\n"
    " 72\n"
    "65\n"
    " 73\n"
    "6\n"
    " 40\n"
    "31.75\n"
    " 49\n"
    "12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "3B\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DIVIDE2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Divide (.5x) __..__..__..__..__..__..__..__.._\n"
    " 72\n"
    "65\n"
    " 73\n"
    "6\n"
    " 40\n"
    "15.875\n"
    " 49\n"
    "6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "3C\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "DIVIDEX2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Divide (2x) ________  .  .  ________  .  .  _\n"
    " 72\n"
    "65\n"
    " 73\n"
    "6\n"
    " 40\n"
    "63.5\n"
    " 49\n"
    "25.3999999999999986\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "3D\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "CENTER\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Center ____ _ ____ _ ____ _ ____ _ ____ _ ____\n"
    " 72\n"
    "65\n"
    " 73\n"
    "4\n"
    " 40\n"
    "50.7999999999999972\n"
    " 49\n"
    "31.75\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "3E\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "CENTER2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Center (.5x) ___ _ ___ _ ___ _ ___ _ ___ _ ___\n"
    " 72\n"
    "65\n"
    " 73\n"
    "4\n"
    " 40\n"
    "28.5749999999999993\n"
    " 49\n"
    "19.0500000000000007\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    " 49\n"
    "3.1749999999999998\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "3F\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "CENTERX2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Center (2x) ________  __  ________  __  _____\n"
    " 72\n"
    "65\n"
    " 73\n"
    "4\n"
    " 40\n"
    "101.5999999999999943\n"
    " 49\n"
    "63.5\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "40\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "BORDER\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Border __ __ . __ __ . __ __ . __ __ . __ __ .\n"
    " 72\n"
    "65\n"
    " 73\n"
    "6\n"
    " 40\n"
    "44.4500000000000028\n"
    " 49\n"
    "12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-6.3499999999999996\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "41\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "BORDER2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Border (.5x) __.__.__.__.__.__.__.__.__.__.__.\n"
    " 72\n"
    "65\n"
    " 73\n"
    "6\n"
    " 40\n"
    "22.2250000000000014\n"
    " 49\n"
    "6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    " 49\n"
    "6.3499999999999996\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-3.1749999999999998\n"
    " 74\n"
    "0\n"
    "  0\n"
    "LTYPE\n"
    "  5\n"
    "42\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLinetypeTableRecord\n"
    "  2\n"
    "BORDERX2\n"
    " 70\n"
    "0\n"
    "  3\n"
    "Border (2x) ____  ____  .  ____  ____  .  ___\n"
    " 72\n"
    "65\n"
    " 73\n"
    "6\n"
    " 40\n"
    "88.9000000000000057\n"
    " 49\n"
    "25.3999999999999986\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "25.3999999999999986\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    " 49\n"
    "0.0\n"
    " 74\n"
    "0\n"
    " 49\n"
    "-12.6999999999999993\n"
    " 74\n"
    "0\n"
    "  0\n"
    "ENDTAB\n"
    "  0";

  s_dxfStream << header << std::endl;
  
  
}

void GsimDXFVolume::addDXFLayer(std::string layerName)
{

  if(!s_dxfStream.is_open()) {
    std::cerr << "DXF ofstream is not opend. Give up addDXFLayer" << std::endl;
    return;
  }

  if(s_dxfFirstDump) {
    std::cerr << "DXF dump is already called. Give up addDXFLayer" << std::endl;
    return;
  }

  std::list<std::string>::iterator pos
    = find( s_dxfLayerNameList.begin(), s_dxfLayerNameList.end(), layerName );
  if( pos == s_dxfLayerNameList.end() ) {
    s_dxfLayerNameList.push_back(layerName);
  } else {
    std::cerr << "DXF layer already exist. Don't care." << std::endl;
  }
}

void GsimDXFVolume::closeDXF()
{

  std::string footer=
    "ENDSEC\n"
    "  0\n"
    "SECTION\n"
    "  2\n"
    "OBJECTS\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "C\n"
    "100\n"
    "AcDbDictionary\n"
    "280\n"
    "0\n"
    "281\n"
    "1\n"
    "  3\n"
    "ACAD_GROUP\n"
    "350\n"
    "D\n"
    "  3\n"
    "ACAD_LAYOUT\n"
    "350\n"
    "1A\n"
    "  3\n"
    "ACAD_MLINESTYLE\n"
    "350\n"
    "17\n"
    "  3\n"
    "ACAD_PLOTSETTINGS\n"
    "350\n"
    "19\n"
    "  3\n"
    "ACAD_PLOTSTYLENAME\n"
    "350\n"
    "E\n"
    "  3\n"
    "AcDbVariableDictionary\n"
    "350";
  s_dxfStream << footer << std::endl;
  s_dxfStream << std::hex << s_dxfCount << std::endl;
  footer = 
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "D\n"
    "100\n"
    "AcDbDictionary\n"
    "280\n"
    "0\n"
    "281\n"
    "1\n"
    "  0\n"
    "ACDBDICTIONARYWDFLT\n"
    "  5\n"
    "E\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "1\n"
    "  3\n"
    "Normal\n"
    "350\n"
    "F\n"
    "100\n"
    "AcDbDictionaryWithDefault\n"
    "340\n"
    "F\n"
    "  0\n"
    "ACDBPLACEHOLDER\n"
    "  5\n"
    "F\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "17\n"
    "100\n"
    "AcDbDictionary\n"
    "280\n"
    "0\n"
    "281\n"
    "1\n"
    "  3\n"
    "Standard\n"
    "350\n"
    "18\n"
    "  0\n"
    "MLINESTYLE\n"
    "  5\n"
    "18\n"
    "100\n"
    "AcDbMlineStyle\n"
    "  2\n"
    "STANDARD\n"
    " 70\n"
    "0\n"
    "  3\n"
    "\n"
    " 62\n"
    "256\n"
    " 51\n"
    "90.0\n"
    " 52\n"
    "90.0\n"
    " 71\n"
    "2\n"
    " 49\n"
    "0.5\n"
    " 62\n"
    "256\n"
    "  6\n"
    "BYLAYER\n"
    " 49\n"
    "-0.5\n"
    " 62\n"
    "256\n"
    "  6\n"
    "BYLAYER\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "19\n"
    "100\n"
    "AcDbDictionary\n"
    "280\n"
    "0\n"
    "281\n"
    "1\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "1A\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "1\n"
    "  3\n"
    "Layout1\n"
    "350\n"
    "1E\n"
    "  3\n"
    "Layout2\n"
    "350\n"
    "26\n"
    "  3\n"
    "Model\n"
    "350\n"
    "22\n"
    "  0\n"
    "LAYOUT\n"
    "  5\n"
    "1E\n"
    "100\n"
    "AcDbPlotSettings\n"
    "  1\n"
    "\n"
    "  2\n"
    "C:\\Program Files\\AutoCAD 2002\\plotters\\DWF ePlot (optimized for plotting).pc3\n"
    "  4\n"
    "\n"
    "  6\n"
    "\n"
    " 40\n"
    "0.0\n"
    " 41\n"
    "0.0\n"
    " 42\n"
    "0.0\n"
    " 43\n"
    "0.0\n"
    " 44\n"
    "0.0\n"
    " 45\n"
    "0.0\n"
    " 46\n"
    "0.0\n"
    " 47\n"
    "0.0\n"
    " 48\n"
    "0.0\n"
    " 49\n"
    "0.0\n"
    "140\n"
    "0.0\n"
    "141\n"
    "0.0\n"
    "142\n"
    "1.0\n"
    "143\n"
    "1.0\n"
    " 70\n"
    "688\n"
    " 72\n"
    "0\n"
    " 73\n"
    "0\n"
    " 74\n"
    "5\n"
    "  7\n"
    "\n"
    " 75\n"
    "16\n"
    "147\n"
    "1.0\n"
    "148\n"
    "0.0\n"
    "149\n"
    "0.0\n"
    "100\n"
    "AcDbLayout\n"
    "  1\n"
    "Layout1\n"
    " 70\n"
    "1\n"
    " 71\n"
    "1\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 11\n"
    "420.0\n"
    " 21\n"
    "297.0\n"
    " 12\n"
    "0.0\n"
    " 22\n"
    "0.0\n"
    " 32\n"
    "0.0\n"
    " 14\n"
    "100000000000000000000.0\n"
    " 24\n"
    "100000000000000000000.0\n"
    " 34\n"
    "100000000000000000000.0\n"
    " 15\n"
    "-100000000000000000000.0\n"
    " 25\n"
    "-100000000000000000000.0\n"
    " 35\n"
    "-100000000000000000000.0\n"
    "146\n"
    "0.0\n"
    " 13\n"
    "0.0\n"
    " 23\n"
    "0.0\n"
    " 33\n"
    "0.0\n"
    " 16\n"
    "1.0\n"
    " 26\n"
    "0.0\n"
    " 36\n"
    "0.0\n"
    " 17\n"
    "0.0\n"
    " 27\n"
    "1.0\n"
    " 37\n"
    "0.0\n"
    " 76\n"
    "0\n"
    "330\n"
    "1B\n"
    "  0\n"
    "LAYOUT\n"
    "  5\n"
    "22\n"
    "100\n"
    "AcDbPlotSettings\n"
    "  1\n"
    "\n"
    "  2\n"
    "C:\\Program Files\\AutoCAD 2002\\plotters\\DWF ePlot (optimized for plotting).pc3\n"
    "  4\n"
    "\n"
    "  6\n"
    "\n"
    " 40\n"
    "0.0\n"
    " 41\n"
    "0.0\n"
    " 42\n"
    "0.0\n"
    " 43\n"
    "0.0\n"
    " 44\n"
    "0.0\n"
    " 45\n"
    "0.0\n"
    " 46\n"
    "0.0\n"
    " 47\n"
    "0.0\n"
    " 48\n"
    "0.0\n"
    " 49\n"
    "0.0\n"
    "140\n"
    "0.0\n"
    "141\n"
    "0.0\n"
    "142\n"
    "1.0\n"
    "143\n"
    "1.0\n"
    " 70\n"
    "1712\n"
    " 72\n"
    "0\n"
    " 73\n"
    "0\n"
    " 74\n"
    "0\n"
    "  7\n"
    "\n"
    " 75\n"
    "0\n"
    "147\n"
    "1.0\n"
    "148\n"
    "0.0\n"
    "149\n"
    "0.0\n"
    "100\n"
    "AcDbLayout\n"
    "  1\n"
    "Model\n"
    " 70\n"
    "1\n"
    " 71\n"
    "0\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 11\n"
    "12.0\n"
    " 21\n"
    "9.0\n"
    " 12\n"
    "0.0\n"
    " 22\n"
    "0.0\n"
    " 32\n"
    "0.0\n"
    " 14\n"
    "0.0\n"
    " 24\n"
    "0.0\n"
    " 34\n"
    "0.0\n"
    " 15\n"
    "0.0\n"
    " 25\n"
    "0.0\n"
    " 35\n"
    "0.0\n"
    "146\n"
    "0.0\n"
    " 13\n"
    "0.0\n"
    " 23\n"
    "0.0\n"
    " 33\n"
    "0.0\n"
    " 16\n"
    "1.0\n"
    " 26\n"
    "0.0\n"
    " 36\n"
    "0.0\n"
    " 17\n"
    "0.0\n"
    " 27\n"
    "1.0\n"
    " 37\n"
    "0.0\n"
    " 76\n"
    "0\n"
    "330\n"
    "1F\n"
    "  0\n"
    "LAYOUT\n"
    "  5\n"
    "26\n"
    "100\n"
    "AcDbPlotSettings\n"
    "  1\n"
    "\n"
    "  2\n"
    "C:\\Program Files\\AutoCAD 2002\\plotters\\DWF ePlot (optimized for plotting).pc3\n"
    "  4\n"
    "\n"
    "  6\n"
    "\n"
    " 40\n"
    "0.0\n"
    " 41\n"
    "0.0\n"
    " 42\n"
    "0.0\n"
    " 43\n"
    "0.0\n"
    " 44\n"
    "0.0\n"
    " 45\n"
    "0.0\n"
    " 46\n"
    "0.0\n"
    " 47\n"
    "0.0\n"
    " 48\n"
    "0.0\n"
    " 49\n"
    "0.0\n"
    "140\n"
    "0.0\n"
    "141\n"
    "0.0\n"
    "142\n"
    "1.0\n"
    "143\n"
    "1.0\n"
    " 70\n"
    "688\n"
    " 72\n"
    "0\n"
    " 73\n"
    "0\n"
    " 74\n"
    "5\n"
    "  7\n"
    "\n"
    " 75\n"
    "16\n"
    "147\n"
    "1.0\n"
    "148\n"
    "0.0\n"
    "149\n"
    "0.0\n"
    "100\n"
    "AcDbLayout\n"
    "  1\n"
    "Layout2\n"
    " 70\n"
    "1\n"
    " 71\n"
    "2\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 11\n"
    "12.0\n"
    " 21\n"
    "9.0\n"
    " 12\n"
    "0.0\n"
    " 22\n"
    "0.0\n"
    " 32\n"
    "0.0\n"
    " 14\n"
    "0.0\n"
    " 24\n"
    "0.0\n"
    " 34\n"
    "0.0\n"
    " 15\n"
    "0.0\n"
    " 25\n"
    "0.0\n"
    " 35\n"
    "0.0\n"
    "146\n"
    "0.0\n"
    " 13\n"
    "0.0\n"
    " 23\n"
    "0.0\n"
    " 33\n"
    "0.0\n"
    " 16\n"
    "1.0\n"
    " 26\n"
    "0.0\n"
    " 36\n"
    "0.0\n"
    " 17\n"
    "0.0\n"
    " 27\n"
    "1.0\n"
    " 37\n"
    "0.0\n"
    " 76\n"
    "0\n"
    "330\n"
    "23\n"
    "  0\n"
    "DICTIONARY\n"
    "  5";
  s_dxfStream << footer << std::endl;
  s_dxfStream << std::hex << s_dxfCount << std::endl;
  footer =
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "1\n"
    "  3\n"
    "DIMASSOC\n"
    "350";
  s_dxfStream << footer << std::endl;
  s_dxfStream << std::hex << s_dxfCount+2 << std::endl;
  footer =
    "  3\n"
    "HIDETEXT\n"
    "350";
  s_dxfStream << footer << std::endl;
  s_dxfStream << std::hex << s_dxfCount+1 << std::endl;
  footer = 
    "  0\n"
    "DICTIONARYVAR\n"
    "  5";
  s_dxfStream << footer << std::endl;
  s_dxfStream << std::hex << s_dxfCount+1 << std::endl;
  footer = 
    "100\n"
    "DictionaryVariables\n"
    "280\n"
    "0\n"
    "  1\n"
    "2\n"
    "  0\n"
    "DICTIONARYVAR\n"
    "  5";
  s_dxfStream << footer << std::endl;
  s_dxfStream << std::hex << s_dxfCount+2 << std::endl;
  footer = 
    "100\n"
    "DictionaryVariables\n"
    "280\n"
    "0\n"
    "  1\n"
    "1\n"
    "  0\n"
    "ENDSEC\n"
    "  0\n"
    "EOF";
  s_dxfStream << footer << std::endl;


  s_dxfStream.close();


  s_dxfLayerNameList.clear();
  delete s_topVolume;
  s_topVolume=0;
}

bool GsimDXFVolume::checkDumpDXF()
{


  if(!s_dxfStream.is_open()) {
    std::cerr << "DXF ofstream is not opend. Give up dumpDXF." << std::endl;
    return false;
  }

  //Check layerName registration
  // std::list<std::string>::iterator pos
  //   = find( s_dxfLayerNameList.begin(), s_dxfLayerNameList.end(), layerName );
  // if( pos == s_dxfLayerNameList.end() ) {
  //   std::cerr << "DXF layerName doesn't registered." << std::endl;
  //   return false;
  // }

  if(!s_dxfFirstDump) {

    //dump layer
    int nLayer = s_dxfLayerNameList.size()+1;
    s_dxfStream << "TABLE\n"
      "  2\n"
      "LAYER\n"
      "  5\n"
      "2\n"
      "100\n"
      "AcDbSymbolTable\n"
      " 70" << std::endl;

    s_dxfStream << nLayer << std::endl;
    s_dxfStream << "  0\n"
      "LAYER\n"
      "  5\n"
      "10\n"
      "100\n"
      "AcDbSymbolTableRecord\n"
      "100\n"
      "AcDbLayerTableRecord\n"
      "  2\n"
      "0\n"
      " 70\n"
      "0\n"
      " 62\n"
      "7\n"
      "  6\n"
      "CONTINUOUS\n"
      "370\n"
      "0\n"
      "390\n"
      "F\n"
      "  0" << std::endl;
    
    
    std::list<std::string>::iterator itLayer = s_dxfLayerNameList.begin();
    for(;itLayer!=s_dxfLayerNameList.end();itLayer++) {
      int icol=7;
      s_dxfStream << "LAYER\n"
	"  5" << std::endl;
      s_dxfStream << std::hex << std::uppercase << s_dxfCount << std::endl;
      s_dxfCount++;
      s_dxfStream <<  "100\n"
	"AcDbSymbolTableRecord\n"
	"100\n"
	"AcDbLayerTableRecord\n"
	"  2" << std::endl;
      s_dxfStream << (*itLayer) << std::endl;
      s_dxfStream << " 70\n"
	"0\n"
	" 62" << std::endl;
      s_dxfStream << icol << std::endl;
      s_dxfStream << "  6\n"
	"CONTINUOUS\n"
	"370\n"
	"13\n"
	"390\n"
	"F\n"
	"  0" << std::endl;
      
    }
    s_dxfStream << "ENDTAB" << std::endl;
    s_dxfStream << "  0"    << std::endl;
    
    //dump header2

    std::string header2=
      "TABLE\n"
      "  2\n"
      "STYLE\n"
      "  5\n"
      "3\n"
      "100\n"
      "AcDbSymbolTable\n"
      " 70\n"
      "1\n"
      "  0\n"
      "STYLE\n"
      "  5\n"
      "11\n"
      "100\n"
      "AcDbSymbolTableRecord\n"
      "100\n"
      "AcDbTextStyleTableRecord\n"
      "  2\n"
      "Standard\n"
      " 70\n"
      "0\n"
      " 40\n"
      "0.0\n"
      " 41\n"
      "0.75\n"
      " 50\n"
      "0.0\n"
      " 71\n"
      "0\n"
      " 42\n"
      "2.5\n"
      "  3\n"
      "txt\n"
      "  4\n"
      "\n"
      "  0\n"
      "ENDTAB\n"
      "  0\n"
      "TABLE\n"
      "  2\n"
      "VIEW\n"
      "  5\n"
      "6\n"
      "100\n"
      "AcDbSymbolTable\n"
      " 70\n"
      "0\n"
      "  0\n"
      "ENDTAB\n"
      "  0\n"
      "TABLE\n"
      "  2\n"
      "UCS\n"
      "  5\n"
      "7\n"
      "100\n"
      "AcDbSymbolTable\n"
      " 70\n"
      "0\n"
      "  0\n"
      "ENDTAB\n"
      "  0\n"
      "TABLE\n"
      "  2\n"
      "APPID\n"
      "  5\n"
      "9\n"
      "100\n"
      "AcDbSymbolTable\n"
      " 70\n"
      "1\n"
      "  0\n"
      "APPID\n"
      "  5\n"
      "12\n"
      "100\n"
      "AcDbSymbolTableRecord\n"
      "100\n"
      "AcDbRegAppTableRecord\n"
      "  2\n"
      "ACAD\n"
      " 70\n"
      "0\n"
      "  0\n"
      "ENDTAB\n"
      "  0\n"
      "TABLE\n"
      "  2\n"
      "DIMSTYLE\n"
      "  5\n"
      "A\n"
      "100\n"
      "AcDbSymbolTable\n"
      " 70\n"
      "1\n"
      "100\n"
      "AcDbDimStyleTable\n"
      " 71\n"
      "0\n"
      "  0\n"
      "DIMSTYLE\n"
      "105\n"
      "27\n"
      "100\n"
      "AcDbSymbolTableRecord\n"
      "100\n"
      "AcDbDimStyleTableRecord\n"
      "  2\n"
      "Standard\n"
      " 41\n"
      "2.5\n"
      " 42\n"
      "0.625\n"
      " 43\n"
      "3.75\n"
      " 44\n"
      "1.25\n"
      " 70\n"
      "0\n"
      " 73\n"
      "0\n"
      " 74\n"
      "0\n"
      " 77\n"
      "1\n"
      " 78\n"
      "8\n"
      "140\n"
      "2.5\n"
      "141\n"
      "2.5\n"
      "143\n"
      "0.03937007874016\n"
      "147\n"
      "0.625\n"
      "171\n"
      "3\n"
      "172\n"
      "1\n"
      "271\n"
      "2\n"
      "272\n"
      "2\n"
      "274\n"
      "3\n"
      "278\n"
      "44\n"
      "283\n"
      "0\n"
      "284\n"
      "8\n"
      "340\n"
      "11\n"
      "  0\n"
      "ENDTAB\n"
      "  0\n"
      "TABLE\n"
      "  2\n"
      "BLOCK_RECORD\n"
      "  5\n"
      "1\n"
      "100\n"
      "AcDbSymbolTable\n"
      " 70\n"
      "1\n"
      "  0\n"
      "BLOCK_RECORD\n"
      "  5\n"
      "1F\n"
      "100\n"
      "AcDbSymbolTableRecord\n"
      "100\n"
      "AcDbBlockTableRecord\n"
      "  2\n"
      "*Model_Space\n"
      "340\n"
      "22\n"
      "  0\n"
      "BLOCK_RECORD\n"
      "  5\n"
      "1B\n"
      "100\n"
      "AcDbSymbolTableRecord\n"
      "100\n"
      "AcDbBlockTableRecord\n"
      "  2\n"
      "*Paper_Space\n"
      "340\n"
      "1E\n"
      "  0\n"
      "BLOCK_RECORD\n"
      "  5\n"
      "23\n"
      "100\n"
      "AcDbSymbolTableRecord\n"
      "100\n"
      "AcDbBlockTableRecord\n"
      "  2\n"
      "*Paper_Space0\n"
      "340\n"
      "26\n"
      "  0\n"
      "ENDTAB\n"
      "  0\n"
      "ENDSEC\n"
      "  0\n"
      "SECTION\n"
      "  2\n"
      "BLOCKS\n"
      "  0\n"
      "BLOCK\n"
      "  5\n"
      "20\n"
      "100\n"
      "AcDbEntity\n"
      "  8\n"
      "0\n"
      "100\n"
      "AcDbBlockBegin\n"
      "  2\n"
      "*Model_Space\n"
      " 70\n"
      "0\n"
      " 10\n"
      "0.0\n"
      " 20\n"
      "0.0\n"
      " 30\n"
      "0.0\n"
      "  3\n"
      "*Model_Space\n"
      "  1\n"
      "\n"
      "  0\n"
      "ENDBLK\n"
      "  5\n"
      "21\n"
      "100\n"
      "AcDbEntity\n"
      "  8\n"
      "0\n"
      "100\n"
      "AcDbBlockEnd\n"
      "  0\n"
      "BLOCK\n"
      "  5\n"
      "1C\n"
      "100\n"
      "AcDbEntity\n"
      " 67\n"
      "1\n"
      "  8\n"
      "0\n"
      "100\n"
      "AcDbBlockBegin\n"
      "  2\n"
      "*Paper_Space\n"
      " 70\n"
      "0\n"
      " 10\n"
      "0.0\n"
      " 20\n"
      "0.0\n"
      " 30\n"
      "0.0\n"
      "  3\n"
      "*Paper_Space\n"
      "  1\n"
      "\n"
      "  0\n"
      "ENDBLK\n"
      "  5\n"
      "1D\n"
      "100\n"
      "AcDbEntity\n"
      " 67\n"
      "1\n"
      "  8\n"
      "0\n"
      "100\n"
      "AcDbBlockEnd\n"
      "  0\n"
      "BLOCK\n"
      "  5\n"
      "24\n"
      "100\n"
      "AcDbEntity\n"
      "  8\n"
      "0\n"
      "100\n"
      "AcDbBlockBegin\n"
      "  2\n"
      "*Paper_Space0\n"
      " 70\n"
      "0\n"
      " 10\n"
      "0.0\n"
      " 20\n"
      "0.0\n"
      " 30\n"
      "0.0\n"
      "  3\n"
      "*Paper_Space0\n"
      "  1\n"
      "\n"
      "  0\n"
      "ENDBLK\n"
      "  5\n"
      "25\n"
      "100\n"
      "AcDbEntity\n"
      "  8\n"
      "0\n"
      "100\n"
      "AcDbBlockEnd\n"
      "  0\n"
      "ENDSEC\n"
      "  0";

    s_dxfStream << header2 << std::endl;

    //dump section
    std::string leader=
      "SECTION\n"
      "  2\n"
      "ENTITIES\n"
      "  0";
    s_dxfStream << leader << std::endl;
      
    s_dxfFirstDump=true;
  }

  
  return true;
}


void GsimDXFVolume::collectDXFLayer()
{
  addDXFLayer(m_fullName);
  std::list<GsimDXFVolume*>::iterator it=m_daughterList.begin();
  for(;it!=m_daughterList.end();it++) {
    (*it)->collectDXFLayer();
  }
}

void GsimDXFVolume::collectPointPairs(std::list<std::string>& nameList,
				      std::list<TVector3>& v0PointPairs,
				      std::list<TVector3>& v1PointPairs)
{
  std::list<TVector3> v0This;
  std::list<TVector3> v1This;
  std::list<std::string> thisNameList;
    
  std::list<GsimDXFVolume*>::iterator it=m_daughterList.begin();
  for(;it!=m_daughterList.end();it++) {
    (*it)->collectPointPairs(thisNameList,v0This,v1This);
  }
  makeLinePointPairs(thisNameList,v0This,v1This);
  convertPointPairToMother(thisNameList,v0This,v1This);
  
  v0PointPairs.insert(v0PointPairs.end(), v0This.begin(), v0This.end());
  v1PointPairs.insert(v1PointPairs.end(), v1This.begin(), v1This.end());
  nameList.insert(nameList.end(), thisNameList.begin(), thisNameList.end());
  
}

void GsimDXFVolume::collectTrianglePoints(std::list<std::string>& nameList,
					  std::list<TVector3>& v0PointPairs,
					  std::list<TVector3>& v1PointPairs,
					  std::list<TVector3>& v2PointPairs)
{
  std::list<TVector3> v0This;
  std::list<TVector3> v1This;
  std::list<TVector3> v2This;
  std::list<std::string> thisNameList;
    
  std::list<GsimDXFVolume*>::iterator it=m_daughterList.begin();
  for(;it!=m_daughterList.end();it++) {
    (*it)->collectTrianglePoints(thisNameList,v0This,v1This,v2This);
  }
  
  makeTrianglePoints(thisNameList,v0This,v1This,v2This);
  
  convertTrianglePointToMother(thisNameList,v0This,v1This,v2This);
  
  v0PointPairs.insert(v0PointPairs.end(), v0This.begin(), v0This.end());
  v1PointPairs.insert(v1PointPairs.end(), v1This.begin(), v1This.end());
  v2PointPairs.insert(v2PointPairs.end(), v2This.begin(), v2This.end());
  nameList.insert(nameList.end(), thisNameList.begin(), thisNameList.end());

}

void GsimDXFVolume::makeLinePointPairs(std::list<std::string>& nameList,
				       std::list<TVector3>& v0PointPairs,
				       std::list<TVector3>& v1PointPairs)
{

  {
    std::list<TVector3>::iterator itV0=m_surfaceLoop0.begin();
    std::list<TVector3>::iterator itV1=m_surfaceLoop1.begin();
    TVector3 v0ini=(*itV0);
    TVector3 v1ini=(*itV1);
    
    bool isLooped=false;
    
    if(m_surfaceLoop0.size()==0) return;
    
    while(!isLooped) { 
      TVector3 v00=(*itV0);
      itV0++;
      TVector3 v01=(*itV0);
      if(itV0==m_surfaceLoop0.end()) {
	v01 = v0ini;
	isLooped=true;
      }
      
      TVector3 v10=(*itV1);
      itV1++;
      TVector3 v11=(*itV1);
      if(itV1==m_surfaceLoop1.end()) {
	v11 = v1ini;
	isLooped=true;
      }
      nameList.push_back(m_fullName);
      v0PointPairs.push_back(v00);
      v1PointPairs.push_back(v01);
      
      nameList.push_back(m_fullName);
      v0PointPairs.push_back(v10);
      v1PointPairs.push_back(v11);
      
      nameList.push_back(m_fullName);
      v0PointPairs.push_back(v00);
      v1PointPairs.push_back(v10);
      
    }
  }

  {
    std::list<TVector3>::iterator itV0=m_surfaceLoop2.begin();
    std::list<TVector3>::iterator itV1=m_surfaceLoop3.begin();
    TVector3 v0ini=(*itV0);
    TVector3 v1ini=(*itV1);
    
    bool isLooped=false;
    
    if(m_surfaceLoop2.size()==0) return;
    
    while(!isLooped) { 
      TVector3 v00=(*itV0);
      itV0++;
      TVector3 v01=(*itV0);
      if(itV0==m_surfaceLoop2.end()) {
	v01 = v0ini;
	isLooped=true;
      }
      
      TVector3 v10=(*itV1);
      itV1++;
      TVector3 v11=(*itV1);
      if(itV1==m_surfaceLoop3.end()) {
	v11 = v1ini;
	isLooped=true;
      }
      nameList.push_back(m_fullName);
      v0PointPairs.push_back(v00);
      v1PointPairs.push_back(v01);
      
      nameList.push_back(m_fullName);
      v0PointPairs.push_back(v10);
      v1PointPairs.push_back(v11);
      
      nameList.push_back(m_fullName);
      v0PointPairs.push_back(v00);
      v1PointPairs.push_back(v10);
      
    }
  }
}


void GsimDXFVolume::makeTrianglePoints(std::list<std::string>& nameList,
				       std::list<TVector3>& v0PointPairs,
				       std::list<TVector3>& v1PointPairs,
				       std::list<TVector3>& v2PointPairs)
{
  if(m_surfaceLoop0.size()>0) {
    //Loop0
    {
      std::list<TVector3> list=m_surfaceLoop0;
      std::list<TVector3>::iterator itVSec=list.begin();
      std::list<TVector3>::iterator itVThd=list.begin();
      TVector3 vIni=(*itVSec);
      itVSec++;
      itVThd++;
      itVThd++;
      while(itVThd!=list.end()) {
    	v0PointPairs.push_back( vIni );
    	v1PointPairs.push_back( (*itVSec) );
    	v2PointPairs.push_back( (*itVThd) );
    	nameList.push_back(m_fullName);
	
    	itVSec++;
    	itVThd++;

      }
    }
    
    //Loop1
    {
      std::list<TVector3> list=m_surfaceLoop1;
      std::list<TVector3>::iterator itVSec=list.begin();
      std::list<TVector3>::iterator itVThd=list.begin();
      TVector3 vIni=(*itVSec);
      itVSec++;
      itVThd++;
      itVThd++;
      while(itVThd!=list.end()) {
    	v0PointPairs.push_back( vIni );
    	v1PointPairs.push_back( (*itVSec) );
    	v2PointPairs.push_back( (*itVThd) );
    	nameList.push_back(m_fullName);
	
    	itVSec++;
    	itVThd++;

      }
    }
    
    {
      bool isLooped=false;
      std::list<TVector3> list0=m_surfaceLoop0;
      std::list<TVector3> list1=m_surfaceLoop1;
      
      std::list<TVector3>::iterator itV0=list0.begin();
      std::list<TVector3>::iterator itV1=list1.begin();
      
      while(!isLooped) {
    	TVector3 p0=(*itV0);
    	itV0++;
    	if(itV0==list0.end()) {
    	  itV0=list0.begin();
    	  isLooped=true;
    	}
    	TVector3 p1=(*itV0);
    	TVector3 p2=(*itV1);
    	itV1++;
	
    	v0PointPairs.push_back(p0);
    	v1PointPairs.push_back(p1);
    	v2PointPairs.push_back(p2);
    	nameList.push_back(m_fullName);
      }
    }
    
    {
      bool isLooped=false;
      std::list<TVector3> list0=m_surfaceLoop0;
      std::list<TVector3> list1=m_surfaceLoop1;
      
      std::list<TVector3>::iterator itV0=list0.begin();
      std::list<TVector3>::iterator itV1=list1.begin();
      itV0++;
      
      while(!isLooped) {
    	TVector3 p0=(*itV1);
    	itV1++;
    	if(itV1==list1.end()) {
    	  itV1=list1.begin();
    	  itV0=list0.begin();
    	  isLooped=true;
    	}
    	TVector3 p1=(*itV1);
    	TVector3 p2=(*itV0);
    	itV0++;
	
    	v0PointPairs.push_back(p0);
    	v1PointPairs.push_back(p1);
    	v2PointPairs.push_back(p2);
    	nameList.push_back(m_fullName);
      }
    }
  }


  if(m_surfaceLoop2.size()>0) {
    //Loop0
    {
      std::list<TVector3> list=m_surfaceLoop2;
      std::list<TVector3>::iterator itVSec=list.begin();
      std::list<TVector3>::iterator itVThd=list.begin();
      TVector3 vIni=(*itVSec);
      itVSec++;
      itVThd++;
      itVThd++;
      while(itVThd!=list.end()) {
  	v0PointPairs.push_back( vIni );
  	v1PointPairs.push_back( (*itVSec) );
  	v2PointPairs.push_back( (*itVThd) );
  	nameList.push_back(m_fullName);
	
  	itVSec++;
  	itVThd++;

      }
    }
    
    //Loop1
    {
      std::list<TVector3> list=m_surfaceLoop3;
      std::list<TVector3>::iterator itVSec=list.begin();
      std::list<TVector3>::iterator itVThd=list.begin();
      TVector3 vIni=(*itVSec);
      itVSec++;
      itVThd++;
      itVThd++;
      while(itVThd!=list.end()) {
  	v0PointPairs.push_back( vIni );
  	v1PointPairs.push_back( (*itVSec) );
  	v2PointPairs.push_back( (*itVThd) );
  	nameList.push_back(m_fullName);
	
  	itVSec++;
  	itVThd++;

      }
    }
    
    {
      bool isLooped=false;
      std::list<TVector3> list0=m_surfaceLoop2;
      std::list<TVector3> list1=m_surfaceLoop3;
      
      std::list<TVector3>::iterator itV0=list0.begin();
      std::list<TVector3>::iterator itV1=list1.begin();
      
      while(!isLooped) {
  	TVector3 p0=(*itV0);
  	itV0++;
  	if(itV0==list0.end()) {
  	  itV0=list0.begin();
  	  isLooped=true;
  	}
  	TVector3 p1=(*itV0);
  	TVector3 p2=(*itV1);
  	itV1++;
	
  	v0PointPairs.push_back(p0);
  	v1PointPairs.push_back(p1);
  	v2PointPairs.push_back(p2);
  	nameList.push_back(m_fullName);
      }
    }
    
    {
      bool isLooped=false;
      std::list<TVector3> list0=m_surfaceLoop2;
      std::list<TVector3> list1=m_surfaceLoop3;
      
      std::list<TVector3>::iterator itV0=list0.begin();
      std::list<TVector3>::iterator itV1=list1.begin();
      itV0++;
      
      while(!isLooped) {
  	TVector3 p0=(*itV1);
  	itV1++;
  	if(itV1==list1.end()) {
  	  itV1=list1.begin();
  	  itV0=list0.begin();
  	  isLooped=true;
  	}
  	TVector3 p1=(*itV1);
  	TVector3 p2=(*itV0);
  	itV0++;
	
  	v0PointPairs.push_back(p0);
  	v1PointPairs.push_back(p1);
  	v2PointPairs.push_back(p2);
  	nameList.push_back(m_fullName);
      }
    }
  }
}
  



void GsimDXFVolume::convertPointPairToMother(std::list<std::string>& nameList,
					     std::list<TVector3>& v0PointPairs,
					     std::list<TVector3>& v1PointPairs)
{

  if(v0PointPairs.size()==0) return;

  std::list<TVector3> v0List;
  std::list<TVector3> v1List;
  std::list<std::string> tmpList;
  
  
  std::list<TVector3>::iterator tvIt = m_traVList.begin();
  std::list<TVector3>::iterator rvIt = m_rotVList.begin();
  for(;tvIt!=m_traVList.end();tvIt++,rvIt++) {

    std::list<std::string>::iterator itName=nameList.begin();
    std::list<TVector3>::iterator itV0=v0PointPairs.begin();
    std::list<TVector3>::iterator itV1=v1PointPairs.begin();
    for(;itV0!=v0PointPairs.end();itV0++,itV1++,itName++) {
      
      v0List.push_back(
		       convert( (*itV0),(*rvIt),(*tvIt) )
		       );

      v1List.push_back(
		       convert( (*itV1),(*rvIt),(*tvIt) )
		       );
      
      tmpList.push_back( (*itName) );
    }
  }
  nameList=tmpList;
  v0PointPairs=v0List;
  v1PointPairs=v1List;
}

void GsimDXFVolume::convertTrianglePointToMother(std::list<std::string>& nameList,
						 std::list<TVector3>& v0PointPairs,
						 std::list<TVector3>& v1PointPairs,
						 std::list<TVector3>& v2PointPairs)
{

  if(v0PointPairs.size()==0) return;

  std::list<TVector3> v0List;
  std::list<TVector3> v1List;
  std::list<TVector3> v2List;
  std::list<std::string> tmpList;
  
  std::list<TVector3>::iterator tvIt = m_traVList.begin();
  std::list<TVector3>::iterator rvIt = m_rotVList.begin();
  for(;tvIt!=m_traVList.end();tvIt++,rvIt++) {

    std::list<std::string>::iterator itName=nameList.begin();
    std::list<TVector3>::iterator itV0=v0PointPairs.begin();
    std::list<TVector3>::iterator itV1=v1PointPairs.begin();
    std::list<TVector3>::iterator itV2=v2PointPairs.begin();
    for(;itV0!=v0PointPairs.end();itV0++,itV1++,itV2++,itName++) {
      
      v0List.push_back(
		       convert( (*itV0),(*rvIt),(*tvIt) )
		       );

      v1List.push_back(
		       convert( (*itV1),(*rvIt),(*tvIt) )
		       );

      v2List.push_back(
		       convert( (*itV2),(*rvIt),(*tvIt) )
		       );
      
      tmpList.push_back( (*itName) );
    }
  }
  nameList=tmpList;
  v0PointPairs=v0List;
  v1PointPairs=v1List;
  v2PointPairs=v2List;
}


TVector3 GsimDXFVolume::convert(TVector3 v,
				const TVector3& rotV,
				const TVector3& traV)
{
  v.RotateX(rotV.X());
  v.RotateY(rotV.Y());
  v.RotateZ(rotV.Z());
  v=v+traV;

  return v;
}


void GsimDXFVolume::printDXFLine(std::string layerName,TVector3 v0,TVector3 v1)
{
  
  s_dxfStream << "LINE" << std::endl;
  s_dxfStream << "  5"  << std::endl;
  s_dxfStream << std::hex << std::uppercase << s_dxfCount << std::endl;
  s_dxfCount++;
  s_dxfStream << "100" << std::endl;
  s_dxfStream << "AcDbEntity" << std::endl;
  s_dxfStream << "100" << std::endl;
  s_dxfStream << "AcDbLine" << std::endl;
  s_dxfStream << "  8"<< std::endl;
  s_dxfStream << layerName << std::endl;
  s_dxfStream << " 62" << std::endl;
  s_dxfStream << "256" << std::endl;
  s_dxfStream << "370" << std::endl;
  s_dxfStream << "-1" << std::endl;
  s_dxfStream << "  6" << std::endl;
  s_dxfStream << "ByLayer" << std::endl;
  s_dxfStream << " 10" << std::endl;
  s_dxfStream << v0.X()*s_scaleX << std::endl;
  s_dxfStream << " 20" << std::endl;
  s_dxfStream << v0.Y()*s_scaleY << std::endl;
  s_dxfStream << " 30" << std::endl;
  s_dxfStream << v0.Z() << std::endl;
  s_dxfStream << " 11" << std::endl;
  s_dxfStream << v1.X()*s_scaleX << std::endl;
  s_dxfStream << " 21" << std::endl;
  s_dxfStream << v1.Y()*s_scaleY << std::endl;
  s_dxfStream << " 31" << std::endl;
  s_dxfStream << v1.Z() << std::endl;
  s_dxfStream << "  0" << std::endl;
}

void GsimDXFVolume::dumpDXF()
{

  collectDXFLayer();
  //s_dxfLayerNameList
  
  if(! checkDumpDXF()) return;
  
  std::list<TVector3> v0PointPairs;
  std::list<TVector3> v1PointPairs;
  std::list<std::string> nameList;
  
  collectPointPairs(nameList,
		    v0PointPairs,
		    v1PointPairs);

  std::list<TVector3>::iterator itV0=v0PointPairs.begin();
  std::list<TVector3>::iterator itV1=v1PointPairs.begin();
  std::list<std::string>::iterator itName=nameList.begin();

  
  
  std::list<TVector3> v0TrianglePoints;
  std::list<TVector3> v1TrianglePoints;
  std::list<TVector3> v2TrianglePoints;
  std::list<std::string> nameListTriangle;
  
  
  collectTrianglePoints(nameListTriangle,
   			v0TrianglePoints,
   			v1TrianglePoints,
   			v2TrianglePoints);

  std::list<std::string> nameXSEC;
  std::list<TVector3>    v0XSEC;
  std::list<TVector3>    v1XSEC;
  getCrossSectionPointPairs(nameXSEC,
			    v0XSEC,
			    v1XSEC,
			    nameListTriangle,
			    v0TrianglePoints,
			    v1TrianglePoints,
			    v2TrianglePoints);
  std::list<TVector3>::iterator itV0XSEC=v0XSEC.begin();
  std::list<TVector3>::iterator itV1XSEC=v1XSEC.begin();
  std::list<std::string>::iterator itNameXSEC=nameXSEC.begin();
  
  if(s_exportMode==0) {
    for(;itV0!=v0PointPairs.end();itV0++,itV1++,itName++) {
      TVector3 p0=convertCoordinate((*itV0));
      TVector3 p1=convertCoordinate((*itV1));
      printDXFLine((*itName),p0,p1);
    }
  } else if(s_exportMode==1) {
    for(;itV0!=v0PointPairs.end();itV0++,itV1++,itName++) {
      if(checkSideOfSurface((*itV0))==0 &&
	 checkSideOfSurface((*itV1))==0 ) {
	TVector3 p0=convertCoordinate((*itV0));
	TVector3 p1=convertCoordinate((*itV1));
	printDXFLine((*itName),p0,p1);
      }
    }
    for(;itV0XSEC!=v0XSEC.end();itV0XSEC++,itV1XSEC++,itNameXSEC++) {
      TVector3 p0=convertCoordinate((*itV0XSEC));
      TVector3 p1=convertCoordinate((*itV1XSEC));
      printDXFLine((*itNameXSEC),p0,p1);
    }
  } else if(s_exportMode==2) {
    for(;itV0!=v0PointPairs.end();itV0++,itV1++,itName++) {
      if(checkSideOfSurface((*itV0))>=0 &&
	 checkSideOfSurface((*itV1))>=0 ) {
	TVector3 p0=convertCoordinate((*itV0));
	TVector3 p1=convertCoordinate((*itV1));
	printDXFLine((*itName),p0,p1);
      }
    }
    for(;itV0XSEC!=v0XSEC.end();itV0XSEC++,itV1XSEC++,itNameXSEC++) {
      TVector3 p0=convertCoordinate((*itV0XSEC));
      TVector3 p1=convertCoordinate((*itV1XSEC));
      printDXFLine((*itNameXSEC),p0,p1);
    }
  } else {
    //do nothing
  }
}

TVector3 GsimDXFVolume::convertCoordinate(TVector3 v)
{
  TVector3 zVec = s_xVec.Cross(s_yVec);
  
  TVector3 newX(s_xVec.X(),s_yVec.X(),zVec.X());
  TVector3 newY(s_xVec.Y(),s_yVec.Y(),zVec.Y());
  TVector3 newZ(s_xVec.Z(),s_yVec.Z(),zVec.Z());
  
  TRotation rotM;
  rotM=rotM.RotateAxes(newX,newY,newZ);
  return rotM*v;
}

double GsimDXFVolume::checkSideOfSurface(TVector3 p0)
{
  double t=s_cutSurfaceNormal*(p0-s_cutSurfacePoint);
  return t;
}

double GsimDXFVolume::checkCrossPoint(TVector3 p0,TVector3 p1)
{
  //v=( (1-t)*p0+t*p1 );
  //s_cutSurfaceNormal*(v-s_cutSurfacePoint)=0;
  //s_cutSurfaceNormal*(( (1-t)*p0+t*p1 )-s_cutSurfacePoint)=0;
  //s_cutSurfaceNormal*( (p1-p0)*t+*p0 )
  // -s_cutSurfaceNormal*s_cutSurfacePoint=0;
  //s_cutSurfaceNormal*(p1-p0)*t
  // +s_cutSurfaceNormal*p0 
  // -s_cutSurfaceNormal*s_cutSurfacePoint=0;
  //t=(s_cutSurfaceNormal*s_cutSurfacePoint
  //     -s_cutSurfaceNormal*p0 )
  //  / s_cutSurfaceNormal*(p1-p0);

  double denomi=s_cutSurfaceNormal*(p1-p0);
  if(denomi==0) return -1;

  double nume=
    s_cutSurfaceNormal* ( s_cutSurfacePoint - p0);

  double t=nume/denomi;

  return t;
}

			  
void GsimDXFVolume::
getCrossSectionPointPairs(std::list<std::string>& nameXSEC,
			  std::list<TVector3>& v0XSEC,
			  std::list<TVector3>& v1XSEC,
			  std::list<std::string>& nameListTriangle,
			  std::list<TVector3>& v0TrianglePoints,
			  std::list<TVector3>& v1TrianglePoints,
			  std::list<TVector3>& v2TrianglePoints)
{

  std::list<std::string>::iterator itName=nameListTriangle.begin();
  std::list<TVector3>::iterator itV0=v0TrianglePoints.begin();
  std::list<TVector3>::iterator itV1=v1TrianglePoints.begin();
  std::list<TVector3>::iterator itV2=v2TrianglePoints.begin();

  for(;itV0!=v0TrianglePoints.end();itV0++,itV1++,itV2++,itName++) {
    //3points -> 3lines -> chceck crossing points on surface

    double t0=checkCrossPoint((*itV0),(*itV1));
    double t1=checkCrossPoint((*itV1),(*itV2));
    double t2=checkCrossPoint((*itV2),(*itV0));

    if( (t0>=0 && t0<=1) &&
	(t1>=0 && t1<=1) &&
	(t2>=0 && t2<=1) ) {

      // do nothing
      // nameXSEC.push_back( (*itName) );
      // v0XSEC.push_back( (*itV0) );
      // v1XSEC.push_back( (*itV1) );

      // nameXSEC.push_back( (*itName) );
      // v0XSEC.push_back( (*itV1) );
      // v1XSEC.push_back( (*itV2) );

      // nameXSEC.push_back( (*itName) );
      // v0XSEC.push_back( (*itV2) );
      // v1XSEC.push_back( (*itV0) );
      
    } else if((t0>=0 && t0<=1) && (t1>=0 && t1<=1) ) {
      nameXSEC.push_back( (*itName) );
      v0XSEC.push_back( (1-t0)*(*itV0)+t0*(*itV1) );
      v1XSEC.push_back( (1-t1)*(*itV1)+t1*(*itV2) );
    } else if((t1>=0 && t1<=1) && (t2>=0 && t2<=1) ) {
      nameXSEC.push_back( (*itName) );
      v0XSEC.push_back( (1-t1)*(*itV1)+t1*(*itV2) );
      v1XSEC.push_back( (1-t2)*(*itV2)+t2*(*itV0) );
    } else if((t2>=0 && t2<=1) && (t0>=0 && t0<=1) ) {
      nameXSEC.push_back( (*itName) );
      v0XSEC.push_back( (1-t2)*(*itV2)+t2*(*itV0) );
      v1XSEC.push_back( (1-t0)*(*itV0)+t0*(*itV1) );
    } else {
      //do nothing
    }
  }
}


void GsimDXFVolume::setLayerColor(std::string colorName)
{
  s_colorName=colorName;
}

void GsimDXFVolume::setXVec(TVector3 xVec)
{
  s_xVec=xVec;
}

void GsimDXFVolume::setYVec(TVector3 yVec)
{
  s_yVec=yVec;
}

void GsimDXFVolume::setLayerTruncateLevel(int layerTruncateLevel)
{
  s_layerTruncateLevel=layerTruncateLevel;
}

void GsimDXFVolume::setCutSurfaceNormal(TVector3 vNormal)
{
  s_cutSurfaceNormal=vNormal;
}

void GsimDXFVolume::setCutSurfacePoint(TVector3 vPoint)
{
  s_cutSurfacePoint=vPoint;
}

void GsimDXFVolume::setExportMode(int exportMode)
{
  s_exportMode=exportMode;
}

void GsimDXFVolume::setTopVolume(GsimDXFVolume* topVolume)
{
  s_topVolume=topVolume;
}

void GsimDXFVolume::setFullName(std::string fullName)
{
  m_fullName=fullName;
}
void GsimDXFVolume::addTraV(TVector3 traV)
{
  m_traVList.push_back(traV);
}

void GsimDXFVolume::addRotV(TVector3 rotV)
{
  m_rotVList.push_back(rotV);
}

void GsimDXFVolume::setSurfaceLoop0(std::list<TVector3> posList)
{
  m_surfaceLoop0=posList;
}

void GsimDXFVolume::setSurfaceLoop2(std::list<TVector3> posList)
{
  m_surfaceLoop2=posList;
}

void GsimDXFVolume::setSurfaceLoop3(std::list<TVector3> posList)
{
  m_surfaceLoop3=posList;
}

void GsimDXFVolume::setSurfaceLoop1(std::list<TVector3> posList)
{
  m_surfaceLoop1=posList;
}  

void GsimDXFVolume::registerDaughter(GsimDXFVolume* vol)
{
  m_daughterList.push_back(vol);
}

void GsimDXFVolume::setScaleXY(double sx,double sy)
{
  s_scaleX=sx;
  s_scaleY=sy;
}


