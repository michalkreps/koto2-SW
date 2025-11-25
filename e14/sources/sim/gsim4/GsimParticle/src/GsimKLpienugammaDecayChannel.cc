/**
 *  @file
 *  @brief  GsimKL2pi0gammaDecayChannel
 *  $Id$ 
 *  $Log$ 
 */

#include "GsimParticle/GsimKLpienugammaDecayChannel.h"

#include "G4VDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayProducts.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"

using CLHEP::MeV;


double GsimPiENuGammaDecayChannel::f_plus(double t){
	const double f0 = 0.977;
	const double lam = 0.0294;
	return f0*(1+lam*t/mp1/mp1);
}



GsimPiENuGammaDecayChannel::GsimPiENuGammaDecayChannel(
			const G4String& theParentName,
			G4double        theBR,
			const G4String& theElectronName,
			const G4String& thePionName,
			const G4String& theNuName,
			const G4String& theGammaName
			)
  :G4VDecayChannel("KLpienugamma Decay",theParentName,
		   theBR,  4,
		   theElectronName,  thePionName,  theNuName, theGammaName)
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

  mkl=0.;
  mp1=0.;
  me=0.;
  pi = CLHEP::pi;

  meta = 548;
  mrho = 770;

  Fp = 92.4;
  Fk = Fp*1.22;

  std::cout << "KL -> pi e nu gamma decay channel was contructed.\n";
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimPiENuGammaDecayChannel::~GsimPiENuGammaDecayChannel()
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

