#include "SimplePrimaryGeneratorAction.hh"
#include "SimpleRootWriter.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Gamma.hh"
#include "G4MuonMinus.hh"
#include "TMath.h"


SimplePrimaryGeneratorAction* SimplePrimaryGeneratorAction::fPtr = 0;

SimplePrimaryGeneratorAction::SimplePrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  f = new TF1("f", "TMath::Cos(x)*TMath::Exp(-0.2/TMath::Cos(x))",0 , TMath::Pi()/2);  
  rnd = new TRandom3(5);
  fPtr = this;
  fParticleGun  = new G4ParticleGun(1);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(particle);
}

SimplePrimaryGeneratorAction::~SimplePrimaryGeneratorAction()
{
  delete fParticleGun;
}

void SimplePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
  double x = rnd->Uniform(-1,1);
  double y = rnd->Uniform(-1,1); 		 

  double theta = f->GetRandom(0, TMath::Pi()/2);
  double fi = rnd->Uniform(0, 2*TMath::Pi());
  double mx = TMath::Sin(theta)*TMath::Cos(fi);
  double my = TMath::Sin(theta)*TMath::Sin(fi);
  double mz = TMath::Cos(theta);
  fParticleGun->SetParticlePosition(G4ThreeVector(x,y,-1)*CLHEP::m);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(mx,my,mz));
  fParticleGun->SetParticleEnergy(100*MeV);

  fParticleGun->GeneratePrimaryVertex(anEvent);

  SimpleRootWriter::GetPointer()->FillTheta(theta);
}
