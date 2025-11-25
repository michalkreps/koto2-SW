/**
 *  @file
 *  @brief  GsimKLpipi0enu
 *  $Id: $
 *  $Log: $
 *  N Shimizu
 */

#include "GsimParticle/GsimKLpipi0enu.h"
#include "GsimParticle/GsimKLpipi0enuDecayChannel.h"

#include "G4PhaseSpaceDecayChannel.hh"
#include "G4VDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"

using namespace CLHEP;

GsimKLpipi0enu* GsimKLpipi0enu::theInstance = 0;

GsimKLpipi0enu* GsimKLpipi0enu::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "KLpipi0enu";
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

    G4ParticleDefinition* particle= pTable->FindParticle("kaon0L");
    double mass =particle->GetPDGMass();
    double width=particle->GetPDGWidth();
    double life =particle->GetPDGLifeTime();
    
   anInstance = new G4ParticleDefinition(
                 name,            mass,         width,         0.0,
                    0,              -1,             0,
                    1,               0,             0,
              "meson",               0,             0,         19000130,
                false,            life,          NULL,
                false,          "kaon",           19000130);

   //create Decay Table
   
   
   G4DecayTable* table = 0;
   table = new G4DecayTable();
   
   // create decay channels
   G4VDecayChannel** mode = new G4VDecayChannel*[2];
   
   // kaon0L -> pi- + pi0 + e+ + nu_e 
   mode[0] = new GsimPiPi0ENuDecayChannel("kaon0L",0.5,"pi-","pi0", "e+", "nu_e");
   // kaon0L -> pi+ + pi0 + e- + anti_nu_e + gamma
   mode[1] = new GsimPiPi0ENuDecayChannel("kaon0L",0.5,"pi+","pi0", "e-", "anti_nu_e");

   for (G4int index=0; index <2; index++ ) {
     table->Insert(mode[index]);
   }
   delete [] mode;
   
   anInstance->SetDecayTable(table);
  }
  theInstance = reinterpret_cast<GsimKLpipi0enu*>(anInstance);
  return theInstance;
}

GsimKLpipi0enu*  GsimKLpipi0enu::KLpipi0enuDefinition()
{
  return Definition();
}

GsimKLpipi0enu*  GsimKLpipi0enu::KLpipi0enu()
{
  return Definition();
}

