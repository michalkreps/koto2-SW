#include "corrcoeff/corrcoeff.h"

corrcoeff::corrcoeff(int size) {
  int tmpV[]={ 27, 77, 196, 381, 615, 854, 1056, 1188, 1230, 1180, 1053, 859, 659, 476, 325, 211, 132, 86, 61, 44, 42 };
  for(int i=0;i<refsize;i++) {
    ref[i]=tmpV[i];
  }
  ccary = new double[size];
  timary = new double[size];
  timsqary = new double[size];
  locary = new int[size];
  arary = new int[size];
  
  //Initialize variables.
  sumy = 0; sumy2 = 0;
  for (int i=0;i<refsize;i++) {
    sumy += ref[i];
    sumy2 += ref[i]*ref[i];
  }
  sigmay2 = sumy2/refsize - 512*512;
  //Set parameters to their default values.
  kstart = 0;
  kend = 63;
  threshold = 0;
  minarea = 0;
  arlen = size;
  toffset = 8.5;
}

corrcoeff::~corrcoeff() {
  //Delete all arrays.
  delete [] ccary;
  delete [] timary;
  delete [] timsqary;
  delete [] locary;
  delete [] arary;
}

void corrcoeff::analyze(int* stream, int samples) {
  long long datasum = 0;
  long long data2sum = 0;
  long long wdatasum = 0;
  long long w2datasum = 0;
  long long top, drprod, sigmax, sigmax2;
  double rhosq, topsq, sigma2xy;
  
  //Zero arrays
  numary = 0;
  for (int i=0;i<arlen;i++) {
    ccary[i] = 0.0;
    timary[i] = 0.0;
    timsqary[i] = 0.0;
    locary[i] = 0;
    arary[i] = 0;
  }
  
  //Set up initial sums
  for (int i=kstart;i<(refsize+kstart);i++) {
    datasum+=stream[i];
    data2sum+=stream[i]*stream[i];
    wdatasum+=i*stream[i];
    w2datasum+=i*i*stream[i];
  }
  //Determine end point
  int end = kend;
  if (samples-refsize-8<kend) end = samples-refsize-8;
  
  //Shift peak through sample data and compute rho squared at each step.
  for (int i=kstart;i<end;i++) {
    if (datasum>minarea) {
      drprod = 0;
      for (int j=0;j<refsize;j++) drprod += stream[i+j]*ref[j];
      top = drprod - 512*datasum;
      if (top>0) {
	topsq = (double)(top*top);
	sigmax2 = refsize*data2sum - datasum*datasum;
	sigma2xy = (double)(sigmax2*sigmay2);
	rhosq = topsq/sigma2xy;
	if (rhosq>threshold) {
	  locary[numary] = i;
	  arary[numary] = datasum;
	  ccary[numary] = rhosq;
	  timary[numary] = wdatasum/(double)datasum;
	  timsqary[numary] = w2datasum/(double)datasum - timary[numary]*timary[numary];
	  timary[numary] -= toffset;
	  numary++;
	}
      }
    }
    //Set sums for next sample.
    int index = refsize+i;
    datasum += stream[index] - stream[i];
    data2sum += stream[index]*stream[index]-stream[i]*stream[i];
    wdatasum += index*stream[index] - i*stream[i];
    w2datasum = w2datasum + index*index*stream[index] - i*i*stream[i];
  }
}

