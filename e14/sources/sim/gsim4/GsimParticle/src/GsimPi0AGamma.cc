#include "G4SystemOfUnits.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "GsimParticle/GsimPi0AGamma.h"


GsimPi0AGamma* GsimPi0AGamma::theInstance = 0;

GsimPi0AGamma* GsimPi0AGamma::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "pi0Agamma";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
    {
      // create particle
      //
      //    Arguments for constructor are as follows
      //               name             mass          width         charge
      //             2*spin           parity  C-conjugation
      //          2*Isospin       2*Isospin3       G-parity
      //               type    lepton number  baryon number   PDG encoding
      //             stable         lifetime    decay table
      //             shortlived      subType    anti_encoding

      G4ParticleDefinition* particle= pTable->FindParticle("pi0");
      double mass =particle->GetPDGMass();
      double width=particle->GetPDGWidth();
      double life =particle->GetPDGLifeTime();
      anInstance = new G4ParticleDefinition(
         name,        mass,            width,  0, 
	    0,          -1,               +1,          
	    2,           0,               -1,             
	    "meson",     0,                0,          1000111,
	    false,       life,             NULL,
	    false,       "pi",    1000111
	    );

      
      //name,   0.1349766*GeV,   7.8e-06*MeV,         0.0,
      //0,              -1,            +1,
      //2,               0,            -1,
      //"meson",               0,             0,         111,
      //false,       8.4e-8*ns,          NULL,
      //false,            "pi",          111);

 
      
      //create Decay Table 
      G4DecayTable* table = new G4DecayTable();
      
      // create a decay channel
      G4VDecayChannel* mode = new G4PhaseSpaceDecayChannel("pi0Agamma",1.0,2,"gamma","Aprime");
      table->Insert(mode);
      anInstance->SetDecayTable(table);
    }
  
  theInstance = reinterpret_cast<GsimPi0AGamma*>(anInstance);
  return theInstance;
}
