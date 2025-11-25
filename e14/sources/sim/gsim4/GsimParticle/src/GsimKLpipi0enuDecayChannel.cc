/**
*  @file
*  @brief  GsimKL2pi0gammaDecayChannel
*  $Id$ 
*  $Log$
*  N Shimizu
*/


#include "G4VDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayProducts.hh"
#include "Randomize.hh"
#include "G4LorentzVector.hh"
#include "G4LorentzRotation.hh"

#include "GsimParticle/GsimKLpipi0enuDecayChannel.h"

using CLHEP::MeV;


GsimPiPi0ENuDecayChannel::GsimPiPi0ENuDecayChannel(
	const G4String& theParentName,
	G4double        theBR,
	const G4String& thePionName,
	const G4String& thePion0Name,
	const G4String& theEName,
	const G4String& theNuName
)
	:G4VDecayChannel("KLpipi0enu Decay",theParentName,
		theBR,  4,
		thePionName,  thePion0Name,  theEName, theNuName)
{
#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

	m_mkl=0.;
	m_mp1=0.;
	m_mp0=0.;
	m_ml=0.;
	m_pi = CLHEP::pi;
	m_max_prob = 0;
	m_i=0;

#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

GsimPiPi0ENuDecayChannel::~GsimPiPi0ENuDecayChannel()
{
#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif


#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
}

G4DecayProducts* GsimPiPi0ENuDecayChannel::DecayIt(G4double) 
{
#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugEnter(__PRETTY_FUNCTION__);
#endif

	//GetParent and GetDaughers call FillParent/Daughers
	G4ParticleDefinition* my_parent = GetParent();
	G4ParticleDefinition* my_daughters[4];
	my_daughters[0]=GetDaughter(0);
	my_daughters[1]=GetDaughter(1);
	my_daughters[2]=GetDaughter(2);
	my_daughters[3]=GetDaughter(3);
	
	//create parent G4DynamicParticle at rest
	G4ThreeVector dummy;
	G4DynamicParticle * parentparticle 
		= new G4DynamicParticle(my_parent, dummy, 0.0);

	//create G4Decayproducts
	G4DecayProducts *products = new G4DecayProducts(*parentparticle);
	delete parentparticle;

	if(m_mkl==0) {
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4ParticleDefinition* particle = 0;
		particle = particleTable->FindParticle("kaon0L");
		m_mkl=particle->GetPDGMass()/MeV;
	}
	if(m_mp1==0) {
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4ParticleDefinition* particle = 0;
		particle = particleTable->FindParticle("pi-");
		m_mp1=particle->GetPDGMass()/MeV;
	}
	if(m_mp0==0) {
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4ParticleDefinition* particle = 0;
		particle = particleTable->FindParticle("pi0");
		m_mp0=particle->GetPDGMass()/MeV;
	}
	if(m_ml==0) {
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4ParticleDefinition* particle = 0;
		particle = particleTable->FindParticle("e-");
		m_ml=particle->GetPDGMass()/MeV;
	}

	/* ---------------------------------------------------------------------------

	  Dynamics of Neutral Ke4 decay is implemented.
	  
	  Ref.1: Phys. Rev. D67 072004 (2003).
		This describes the general formula of K -> pi pi e nu decays
		The diff. decay width is shown in Eq.(8) is used. 
		The form factors F1, F2, F3 are taken from Ref.2

	  Ref.2: Phys. Lett. B595, 75 (2004).
		This is paper reports the experimental measurement of KL -> pi pi0 e nu.
		Paper explains they used experimental parameters (Ep.(4)) from Ref.3,
		but I guess they simply adopted the formula in this reference, because
		Ref. 3 measured the parameters using the charged Ke4 mode. 
		It may be common because of isospin, but some of parameters (g,h in Eg.4)
		describe the assymetry of two pions, thus I think it is not so natural to
	    expect the pipi0 assymetry and pi+pi- assymetry are same.
		This paper also reports fit result of the parameters using their statistics.

	  Ref.3: Phys. Rev. D15, 574 (1977).
	 	This paper reports the measurement of form factor using K+->pi+pi-e+ nu decay. 


	 In this code, parameters of both Ref.2 and Ref.3 can be used, but by default, 
	 the result of Ref.2 is used.

	 Diff. decay width of Ke4 is written as

	 dGamma propto (dynamics)*X*sigpi*(Msqenu-me*me)/Msqenu*dMsq(enu)*dMsq(pipi)*dcosth(pi)*dcosth(e)*dPhi,
	 where th(pi) is an angle of pi in pipi rest frame with respect to pipi--enu axis,
	 th(e) is an angle of e in enu rest frame with respect to pipi--enu axis,
	 and Phi is a titing angle of decay plane of pipi and enu (See Fig.1 of Ref.1).

	 sigpi = sqrt(1-4*mpi^2/mpipi^2)
	 X = M*Q (Q is momentum of enu or pipi system in KL rest frame).
	 In Ref.2, (Msqenu-me*me)/Msqenu is not included. They ignores the small mass of electron. 
	 

	 ------------------------------------------------------------------------------*/


	double th;
	double ph;

	TLorentzVector Pp0;
	TLorentzVector Ppi;
	TLorentzVector Ppipi;
	TLorentzVector Plnu;
	TLorentzVector Pl;
	TLorentzVector Plp;
	TLorentzVector Pnu;
	TLorentzVector Pkl;
	TVector3 Vpipi;
	TVector3 Vlnu;

	double mpipisq;
	double mpipi;
	double mlnusq;
	double mlnu;
	double th_pi;
	double th_l;
	double phi;

	double pst;

	double pst_l;
	double pst_pi;

	double prob;

	double X;
	double sigpi;
	double PL;

	const double maj = 4.5e-15;
	while(1){

		mlnusq = m_ml*m_ml + ((m_mkl-m_mp0-m_mp1)*(m_mkl-m_mp0-m_mp1)-m_ml*m_ml)*G4UniformRand();
		mpipisq = (m_mp0+m_mp1)*(m_mp0+m_mp1) + ((m_mkl-m_ml)*(m_mkl-m_ml)-(m_mp0+m_mp1)*(m_mp0+m_mp1))*G4UniformRand();

		mpipi = sqrt(mpipisq);
		mlnu = sqrt(mlnusq);
		if(mlnu+mpipi>m_mkl)continue;

		pst = sqrt((m_mkl*m_mkl-(mlnu-mpipi)*(mlnu-mpipi))*(m_mkl*m_mkl-(mlnu+mpipi)*(mlnu+mpipi)))/2/m_mkl;

		th_pi = acos(2*G4UniformRand()-1);
		th_l  = acos(2*G4UniformRand()-1);
		phi   = m_pi*(2*G4UniformRand()-1);

		PL = (m_mkl*m_mkl-mpipisq-mlnusq)*0.5;

		X = sqrt(PL*PL-mpipisq*mlnusq);
		sigpi = sqrt(1-4*m_mp0*m_mp0/mpipisq);
		prob = X*sigpi*(mlnusq-m_ml*m_ml)/mlnusq*probJ5(mpipisq, mlnusq, th_pi, th_l, phi)*maj; 

//#define _DEBUG
#ifdef _DEBUG
		if(prob>1){
			std::cout << "warning! prob exceeds 1! fac = " << prob << "\n";
		}

		if(max_prob<prob){
			max_prob = prob;
			if(i>10) std::cout << "max_prob = " <<  max_prob << "\n";
		}
		m_i++;
#endif

		if(G4UniformRand()>prob)continue;


		pst_l = (mlnusq-m_ml*m_ml)/2/mlnu;
		pst_pi = sqrt((mpipisq-(m_mp1-m_mp0)*(m_mp1-m_mp0))*(mpipisq-(m_mp1+m_mp0)*(m_mp1+m_mp0)))/2/mpipi;

		Pl.SetPxPyPzE(0,  pst_l*sin(th_l),    pst_l*cos(th_l), sqrt(pst_l*pst_l+m_ml*m_ml));
		Pnu.SetPxPyPzE(0,-pst_l*sin(th_l),   -pst_l*cos(th_l), pst_l);
		Ppi.SetPxPyPzE(0, pst_pi*sin(th_pi), -pst_pi*cos(th_pi), sqrt(pst_pi*pst_pi+m_mp1*m_mp1));
		Pp0.SetPxPyPzE(0,-pst_pi*sin(th_pi),  pst_pi*cos(th_pi), sqrt(pst_pi*pst_pi+m_mp0*m_mp0));
		
		Ppi.RotateZ(phi);
		Pp0.RotateZ(phi);

		Vpipi.SetXYZ(0,0, -pst/sqrt(pst*pst+mpipisq));
		Vlnu.SetXYZ(0,0,   pst/sqrt(pst*pst+mlnusq));

		Ppi.Boost(Vpipi);
		Pp0.Boost(Vpipi);

		Pl.Boost(Vlnu);
		Pnu.Boost(Vlnu);

		// rotate entire space
		th = acos(2*G4UniformRand()-1);
		ph = m_pi*(2*G4UniformRand()-1);

		Ppi.RotateY(th); Ppi.RotateZ(ph);
		Pp0.RotateY(th); Pp0.RotateZ(ph);
		Pl.RotateY(th);  Pl.RotateZ(ph);
		Pnu.RotateY(th); Pnu.RotateZ(ph);


		break;
	}

	

	// pion
	G4ThreeVector momentum0 =  G4ThreeVector(Ppi.X()*MeV,Ppi.Y()*MeV,Ppi.Z()*MeV);
	G4DynamicParticle * daughterparticle = new G4DynamicParticle(my_daughters[0], momentum0);
	products->PushProducts(daughterparticle);
	
	// pion0
	G4ThreeVector momentum1 =  G4ThreeVector(Pp0.X()*MeV,Pp0.Y()*MeV,Pp0.Z()*MeV);
	daughterparticle = new G4DynamicParticle(my_daughters[1], momentum1);
	products->PushProducts(daughterparticle);

	// electron
	G4ThreeVector momentum2 =  G4ThreeVector(Pl.X()*MeV,Pl.Y()*MeV,Pl.Z()*MeV);
	daughterparticle = new G4DynamicParticle(my_daughters[2], momentum2);
	products->PushProducts(daughterparticle);

	// nu
	G4ThreeVector momentum3 =  G4ThreeVector(Pnu.X()*MeV,Pnu.Y()*MeV,Pnu.Z()*MeV);
	daughterparticle = new G4DynamicParticle(my_daughters[3], momentum3);
	products->PushProducts(daughterparticle);

#ifdef GSIMDEBUG
	GsimMessage::getInstance()->debugExit(__PRETTY_FUNCTION__);
#endif
	return products;
}


void GsimPiPi0ENuDecayChannel::FormF(double* f, double spi, double se, double th_pi){

  
#ifdef USECERNMODEL
        double PL = (m_mkl*m_mkl-spi-se)*0.5;
        double X = sqrt(PL*PL-spi*se);
        double sigpi = sqrt(1-4*m_mp0*m_mp0/spi);
        double gm_al = X/(PL*sigpi);
	const double dlts = 0.205;
	const double dltp = 0;
	const double fs = 1;
	const double g  = 0.855;
	const double gp = 0.868;
	const double h  = -0.48;
	double fp = (gp-g)/gm_al;
#endif
#define USENA48MODEL
#ifdef USENA48MODEL
	const double scale = 2.5;
	const double dlts = 0;
	const double dltp = 0;
	const double fs = 0.052*scale;
	double       g  = (1+0.087*(spi/4/m_mp1/m_mp1-1))*scale;
	const double h  = -0.32*scale;
	const double fp = -0.051*scale;
#endif

	// ReF
	f[0] = fs*cos(dlts) + fp*cos(dltp)*cos(th_pi);

	// ImF
	f[1] = fs*sin(dlts) + fp*sin(dltp)*cos(th_pi);

	// ReG
	f[2] = g*cos(dltp);

	// ImG
	f[3] = g*sin(dltp);

	// ReH
	f[4] = h*cos(dltp);

	// ImH
	f[5] = h*sin(dltp);


	return;

}

double GsimPiPi0ENuDecayChannel::probJ5(double spi, double se, double th_pi, double th_l, double phi){



	double PL = (m_mkl*m_mkl-spi-se)*0.5;
	double X = sqrt(PL*PL-spi*se);
	double sigpi = sqrt(1-4*m_mp0*m_mp0/spi);

	double F[6];
	FormF(F, spi, se, th_pi);

	const double r_F = F[0];
	const double i_F = F[1];
	const double r_G = F[2];
	const double i_G = F[3];
	const double r_H = F[4];
	const double i_H = F[5];


	double r_F1 = X*r_F + sigpi*PL*cos(th_pi)*r_G;
	double i_F1 = X*i_F + sigpi*PL*cos(th_pi)*i_G;

	double r_F2 = sigpi*sqrt(spi*se)*r_G;
	double i_F2 = sigpi*sqrt(spi*se)*i_G;

	double r_F3 = sigpi*X*sqrt(spi*se)*r_H/m_mkl/m_mkl;
	double i_F3 = sigpi*X*sqrt(spi*se)*i_H/m_mkl/m_mkl;


	double I1 =  (2*(r_F1*r_F1+i_F1*i_F1)+3*(r_F2*r_F2+i_F2*i_F2+r_F3*r_F3+i_F3*i_F3)*sin(th_pi)*sin(th_pi))/8;
	double I2 = -(2*(r_F1*r_F1+i_F1*i_F1)-(r_F2*r_F2+i_F2*i_F2+r_F3*r_F3+i_F3*i_F3)*sin(th_pi)*sin(th_pi))/8;
	double I3 = -((r_F2*r_F2+i_F2*i_F2)-(r_F3*r_F3+i_F3*i_F3))*sin(th_pi)*sin(th_pi)/4;
	double I4 =  0.5*(r_F1*r_F2+i_F1*i_F2)*sin(th_pi);
	double I5 = -(r_F1*r_F3+i_F1*i_F3)*sin(th_pi);
	double I6 = -(r_F2*r_F3+i_F2*i_F3)*sin(th_pi)*sin(th_pi);
	double I7 = -(r_F1*i_F2-i_F1*r_F2)*sin(th_pi);
	double I8 =  0.5*(r_F1*i_F3-i_F1*r_F3)*sin(th_pi);
	double I9 = -0.5*(r_F2*i_F3-i_F2*r_F3)*sin(th_pi)*sin(th_pi);

	double J5 = I1 + I2*cos(2*th_l) + I3*sin(th_l)*sin(th_l)*cos(2*phi)
		+I4*sin(2*th_l)*cos(phi) + I5*sin(th_l)*cos(phi) + I6*cos(th_l)
		+I7*sin(th_l)*sin(phi) + I8*sin(2*th_l)*sin(phi) 
		+I9*sin(th_l)*sin(th_l)*sin(phi)*sin(phi);

	return J5;
}




