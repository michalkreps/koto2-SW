#include "GsimParticle/GsimAprime.h"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

GsimAprime* GsimAprime::theInstance = 0;
double GsimAprime::s_mass = 0*GeV;

GsimAprime::GsimAprime(){
  
}

GsimAprime::~GsimAprime(){
}

GsimAprime* GsimAprime::Initialize(double Amass){
  GsimAprime::s_mass = Amass;
  return GsimAprime::Definition();
}

GsimAprime* GsimAprime::Definition(){
  
  if (theInstance !=0) return theInstance;
  const G4String name = "Aprime";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0){
    
    
    // create particle
    //
    //    Arguments for constructor are as follows
    //               name             mass          width         charge
    //             2*spin           parity  C-conjugation
    //          2*Isospin       2*Isospin3       G-parity
    //               type    lepton number  baryon number   PDG encoding
    //             stable         lifetime    decay table
    //             shortlived      subType    anti_encoding
    anInstance = new G4ParticleDefinition(
					  name, s_mass, 0.0*eV,   0, 
					  2,                -1,   -1,          
					  0,                0,     0,             
					  "boson",          0,     0,          900022,
					  true,       0.0,             NULL,
					  false,     "photon",   2000022
					  );
    
    
  }
  
  theInstance = reinterpret_cast<GsimAprime*>(anInstance);
  return theInstance;
}

GsimAprime* GsimAprime::AprimeDefinition()
{
  return Definition();
}

GsimAprime* GsimAprime::Aprime()
{
  return Definition();
}
