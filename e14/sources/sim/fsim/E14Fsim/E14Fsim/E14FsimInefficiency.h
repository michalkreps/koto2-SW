#ifndef E14FsimInefficiency_h
#define E14FsimInefficiency_h
#include <list>
class TGraph;


class E14FsimInefficiency
{
public:
  E14FsimInefficiency();
  virtual ~E14FsimInefficiency();
  
  bool   setupThreshold(double vth);
  bool   setup2ndThreshold(double vth, double z0,double z1);
  virtual double getWeight(double Mome,double theta);
  virtual double getWeight2nd(double Mome,double theta);
  virtual double getWeight(double Mome,double theta, double z);


  TGraph*   draw(int iThreshold, int iTheta);
  TGraph*   draw(double threshold, double theta);
  
protected:
  void initialize(int nTheta,
		  int nMome,
		  int nThreshold,
		  double* ineffiSingle,
		  double* thetaTemp,
		  double* momeTemp,
		  double* threTemp);
  
  double getLogInterpolate(double x,
			   double x1,double y1,
			   double x2,double y2);
  
  
  double getInterpolate(double x,
			double x1,double y1,
			double x2,double y2);
  inline int getIndex(int iTheta,int iMome, int iThreshold)
  {
    return iTheta*(m_nMome*m_nThreshold)+iMome*m_nThreshold+iThreshold;
  }

  int    m_nTheta;
  int    m_nMome;
  int    m_nThreshold;
  
  double* m_ineffiDef;
  double* m_thetaDef;
  double* m_momeDef;
  double* m_threDef;

  /// interpolated inefficiency for the threshold
  double m_ineffi[100][100];
  double  m_threshold;

  double m_z0; // min z for 2nd ineffi.
  double m_z1; // max z for 2nd ineffi.
  
  /// interpolated inefficiency for the threshold for the other z region
  bool   m_2ndFilled;
  double m_ineffi2[100][100];
  
  std::list<TGraph*> m_graphList;
};
#endif
