#include "GsimE14Spectrum/GsimE14dist2d_kl.h"
#include "GsimPersistency/GsimMessage.h"
#include "GsimPersistency/GsimPersistencyManager.h"
#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <CLHEP/Random/Randomize.h>
#include <string>
#include <vector>

using namespace std;

//Mod with double value inputs.
double GsimE14dist2d_kl::mod(double x, double y) {
  return x-(((int)(x/y))*y);
}

//LaGrange Interpolation on points x with values y, evaluated at
//xp0. There are expected to be nx points in the given array.
double GsimE14dist2d_kl::lagrange(double* x, double* y, double xp0, int n) {
  double out = 0; //Output
  long double mult;
  int i, j;
  for (i = 0;i<n;i++) {
    mult = 1;
    for (j = 0;j<n;j++) {
      if (i!=j) {
	mult*= (xp0-x[j])/(long double)(x[i]-x[j]);
      }
    }
    out += mult*y[i];
  }
  return out;
}

//LaGrange Interpolation same as above, but order may be specified.
//Chooses the points closest to xp0 to perform the interpolation on.
double GsimE14dist2d_kl::lagrange(double x[], double y[], double xp0, int n, int ord) {
  int upper, lower, count;

  //Find value in x closest to xp0 by binary search.
  upper = n-1;
  lower = 0;
  int k=0;
  while (upper > lower) {
    k = (upper+lower)/2;
    if (upper==lower+1) {
      if ((x[upper]-xp0)>(xp0-x[lower])) k=upper;
      else k = lower;
      break;
    }
    if (x[k]>xp0) {
      upper = k;
    } else if (x[k]<xp0) {
      lower = k;
    }
    else break;
  }

  //Choose ord points closest to xp0.
  upper = k;
  lower = k;
  for (count=1;count<ord;count++) {
    if (upper==n-1) {
      if (lower==0) break;
      lower--;
      continue;
    }
    else if (lower==0) {
      upper++;
      continue;
    }
    else if ((x[upper+1]-xp0)<(x[lower-1]-xp0)) {
      upper++;
    } else {
      lower--;
    }
  }

  return lagrange(&x[lower], &y[lower], xp0, count);
}

