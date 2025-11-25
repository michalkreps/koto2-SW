/**
 *  @file
 *  @brief  GsimKLpi02gamma
 *  $Id$ 
 *  $Log$ 
 */

#include "GsimParticle/GsimKLpi02gamma.h"
#include "GsimParticle/GsimKPiGGDecayChannel.h"

#include "G4VDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4SystemOfUnits.hh"

GsimKLpi02gamma* GsimKLpi02gamma::theInstance = 0;

GsimKLpi02gamma* GsimKLpi02gamma::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "KLpi02gamma";

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
              "meson",               0,             0,         2000130,
                false,            life,          NULL,
                false,          "kaon",           2000130);

   //create Decay Table
   
   
   G4DecayTable* table = 0;
   table = new G4DecayTable();
   
   // create decay channels
   G4VDecayChannel** mode = new G4VDecayChannel*[1];
   mode[0] = new GsimKPiGGDecayChannel("kaon0L",1.,"pi0","gamma","gamma");

   for (G4int index=0; index <1; index++ ) {
     table->Insert(mode[index]);
   }
   delete [] mode;
   
   anInstance->SetDecayTable(table);
  }
  theInstance = reinterpret_cast<GsimKLpi02gamma*>(anInstance);
  return theInstance;
}

GsimKLpi02gamma*  GsimKLpi02gamma::KLpi02gammaDefinition()
{
  return Definition();
}

GsimKLpi02gamma*  GsimKLpi02gamma::KLpi02gamma()
{
  return Definition();
}