G4DecayProducts* GsimPiENuGammaDecayChannel::DecayIt(G4double) 
{
#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif
  
  //  if (parent == 0) {
  //    FillParent();
  //  }
  G4ParticleDefinition* my_parent = GetParent();

  // fill daughter particles and their mass
  //  if (daughters == 0) {
  //    FillDaughters();
  //  }
  G4ParticleDefinition* my_daughters[4];
  for(int k=0;k<4;k++) {
    my_daughters[k] = GetDaughter(k);
  }



  //create parent G4DynamicParticle at rest
  G4ThreeVector dummy;
  G4DynamicParticle * parentparticle 
    = new G4DynamicParticle(my_parent, dummy, 0.0);
  
  //create G4Decayproducts
  G4DecayProducts *products = new G4DecayProducts(*parentparticle);
  delete parentparticle;
  
  if(mkl==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("kaon0L");
    mkl=particle->GetPDGMass()/MeV;
  }
  if(mp1==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("pi-");
    mp1=particle->GetPDGMass()/MeV;
  }
  if(me==0) {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = 0;
    particle = particleTable->FindParticle("e-");
    me=particle->GetPDGMass()/MeV;
	init_formfactor();
  }




  /* ---------------------------------------------------------------------------
	
	 The formalism is taken from Eur. Phys. J. C40, 205 (2005).
	 The explicit formula of the spin sum of the amplitude squared is 
	 introduced in the appendix B. 
	 The form factors, which descibe the vector and axial vector contributions
	 are introduced in the chapter 3. In this implementation, the simplified ones
	 (Eq. 4.3) are used, where momentum dependence is dropped out.
	 
	 HOWEVER!! According to Eq.(B.1), the V1, V2, V3, and V4 should have same 
	 dimension of mass because the coefficients of bi and ci are dimensionless.  

	 On the other hand, Vi (i=1,2,3) introduced in Eq.(4.3) shows that they actually
	 have different dimension. V1 and V2 are dimension of mass^{-2} while V3 has 
	 a dimension of mass^{-4}. Indeed, this is reasonable from the original defintion.
	 See (3.16). From the scratch, the Vi are defined such that they do not neccessarily 
	 have same dimension of masses. Somehow, the author may have misunderstood 
	 the treatment of the differece of the notation of Vi. For this reasons,
	 the current implementation is beta version.


	 The diff. decay rate is written as

	 dGamma \propto dLIPS * |T|^2 
	 = dm^2_{pi e gam} dm^2_{e gam} dOmega_{nu} dOmega'_{e gam} dOmega''_{e} 
	   (2p_nu/Mkl)*(2*p'_{eg}/m_{pi e gam})*(2*p''_{g}/m_{e gam}) * |T|^2
	 
	 \propto dm^2_{pi e gam} m_{e gam} dm_{e gam} dOmega'_{e gam} dOmega''_{e} 
	   (2p_nu/Mkl)*(2*p'_{eg}/m_{pi e gam})*(2*p''_{g}/m_{e gam}) * |T|^2

	 \equiv dm^2_{pi e gam} dm_{e gam} dOmega'_{e gam} dOmega''_{e} PDF,
	 
	 where prime and double-prime of the Omegas mean they are defined in the
	 pi-e-gam and e-gam rest frame, respectively, and 
	 PDF = m_{e gam} (2p_nu/Mkl)*(2*p'_{eg}/m_{pi e gam})*(2*p''_{g}/m_{e gam})* |T|^2.
	 
	 Since the diff. decay rate has a singularity on Eg --> 0 and
	 costh_{eg}-->1, m_{e gam} and m_{pi e gam} should not be distributed uniformly. 
	 In this calculation, m = m_{e gam} is distributed by 
	 PDF1(m) = 2m/(m^2-me^2). The left hand side is indeed equal to 1/Eg''.
	 In addition,  m^2 = m^2_{pi e gam} is distributed uniformly:
	 PDF2(m^2) = 1/(M1^2-M0^2), where M1 and M0 are max and min of m^2. 

	 To make the decay rate finite, we need a cutoff of photon energy.
	 By default, 1% of KL mass ~5MeV ''IN THE e-gam rest frame'' is chosen.

	 The weighted distribution explained above requires the majorizing function
	 defined as:
	 maj(m^2_{pi e gam}, m_{e gam}) = const. times PDF1(m_{e gam})PDF2(m_{e gam pi}).

	 Using this function, if the trial variables (set of invariant masses, and solid angles)
	 satisfy maj*r < PDF, then this event is used, where r is a random variable which uniformly
	 distributes in (0,1).


	 If you want to change the minimum energy of photons in the gam-e rest frame,
	 not only do you need to change the definition of Egmin, but also you must change
	 the constant of the majorizing function.

  -----------------------------------------------------------------------------*/

  TLorentzVector Ppeg;
  TLorentzVector Peg;
  TLorentzVector Pe;
  TLorentzVector Pg;
  TLorentzVector Pp;
  TLorentzVector Pn;
  TVector3 Vpeg;

#ifdef LOWECUT
  const double Egmin = mkl*0.001;
  const double majconst = 2e11;
#endif

#define NORMALECUT
#ifdef NORMALECUT
  const double Egmin = mkl*0.01;
  const double majconst = 2e9;
#endif


  const double m0 = Egmin + sqrt(Egmin*Egmin+me*me);
  const double M0 = mp1+m0;

  while(1){

		double msq_peg = M0*M0 + (mkl*mkl-M0*M0)*G4UniformRand();
		
		double m1 = sqrt(msq_peg)-mp1;
		
		// m_eg is distributed by const. times 2 m_eg/(m_eg^2 - m_e^2).
		double msq_eg  = me*me + (m0*m0-me*me)*pow((m1*m1-me*me)/(m0*m0-me*me), G4UniformRand());
		
		
		double ppeg = (mkl*mkl-msq_peg)/mkl/2;
		double epeg = (mkl*mkl+msq_peg)/mkl/2;
		
		Vpeg.SetXYZ(0,0,ppeg/epeg);
		Pn.SetPxPyPzE(0,0,-ppeg, ppeg);
		
		Ppeg.SetPxPyPzE(0,0,ppeg, epeg);
		
		double eeg = (msq_peg+msq_eg-mp1*mp1)/2/sqrt(msq_peg);
		double peg = sqrt(eeg*eeg-msq_eg);
		
		double c  = 2*G4UniformRand()-1;
		double ph = pi*(2*G4UniformRand()-1);
		double s = sqrt(1-c*c);
		Peg.SetPxPyPzE(peg*s*cos(ph), peg*s*sin(ph), peg*c, eeg);
		Pp.SetVect(-Peg.Vect());
		Pp.SetE(sqrt(msq_peg)-eeg);
		
		Pp.Boost(Vpeg);
		Peg.Boost(Vpeg);
		
		double ee = (msq_eg+me*me)/2/sqrt(msq_eg);
		double pe = sqrt(ee*ee-me*me);
		
		c  = 2*G4UniformRand()-1;
		ph = pi*(2*G4UniformRand()-1);
		s = sqrt(1-c*c);
		Pe.SetPxPyPzE(pe*s*cos(ph), pe*s*sin(ph), pe*c, ee);
		Pg.SetVect(-Pe.Vect());
		Pg.SetE(sqrt(msq_eg)-ee);
		
		Pe.Boost(Peg.BoostVector());
		Pg.Boost(Peg.BoostVector());
		
		double PDF = get_PDF(Pe, Pp, Pn, Pg);
		
		double PS = 2*Pn.E()/mkl*2*peg/sqrt(msq_peg)*2*sqrt(msq_eg)*pe/sqrt(msq_eg);
		
		double prob = PS*PDF;
		double maj = majconst*sqrt(msq_eg)/(msq_eg-me*me)/(log((m1*m1-me*me)/(m0*m0-me*me)));
		
		if(prob>maj){
			std::cout << "warning! prob/maj=" << prob/maj <<  ", ang=" << Pe.Angle(Pg.Vect()) <<  "\n";
		}
		
		if(maj*G4UniformRand()<prob)break;
	}



  double gth = acos(2*G4UniformRand()-1);
  double gph = pi*(2*G4UniformRand()-1);

  Pe.RotateY(gth);
  Pe.RotateZ(gph);
  
  Pg.RotateY(gth);
  Pg.RotateZ(gph);
  
  Pp.RotateY(gth);
  Pp.RotateZ(gph);
  
  Pn.RotateY(gth);
  Pn.RotateZ(gph);

	
  // electron
  G4ThreeVector momentum0 =  G4ThreeVector(Pe.X()*MeV,Pe.Y()*MeV,Pe.Z()*MeV);
  G4DynamicParticle * daughterparticle = new G4DynamicParticle(my_daughters[0], momentum0);
  products->PushProducts(daughterparticle);
  
  // pion
  G4ThreeVector momentum1 =  G4ThreeVector(Pp.X()*MeV,Pp.Y()*MeV,Pp.Z()*MeV);
  daughterparticle = new G4DynamicParticle(my_daughters[1], momentum1);
  products->PushProducts(daughterparticle);
    
  // neutrino
  G4ThreeVector momentum2 =  G4ThreeVector(Pn.X()*MeV,Pn.Y()*MeV,Pn.Z()*MeV);
  daughterparticle = new G4DynamicParticle(my_daughters[2], momentum2);
  products->PushProducts(daughterparticle);

  // gamma
  G4ThreeVector momentum3 =  G4ThreeVector(Pg.X()*MeV,Pg.Y()*MeV,Pg.Z()*MeV);
  daughterparticle = new G4DynamicParticle(my_daughters[3], momentum3);
  products->PushProducts(daughterparticle);

#ifdef GSIMDEBUG
  GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
  return products;
}

