/* K0TO Correlation Coefficient algorithm
 * Translated to C++ by Jason Stevens from FORTRAN code originally by
 * Joseph Comfort, Arizona State University, Department of Physics, 2013 
 */

class corrcoeff {
public:
  //Return data arrays
  double* ccary;	//Correlation Coefficient
  double* timary;	//Peak time
  double* timsqary;	//Time-squared weighted peak
  int* locary;		//Peak start position
  int* arary;		//datasum values
  int numary;		//Number of data points stored in arrays
  int kstart, kend;	//Where to start and end the scan
  double threshold;	//Theshold; rho squared must be greater than this for cc to be recorded.
  int minarea;		//Minimum size of datasum for cc to be recorded.
private:
  //Reference peak
  const static int refsize = 21;
  int ref[21];
  double toffset;
  long long sumy, sumy2, sigmay2;
  int arlen;		//Array length
  
public:
  corrcoeff(int size);
  ~corrcoeff();
  void analyze(int* stream, int samples);
};

