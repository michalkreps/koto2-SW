#include "GsimTools/GsimKPiGGDecay.h"

#include <iostream>

int main(int ,char** )
{
  GsimKPiGGDecay* kpggd = new GsimKPiGGDecay();
  kpggd->pgg6init();
  kpggd->crateRootFile("av070.root");
  kpggd->setAV(-0.43);
  kpggd->pgg6init();
  kpggd->crateRootFile("av043.root");


  double Ppi[4];
  double Pg2[4];
  double Pg3[4];
  for(int i=0;i<10;i++) {
    kpggd->pi0ggdk(Ppi,Pg2,Pg3);
  }
  
  return 0;
}
