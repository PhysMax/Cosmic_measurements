#include "SimpleDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "SimpleRootWriter.hh"
#include <iostream>

using namespace CLHEP;

SimpleDetector::SimpleDetector(const G4String& name) 
  : G4VSensitiveDetector(name)
{
    //std::cout << "hello detector " << SensitiveDetectorName << std::endl;
}
SimpleDetector::~SimpleDetector() 
{}

void SimpleDetector::Initialize(G4HCofThisEvent* hce)
{

}

G4bool SimpleDetector::ProcessHits(G4Step* step, 
                           G4TouchableHistory*)
{  
  G4int pdgcode =  step->GetTrack()->GetDefinition()->GetPDGEncoding();
  G4double energy =  step->GetTrack()->GetKineticEnergy();
  G4double time =  step->GetPostStepPoint()->GetGlobalTime();
  G4int id = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();

  G4int mid = step->GetTrack()->GetParentID();

  if (mid == 1) {
    SimpleRootWriter::GetPointer()->FillDeltaEnergy(energy);
  }

  G4double edep = step->GetTotalEnergyDeposit();
  if (SensitiveDetectorName == "sc1sd") {
      SimpleRootWriter::GetPointer()->AddEdep1(edep);
  }
  else if (SensitiveDetectorName == "sc2sd") {
      SimpleRootWriter::GetPointer()->AddEdep2(edep);
  }
  else {
      std::cout << "Unknown sensitive detector name: " << SensitiveDetectorName << std::endl;
      exit(1);
  }


//  G4double 

  //if 1:
  //edep1 += edep;
  //if 2:
  //edep2 += edep;
  
//  SimpleRootWriter::GetPointer()->AddEdep1(



  if(pdgcode != 0) return false; // take only optical photons

//  SimpleRootWriter::GetPointer()->h1[0]->Fill(id);      
//  SimpleRootWriter::GetPointer()->h1[1]->Fill(energy/eV);      
//  SimpleRootWriter::GetPointer()->h1[2]->Fill(time/microsecond);      

  return true;
}

void SimpleDetector::EndOfEvent(G4HCofThisEvent*)
{

}