void GsimPiENuGammaDecayChannel::init_formfactor(){

	double L9 =   6.3e-3 - log(mp1*mp1*mkl*mkl*mkl*mkl*meta*meta/pow(mrho,8))/512/pi/pi;
	double L10 = -4.9e-3 + log(mp1*mp1*mkl*mkl*mkl*mkl*meta*meta/pow(mrho,8))/512/pi/pi;

	const double x = mp1*mp1/mkl/mkl;
	const double y = meta*meta/mkl/mkl;

	double V1 = -8*L9/Fp/Fp - 1.0/(1-x)/(1-x)/32/pi/pi/Fp/Fp*(1.0/3*(53-25*x+2*x*x)+(1+x-x*x+x*x*x)*log(x)/2/(1-x)
		       -(127-93*x+21*x*x-x*x*x)*log(y)/2/(1-x));

	double V2 = 4*(L9+L10)/Fp/Fp + (1+x)/(1-x)/(1-x)/64/pi/pi/Fp/Fp*(1+x+2*x*log(x)/(1-x))
		       -1.0/(1-x)/(1-x)/(1-x)/32/pi/pi/Fp/Fp*(166.0/3*(9-4*x)+(77-x)*x*x/3 + x*(3+2*x)*log(x)/(1-x)-9*(12-x)*(4-x)*(4-x)*log(y)/(1-x));

	double V3 = -1.0/(1-x)/(1-x)/(1-x)/(1-x)/32/pi/pi/Fp/Fp*(2611.0/3-13*x*(34-5*x)-4*x*x*x/3
		       +x*(2+3*x+x*x)*log(x)/(1-x)-27*(7-x)*(4-x)*(4-x)*log(y)/(1-x));

	double A2 = -1.0/8/pi/pi/Fp/Fp;

	V[0] = 0;
	V[1] = V1;
	V[2] = V2;
	V[3] = V3;
	A[0] = 0;
	A[1] = 0;
	A[2] = A2;
	A[3] = 0;

}



