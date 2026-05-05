#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="opticalphoton");
  
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(2.5*eV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Isotropic distribution
  G4double cosTheta = 2*G4UniformRand() - 1., phi = 2*CLHEP::pi*G4UniformRand();
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double ux = sinTheta*std::cos(phi), uy = sinTheta*std::sin(phi), uz = cosTheta;

  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux, uy, uz));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
