#include "PhotocathodeSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"
#include "G4Track.hh"

PhotocathodeSD::PhotocathodeSD(G4String name)
: G4VSensitiveDetector(name)
{
}

PhotocathodeSD::~PhotocathodeSD()
{
}


void PhotocathodeSD::Initialize(G4HCofThisEvent*) {}

G4bool PhotocathodeSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4Track* track = step->GetTrack();
  
  if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
      return false;
  }

  G4double energy = track->GetKineticEnergy();
  G4double time = track->GetGlobalTime();

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, energy);
  analysisManager->FillNtupleDColumn(1, time);
  analysisManager->AddNtupleRow();

  track->SetTrackStatus(fStopAndKill);

  return true;
}

void PhotocathodeSD::EndOfEvent(G4HCofThisEvent*) {}
