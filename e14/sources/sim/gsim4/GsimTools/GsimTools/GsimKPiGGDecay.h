#ifndef GsimKPiGGDecay_h
#define GsimKPiGGDecay_h

class TH1D;

class GsimKPiGGDecay
{
 public:

  /// Constructor.
  GsimKPiGGDecay();

  /// Destructor.
  virtual ~GsimKPiGGDecay();


  /// initailzaion
  void pgg6init();
  
  /// generation of decay particles at KL rest frame.
  void pi0ggdk(double* Ppi,double* Pg2,double* Pg3);
  
  /// set a_v
  void setAV(double av);

  /// get branching ration.
  double getBR();

  /// make MGG root file.
  void crateRootFile(const char* ofn);

  
 private:

  double pgg6babs(double z);
  double pgg6aabs(double y,double z);
  double d2brabs(double y, double z);

  double pgg6pbdisp(double z);
  double pgg6adisp(double y, double z);
  double d2brdisp(double y, double z);
    
  double pgg6zmax(double y);
  double pgg6ymax(double z);

  double pgg6zmin(double y);
  double pgg6ymin(double z);

  //12 points Gauss integration
  double pgg6int2(double (GsimKPiGGDecay::*f)(double,double),
		  double xa,
		  double xb,
		  double (GsimKPiGGDecay::*ymin)(double),
		  double (GsimKPiGGDecay::*ymax)(double),
		  int numrett);
  
  double pgg6intx(double (GsimKPiGGDecay::*f)(double,double),
		  double y, double a, double b,
		  int numintervalli);
  double pgg6intx2(double (GsimKPiGGDecay::*f)(double,double),
		   double y, double a, double b,
		   int numintervalli);


  double ranuni();
  void gensph(double* vec);
  void dboost(double* P0,double U0,double* PA,double* PB);

  ////////////////////////////

  /// KL mass
  double m_mkl;
  /// pi0 mass
  double m_mp0;
  /// coupling constant
  double m_alpha;
  
  ////////////////////////////

  /// mkl^2
  double m_mkl2;
  /// mkl^4
  double m_mkl4;
  /// mpi^2
  double m_mp2;
  /// (mpi/mkl)
  double m_rp;
  /// (mpi/mkl)^2
  double m_rp2;
  /// (mpi/mkl)^4
  double m_rp4;
  ////////////////////////////
  
  ///p^6 corrections parameters
  double m_fp;

  ////////////////////////////

  ///Gamma_KL
  double m_gammakl;

  ////////////////////////////

  ///Here we include the parameters of K->3pi from the fit in the reference
  ///     J. Kambor et al., Phys. Lett. B261 (1991) 496
  double m_alfa1;
  double m_beta1;
  double m_zita1;
  double m_csi1;
  double m_alfa3;
  double m_beta3;
  double m_gamma3;
  double m_zita3;
  double m_csi3;
  double m_csi3primo;

  ////////////////////////////
  double m_h8;
  double m_h27;
  ////////////////////////////


  ///a_v
  /**
   * a_v which represents the vector meson exchange contribution
   * (to the B amplitude).
   */
  double m_PGG_AV;

  double m_lambdacut;
  ////////////////////////////
  
  /// y minimum
  double m_yminimo;
  /// z minimum
  double m_zminimo;


  /// branching ratio from ChPT
  double m_branch;
  /// histogram for Mgg
  static TH1D  s_hMGG;
  
};

inline void GsimKPiGGDecay::setAV(double av)
{
  m_PGG_AV=av;
}
inline double GsimKPiGGDecay::getBR()
{
  return m_branch;
}
#endif //GsimKPiGGDecay_h
