#include <iostream>
#include <iomanip>
#include "corrcoeff/corrcoeff.h"

using namespace std;

int main() {
  int pedestal = 0;
  int data[80];
  
  cout << "Test\n";
  
  //Take in pedestal
  cin >> pedestal;
  //Zero the first and last eight elements of the array.
  for (int i=0;i<8;i++) {
    data[i]=0;
    data[79-i]=0;
  }
  //Read in data. Perform pedestal subtraction along the way.
  for (int i=8;i<72;i++) {
    cin >> data[i];
    data[i]-=pedestal;
  }
  
  //Instantiate the correlation coefficient class.
  corrcoeff* c = new corrcoeff(80);
  c->analyze(data, 80);
  
  //Display analyzed data
  for (int i=0;i<c->numary;i++) cout << setw(6) << c->locary[i];
  cout << endl;
  for (int i=0;i<c->numary;i++) cout << setw(6) << c->arary[i];
  cout << endl;
  for (int i=0;i<c->numary;i++) cout << setw(6) << setprecision(4) << c->timary[i];
  cout << endl;
  for (int i=0;i<c->numary;i++) cout << setw(6) << setprecision(4) << c->timsqary[i];
  cout << endl;
  for (int i=0;i<c->numary;i++) cout << setw(6) << setprecision(2) << c->ccary[i];
  cout << endl;
  
  delete c;
  return 0;
}
