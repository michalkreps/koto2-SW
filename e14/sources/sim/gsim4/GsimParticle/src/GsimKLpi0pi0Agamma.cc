/**
 *  @file
 *  @brief  GsimKLpi0pi0Agamma
 *  $Id:
 *  $Log:
 */

#include "GsimParticle/GsimKLpi0pi0Agamma.h"
#include "GsimParticle/GsimKPi3DecayChannel.h"

#include "G4VDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"

GsimKLpi0pi0Agamma* GsimKLpi0pi0Agamma::theInstance = 0;

GsimKLpi0pi0Agamma* GsimKLpi0pi0Agamma::Definition()
{
	
  if (theInstance !=0) return theInstance;
  const G4String name = "KLpi0pi0Agamma";
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
              "meson",               0,             0,      13000130,
                false,            life,          NULL,
                false,          "kaon",     13000130);

   //create Decay Table
   
   
   G4DecayTable* table = 0;
   table = new G4DecayTable();
   
   // create decay channels
   G4VDecayChannel** mode = new G4VDecayChannel*[1];
   // kaon0L -> pi0 + pi0 + pi0
   mode[0] = new GsimKPi3DecayChannel("kaon0L",1.,"pi0","pi0","pi0Agamma");

   for (G4int index=0; index <1; index++ ) {
     table->Insert(mode[index]);
   }
   delete [] mode;
   
   anInstance->SetDecayTable(table);
  }
  theInstance = reinterpret_cast<GsimKLpi0pi0Agamma*>(anInstance);
  return theInstance;
}

GsimKLpi0pi0Agamma*  GsimKLpi0pi0Agamma::KLpi0pi0AgammaDefinition()
{
  return Definition();
}

GsimKLpi0pi0Agamma*  GsimKLpi0pi0Agamma::KLpi0pi0Agamma()
{
  return Definition();
}