void GsimE14dist2d_kl::sampl2d(double& xpart, double& ypart, double& zpart, double& px, double& py, double& pz) {
  //Computes and returns kinematic quantities of an event based on parameterization of the distributions.
  //Returns X, Y, Z coordinates and momentum components px, py, pz.
#ifdef GSIMDEBUG
  static ofstream fout;
#endif
  
  static int ixdvx[20000], iydvy[20000], iptot[500];
  static double xdvx[20002], ydvy[20002], ptot[502];
  static double xval[20002], yval[20002], ptval[500];
  static bool first = true;

  static int nx, ndx, ny, ndy, npt, nxdx, nydy;
  static int nxdx1, nydy1, npt1;
  static double stepx, stepdx, stepy, stepdy;
  static double strtx, strty, strtdx, strtdy;

  static double randx, randy, ranpt;
  static double resultx, resulty, pt;

  static double PI = 3.14159265358979323846;

  if (first) {
#ifdef GSIMDEBUG
    fout.open("test.txt");
#endif
    for (int i=0;i<20000;i++) {
      ixdvx[i] = 0;
      iydvy[i] = 0;
      xdvx[i+1] = 0.0;
      ydvy[i+1] = 0.0;
    }
    for (int i=0;i<500;i++) {
      iptot[i] = 0;
      ptot[i+1] = 0.0;
    }

    //Read in 2D arrays for X/dvrgX, Y/dvrgY, and 1D array for momentum
    ifstream fin;
    std::string ifname = 
      std::getenv("E14_TOP_DIR")+std::string("/share/GsimE14Spectrum/dist2d_C3_kl.dat");
    fin.open(ifname.c_str());
    fin >> nx >> ndx >> ny >> ndy >> npt;
    stepx = 0.1;
    stepdx = 0.1;
    stepy = 0.1;
    stepdy = 0.1;
    strtx = -nx/20.0;
    strty = -ny/20.0;
    strtdx = -ndx/20.0 + stepdx/2.0;
    strtdy = -ndy/20.0 + stepdy/2.0;
    nxdx = nx*ndx;
#ifdef GSIMDEBUG
    fout << nxdx;
#endif
    fin.ignore(10000,'\n');
    fin.ignore(10000,'\n');
    for (int i=0;i<nxdx;i++) {
      fin>>ixdvx[i];
      for (int j=0;j<ixdvx[i];j++)
      GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("fDistX",i);
    }
    fin.ignore(10000,'\n');
    fin.ignore(10000,'\n');
    fin.ignore(10000,'\n');
    nydy = ny*ndy;

    for (int i=0;i<nydy;i++) {
      fin>>iydvy[i];
      for (int j=0;j<iydvy[i];j++)
      GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("fDistY",i);
    }
    fin.ignore(10000,'\n');
    fin.ignore(10000,'\n');
    fin.ignore(10000,'\n');

    for (int i=0;i<npt;i++) {
      fin>>iptot[i];
      for (int j=0;j<iptot[i];j++)
      GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("fDistP",i);
    }
    fin.close();

    //Generate accumulated sum arrays
    xdvx[0] = 0.0;
    for (int i=0;i<nxdx;i++) {
      xdvx[i+1]=xdvx[i]+ixdvx[i];
      for (int j=0;j<xdvx[i+1];j++)
      GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("fCDFX",i);
    }

    xdvx[nxdx+1] = xdvx[nxdx];
    ydvy[0] = 0.0;
    for (int i=0;i<nydy;i++) {
      ydvy[i+1]=ydvy[i]+iydvy[i];
      for (int j=0;j<ydvy[i+1];j++)
      GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("fCDFY",i);
    }

    ydvy[nydy+1] = ydvy[nydy];
    ptot[0] = 0.0;
    for (int i=0;i<npt;i++) {
      ptot[i+1]=ptot[i]+iptot[i];
      for (int j=0;j<ptot[i+1];j++)
      GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("fCDFP",i);
    }

    ptot[npt+1] = ptot[npt];

    //Normalize arrays, 0->1; in place
    double xdxtot = xdvx[nxdx];
    for (int i=0;i<nxdx;i++) xdvx[i+1]=xdvx[i+1]/xdxtot;
    double ydytot = ydvy[nydy];
    for (int i=0;i<nydy;i++) ydvy[i+1]=ydvy[i+1]/ydytot;
    double psum = ptot[npt];
    for (int i=0;i<npt;i++) ptot[i+1]=ptot[i+1]/psum;

    //Set up the coordinates (channel number for the 2D arrays)
    nxdx1 = nxdx+1;
    xval[0]=0.0;
    for (int i=1;i<nxdx1;i++) xval[i] = (double)i;
    nydy1 = nydy+1;
    yval[0]=0.0;
    for (int i=1;i<nydy1;i++) yval[i]=(double)i;
    npt1 = npt+1;
    ptval[0] = 0.0;
    for (int i=1;i<npt1;i++) {
      ptval[i]=i*0.1;
    }
    first = false;
#ifdef GSIMDEBUG
    fout.close();
#endif
  }
  
  static int irow, jrow;
  static double colx, coly, xrot, dxrot, yrot, dyrot, arot, divx, divy;;

  zpart = 639.0;
  randx = CLHEP::RandFlat::shoot();
  randy = CLHEP::RandFlat::shoot();
  ranpt = CLHEP::RandFlat::shoot();

  resultx = lagrange(xdvx, xval, randx, nxdx1, 3);
  resulty = lagrange(ydvy, yval, randy, nydy1, 3);
  
  pt = lagrange(ptot, ptval, ranpt, npt1, 3);
  
  GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("XInterp",resultx);
  GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("YInterp",resulty);
  GsimPersistencyManager::getPersistencyManager()
	->fillHistogram("PInterp",pt);

  randx = CLHEP::RandFlat::shoot();
  randy = CLHEP::RandFlat::shoot();

  irow = resultx/nx + 1.0;
  colx = mod(resultx, (double)nx);
  xrot = strtx + colx*stepx;
  dxrot = strtdx + (irow-1)*stepdx + (randx-0.5)*stepdx;
  jrow = resulty/ny + 1.0;

  coly = mod(resulty, 1.0*ny);
  yrot = strty + coly*stepy;
  dyrot = strtdy + (jrow-1)*stepdy + (randy-0.5)*stepdy;

  arot = 3.5*PI/180.0;
  dxrot += 1.33*xrot;
  xpart = xrot*cos(arot) - dxrot*sin(arot);
  divx = xrot*sin(arot) + dxrot*cos(arot);
  arot = 2.0*M_PI/180.0;
  dyrot += 1.45*yrot;
  
  ypart = yrot*cos(arot) - dyrot*sin(arot);
  divy = yrot*sin(arot) + dyrot*cos(arot);
  px = pt*sin(divx/1000.0);
  py = pt*sin(divy/1000.0);
  pz = sqrt(pt*pt - px*px - py*py);

}

