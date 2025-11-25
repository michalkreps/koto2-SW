#ifndef E14FsimMBPropsalInefficiency_h
#define E14FsimMBPropsalInefficiency_h
#include "E14Fsim/E14FsimInefficiency.h"
#include <list>
class TGraph;
class TGraphErrors;
class TH1D;


class E14FsimMBProposalInefficiency : public E14FsimInefficiency
{
public:
  E14FsimMBProposalInefficiency();
  virtual ~E14FsimMBProposalInefficiency();
  virtual double getWeight(double Mome,double theta, double z);
  virtual double getWeight(double Mome,double theta);
  
  TGraph* draw(double theta);
  TGraphErrors* drawES171();
  void drawFrame();
  
 private:
  double func0( double* par, double x );
  double func1( double* par, double x );
  double m_fpar[9][4];
  TH1D* m_frame;
    
};
#endif
