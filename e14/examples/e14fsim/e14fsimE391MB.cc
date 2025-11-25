#include "E14Fsim/E14FsimAnalysisManager.h"
#include "E14Fsim/E14FsimFunction.h"
#include "E14Fsim/E14FsimE391MBInefficiency.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
  char* ifileName=0;
  char* ofileName=0;
  
  int iCluster=-1;//all
  int iFusion=1;  //chi2
  int iSmear=3;   //all
  int nSel=-1;    //all
  double vth=2;// 0.5,1,2,3 MeV threshold

  if(argc==4) {
    ifileName = argv[1];
    ofileName= argv[2];
    vth =  std::atof(argv[3]);
  } else {
    std::cerr << "Arguments error." << std::endl;
    return 1;
  }





  E14FsimE391MBInefficiency*  mb = new E14FsimE391MBInefficiency();
  mb->setupThreshold(vth);
  E14FsimFunction::getFunction()->loadMBInefficiency(mb);

  E14FsimAnalysisManager* am
    = new E14FsimAnalysisManager(ifileName,ofileName,nSel,
				 iCluster, iFusion,iSmear);
  am->run();
  
  return 0;
}