double GsimPiENuGammaDecayChannel::get_PDF(const TLorentzVector& Pe, const TLorentzVector& Pp, 
	const TLorentzVector& Pn, const TLorentzVector& Pg){

	TLorentzVector PW(0,0,0,mkl);
	PW -= (Pp+Pg);

	double a = Pp.E()/mkl; // a
	double b = Pg.E()/mkl; // b
	double c = Pe.E()/mkl; // c
	double d = Pn.E()/mkl; // d
	double e = Pp*Pg/mkl/mkl; // e
	double f = Pp*Pe/mkl/mkl; // f
	double g = Pp*Pn/mkl/mkl; // g
	double h = Pg*Pe/mkl/mkl; // h
	double j = Pg*Pn/mkl/mkl; // j
	double k = Pe*Pn/mkl/mkl; // k
	double l = (mkl-Pp.E()-Pg.E())/mkl; // l
	double m = Pp*PW/mkl/mkl; // m
	double n = Pg*PW/mkl/mkl; // n
	
	const double r = mp1*mp1/mkl/mkl;
	double t = mkl*mkl+mp1*mp1-2*mkl*Pp.E();

#if 0
	double b4 = 2*e*(c*e*k*(e-2*g)+e*(-b*k*(f+g)+a*k*(j-h+2*k)+l*(g*h+f*j-e*k))+c*m*(2*g*h-e*j)+k*m*(b*e-2*a*h) + d*(e+2*f)*(-e*k+h*m))
		        +2*n*(-(e*(c*g*(e-2*f)-2*b*f*g+a*(-e*k+2*f*(j+k)))) + r*(c*(-2*g*h+e*j)+k*(-b*e+2*a*h)) + d*(e+2*f)*(e*f-h*r));
	b4 *= 1/e/h;
	
	double b51 = h*(d*e+b*g+c*g) - j*(c*f+a*h) + k*(b*f-a*h);
	b51 *= 4/h;

	double b54 = k*(-d*e*e+a*(e-2*f)*(j-h) +b*(2*f*(g-h)-e*(g+f))) + k*k*(-2*r*b+2*a*e)
		        +l*(g*h*(e-2*f)+(e+2*f)*(f*j-e*k)+2*r*h*k) + m*(d*e*h+b*e*k+2*b*f*k-2*a*h*k)
		        +n*(f*(d*e+2*b*g-2*a*j)-r*(b*k+d*h)+a*e*k) + c*(k*(e*e-2*r*h+2*r*j)+m*(2*g*h-2*f*j)+r*j*n+e*(2*f*k-j*m-g*(2*k+n)));
	b54 *= 2/h;
	
	double c4 = 8*e*e*k*(e*f+c*g-g*h-f*j-a*k+e*k)+4*e*n*(e*k*(f+g-2*m)+m*(-2*d*f-2*c*g+g*h+f*j+2*a*k))+4*n*n*(r*(d*f+c*g+k*(e-a))-2*e*f*g);
	c4 *= 1/e;

	double c51 = 4*(b*k*(g-f)-e*h*k+a*k*(h-j)+e*j*k-g*h*l+f*j*l-g*h*n+f*j*n);
	double c54 = 4*n*(e*k*(g-h)+r*k*(h-j)+m*(f*j-g*h));
	
	double d4 = (2*e*e*g+2*e*h*(f+g)-r*e*(h+j)-2*r*h*(h+j))*2/e/h;
	double d51 = 4*f/h;
	double d54 = (g*(2*e+4*f)+2*h*(g-f)-r*(h+j*2*k))*2/h;
	double e4 = (r*(h+j)-e*(f+g))*4*n/e;
	double e51 = 4*(g-f);
	double e54 = 4*n*(f-g);

#endif

	double a1 = 2*b*d*e*(e+f) - e*(h*(2*a*d-g)+j*(e+f)+k*(e+2*f)) + h*k*r + 2*c*d*(e*e+2*e*f-h*r);
	a1 *= 4/e/e/h;

	double a2 = 4*e*(e*k*(2*d*(2*c-h) + h-j*(1+2*c)-2*k + 2*b*(d-h+k)) -2*e*h*l*(d-j) + 2*h*m*(d*h+c*(j-2*d)+k*(1-b)))
		       -4*n*(2*b*d*e*f + e*h*(2*d*(-a+e+f) + g) -e*f*j -2*e*k*(f+a*h)+2*h*k*r+2*c*(2*d*e*f+e*g*h-2*d*h*r));
	a2 *= 1/e/e/h;

	double a3 = -2*e*e*k*(2*(c-h)*(d-j)+k*(2*b-1)) -2*e*n*(e*k*(c+d-h-j-l)+m*(d*h+c*(j-2*d)+k*(1-b))) + n*n*(2*e*(d*f+c*g-a*k)+r*(k-2*c*d));
	a3 *= 4/e/e;

	double b1 = e*(d*h*(e+2*f)+g*h*(b+c)+j*(c*f-a*h)-k*(b*f+a*h)) -h*r*(d*h+c*j-b*k);
	b1 *= 4/e/h;

	double b2 = j*(c*e*k-e*h*l-c*h*m) + b*k*(e*(h-k)+h*m) + n*h*(c*g-a*k) + d*h*(e*k-h*m+n*(e+f));
	b2 *= 4/e/h;

	double b52 = c*e*k*(j-h) -b*k*(h*(e+f-g)+e*k) + h*(a*k*(h-j)+l*(-g*h+e*j+f*j+e*k)-d*e*n);
	b52 *= 4/e/h;
	
	double c1 = e*(e*k*(d-h-j-l)+m*(2*h*j+b*k-d*h)+c*(e*k-j*m)) -e*n*(g*(c+h)+f*(d+j)-a*k) + n*r*(d*h+c*j-b*k);
	c1 *= 4/e;

	double c2 = 8*e*k*(2*h*j+b*k-d*h-c*j)+4*n*(e*k*(c+d-2*(h+j)-l)+m*(d*h+c*j-b*k)) -4*n*n*(d*f+c*g-a*k);
	c2 *= 1/e;
	
	double c52 = b*k*(f-g)-k*(a-2*e)*(h-j)+l*(g*h-f*j);
	c52 *= 4*n/e;

	double d1 = (-e*f+r*h)*4/e/h;
	double d2 = (h*(f+g+h+j)+e*(h-k))*4/e/h;
	double d52 = (h*(j-f+g-h)-e*(h+k))*4/e/h;
	double e1 = (-r*n+e*(f+g))*4/e;
	double e2 = (2*e*k-n*(f+g+h+j))*4/e;

	double e52 = (f-g+h-j)*4*n/e;
	
	double ft = f_plus(t);
	double delft = mkl*mkl/(Pg*PW)*(ft-f_plus(PW*PW));
	double xi = 0;//Pg.Vect()*(Pp.Vect().Cross(Pe.Vect()))/mkl/mkl/mkl;

	double PDF = a1*ft*ft + a2*ft*delft + a3*delft*delft 
		+ (b1*V[1]+b2*V[2]+b52*A[2])*ft    +  (c1*V[1]+c2*V[2]+c52*A[2])*delft
		+ xi*(d1*V[1]+d2*V[2]+d52*A[2])*ft + xi*(e1*V[1]+e2*V[2]+e52*A[2])*delft;

	return PDF;
}