GsimE14dist2d_kl::GsimE14dist2d_kl(string name) : GsimSpectrum(name) {

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("distMomentum","Momentum [GeV/c]", 100,0,10);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("distX", "Initial x [cm]",100,-2,2);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("distY", "Initial y [cm]",100,-2,2);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("distpX", "Initial X momentum",100,0,0.01);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("distpY", "Initial Y momentum", 100,0,0.01);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("distpZ", "Initial Z momentum", 100,0,10);
    
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("fDistX","File Distribution X",2000,0,2000);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("fDistY","File Distribution Y",2000,0,2000);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("fDistP","File Distribution Momentum",150,0,150);
    
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("fCDFX","File CDF X",2000,0,2000);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("fCDFY","File CDF Y",2000,0,2000);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("fCDFP","File CDF Momentum",150,0,150);

  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("XInterp","Output of X Interpolation",1000,0,2000);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("YInterp","Output of Y Interpolation",1000,0,2000);
  GsimPersistencyManager::getPersistencyManager()
    ->createHistogram("PInterp","Output of P Interpolation",100,0,10);
}

void GsimE14dist2d_kl::generateSpecial(const G4Event* anEvent) {
  G4ThreeVector position;
  G4ThreeVector momentum;
  double x, y, z, px, py, pz;
  G4PrimaryVertex* pv = anEvent->GetPrimaryVertex(0);
  
  //  do {
  sampl2d(x, y, z, px, py, pz);
  //  } while ((x!=x)||(y!=y)||(px!=px));
  // if x!=x is necessary, x,y,px should be member variable.
  // ignore such check by HN

  position.set(x, y, 0);
  momentum.set(px, py, pz);


  
  // z should be -21*m+ 639*cm - detectorShiftZ
  // generate around pv->GetPosition().x() and y() by HN
  G4ThreeVector vPos = pv->GetPosition();
  double  detectorShiftZ=507*mm;
  z=-21*m+639*cm-detectorShiftZ;
  pv->SetPosition(
		  x*cm+vPos.x(),
		  y*cm+vPos.y(),
		  z);
  
  G4PrimaryParticle* particle = pv->GetPrimary();
  particle->SetMomentum(px*GeV,py*GeV,pz*GeV);
  
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("distMomentum",momentum.mag());
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("distX",position.getX());
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("distY",position.getY());
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("distpX",px);
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("distpY",py);
  GsimPersistencyManager::getPersistencyManager()
    ->fillHistogram("distpZ",pz);
}

