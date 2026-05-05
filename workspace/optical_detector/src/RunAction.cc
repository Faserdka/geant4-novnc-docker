#include "RunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include <fstream>


RunAction::RunAction()
: G4UserRunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv");
  analysisManager->CreateNtuple("Hits", "Optical Photon Hits");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->CreateNtupleDColumn("GlobalTime");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile("hits.csv");
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // Сохраняем информацию о количестве запущенных событий
  std::ofstream runInfo("run_info.txt");
  if (runInfo.is_open()) {
      runInfo << run->GetNumberOfEvent();
      runInfo.close();
  }
}

