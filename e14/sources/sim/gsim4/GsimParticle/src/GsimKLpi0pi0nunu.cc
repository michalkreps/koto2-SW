/**
 *  @file
 *  @brief  GsimKLpi0pi0nunu
 *  $Id$
 *  $Log$
 */

#include "GsimParticle/GsimKLpi0pi0nunu.h"
#include "GsimParticle/GsimKPiPiNuNuDecayChannel.h"

#include "G4VDecayChannel.hh"
#include "G4ParticleTable.hh"
#include "G4DecayTable.hh"
#include "G4SystemOfUnits.hh"

GsimKLpi0pi0nunu* GsimKLpi0pi0nunu::theInstance = 0;

GsimKLpi0pi0nunu* GsimKLpi0pi0nunu::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "KLpi0pi0nunu";

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
              "meson",               0,             0,         11000130,
                false,            life,          NULL,
                false,          "kaon",           11000130);

   //create Decay Table
   
   
   G4DecayTable* table = 0;
   table = new G4DecayTable();
   
   // create decay channels
   G4VDecayChannel** mode = new G4VDecayChannel*[3];
   // kaon0L -> pi0 + pi0 + nu_e + anti_nu_e
   mode[0] = new GsimKPiPiNuNuDecayChannel("kaon0L",1./3.,"pi0","pi0","nu_e","anti_nu_e");
   // kaon0L -> pi0 + pi0 + nu_mu + anti_nu_mu
   mode[1] = new GsimKPiPiNuNuDecayChannel("kaon0L",1./3.,"pi0","pi0","nu_mu","anti_nu_mu");
   // kaon0L -> pi0 + pi0 + nu_tau + anti_nu_tau
   mode[2] = new GsimKPiPiNuNuDecayChannel("kaon0L",1./3.,"pi0","pi0","nu_tau","anti_nu_tau");
   
   for (G4int index=0; index <3; index++ ) {
     table->Insert(mode[index]);
   }
   delete [] mode;
   
   anInstance->SetDecayTable(table);
  }
  theInstance = reinterpret_cast<GsimKLpi0pi0nunu*>(anInstance);
  return theInstance;
}

GsimKLpi0pi0nunu*  GsimKLpi0pi0nunu::KLpi0pi0nunuDefinition()
{
  return Definition();
}

GsimKLpi0pi0nunu*  GsimKLpi0pi0nunu::KLpi0pi0nunu()
{
  return Definition();
}
