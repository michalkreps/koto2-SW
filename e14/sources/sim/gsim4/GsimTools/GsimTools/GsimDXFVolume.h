/**
 *  @file
 *  @brief  GsimDXFVolume
 *  $Id$
 *  $Log$
 */
#ifndef GsimDXFVolume_h
#define GsimDXFVolume_h

#include "TVector3.h"
#include <string>
#include <list>
#include <fstream>



class GsimDXFVolume
{
 public:
  GsimDXFVolume();
  virtual ~GsimDXFVolume();

  static void    openDXF(std::string fileName);
  static void    closeDXF();
  static void    addDXFLayer(std::string layerName);
  static bool    checkDumpDXF();
  
  void    dumpDXF();
  //clear the strcuture
  //GsimDetector -> 
  //   create new volume strucutre
  //GsimDetector::createDXFVolume();
  //dump from the top
  
  //define Set/Get
  static void setLayerColor(std::string colorName);
  static void setXVec(TVector3 xVec);
  static void setYVec(TVector3 yVec);
  static void setLayerTruncateLevel(int layerTruncateLevel);
  static void setCutSurfaceNormal(TVector3 vNormal);
  static void setCutSurfacePoint(TVector3 vPoint);
  static void setScaleXY(double sx,double sy);
  static void setExportMode(int exportMode);
  static void setTopVolume(GsimDXFVolume* topVolume);

  void setFullName(std::string fullName);
  void addTraV(TVector3 traV);
  void addRotV(TVector3 rotV);
  void setSurfaceLoop0(std::list<TVector3> posList);
  void setSurfaceLoop1(std::list<TVector3> posList);
  void setSurfaceLoop2(std::list<TVector3> posList);
  void setSurfaceLoop3(std::list<TVector3> posList);
  void registerDaughter(GsimDXFVolume* vol);
  
 protected:

  void collectDXFLayer();
  
  void collectPointPairs(std::list<std::string>& nameList,
			 std::list<TVector3>& v0PointPairs,
			 std::list<TVector3>& v1PointPairs);

  void collectTrianglePoints(std::list<std::string>& nameList,
			     std::list<TVector3>& v0PointPairs,
			     std::list<TVector3>& v1PointPairs,
			     std::list<TVector3>& v2PointPairs);

  void makeLinePointPairs(std::list<std::string>& nameList,
			  std::list<TVector3>& v0PointPairs,
			  std::list<TVector3>& v1PointPairs);

  void makeTrianglePoints(std::list<std::string>& nameList,
			  std::list<TVector3>& v0PointPairs,
			  std::list<TVector3>& v1PointPairs,
			  std::list<TVector3>& v2PointPairs
			  );

  
  void convertPointPairToMother(std::list<std::string>& nameList,
				std::list<TVector3>& v0PointPairs,
				std::list<TVector3>& v1PointPairs);

  void convertTrianglePointToMother(std::list<std::string>& nameList,
				    std::list<TVector3>& v0PointPairs,
				    std::list<TVector3>& v1PointPairs,
				    std::list<TVector3>& v2PointPairs);

  void printDXFLine(std::string layerName,TVector3 v0,TVector3 v1);
  
  TVector3 convert(TVector3 v,
		   const TVector3& rotV,
		   const TVector3& traV);

  TVector3 convertCoordinate(TVector3 v);

  void 
    getCrossSectionPointPairs(std::list<std::string>& nameXSEC,
			      std::list<TVector3>& v0XSEC,
			      std::list<TVector3>& v1XSEC,
			      std::list<std::string>& nameListTriangle,
			      std::list<TVector3>& v0TrianglePoints,
			      std::list<TVector3>& v1TrianglePoints,
			      std::list<TVector3>& v2TrianglePoints);

  double checkCrossPoint(TVector3 p0,TVector3 p1);
  double checkSideOfSurface(TVector3 p0);

  
  std::string m_fullName;
  std::list<TVector3> m_traVList;
  std::list<TVector3> m_rotVList;

  std::list<TVector3> m_surfaceLoop0;
  std::list<TVector3> m_surfaceLoop1;
  std::list<TVector3> m_surfaceLoop2;
  std::list<TVector3> m_surfaceLoop3;

  std::list<GsimDXFVolume*> m_daughterList;

  
  static std::ofstream s_dxfStream;
  static int s_dxfCount;
  static bool s_dxfFirstDump;
  static std::list<std::string> s_dxfLayerNameList;
  static GsimDXFVolume* s_topVolume;
  

  static std::string s_colorName;
  static TVector3    s_xVec;
  static TVector3    s_yVec;
  static int         s_layerTruncateLevel;
  static TVector3    s_cutSurfaceNormal;
  static TVector3    s_cutSurfacePoint;
  static double      s_scaleX;
  static double      s_scaleY;
  static int         s_exportMode;
  //0:3D
  //1:2D cut
  //2:2D cut and impose
  
  
  
};


#endif //GsimDXFVolume_h
