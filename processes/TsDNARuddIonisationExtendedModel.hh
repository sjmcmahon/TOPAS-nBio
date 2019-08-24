//
// ********************************************************************
// *                                                                  *
// * This file is part of the TOPAS-nBio extensions to the            *
// *   TOPAS Simulation Toolkit.                                      *
// * The TOPAS-nBio extensions are freely available under the license *
// *   agreement set forth at: https://topas-nbio.readthedocs.io/     *
// *                                                                  *
// ********************************************************************
//
// $Id: TsDNARuddIonisationExtendedModel.hh 96060 2016-03-11 12:58:04Z gcosmo $

#ifndef TsDNARuddIonisationExtendedModel_h
#define TsDNARuddIonisationExtendedModel_h 1

#include "G4VEmModel.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4ProductionCutsTable.hh"

#include "G4DNAGenericIonsManager.hh"
#include "G4DNACrossSectionDataSet.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4LogLogInterpolation.hh"

#include "G4DNAWaterIonisationStructure.hh"
#include "G4VAtomDeexcitation.hh"
#include "G4NistManager.hh"

class TsDNARuddIonisationExtendedModel : public G4VEmModel
{

public:

  TsDNARuddIonisationExtendedModel(const G4ParticleDefinition* p = 0, 
		           const G4String& nam = "DNARuddIonisationExtendedModel");

  virtual ~TsDNARuddIonisationExtendedModel();

  virtual void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  virtual G4double CrossSectionPerVolume(  const G4Material* material,
					   const G4ParticleDefinition* p,
					   G4double ekin,
					   G4double emin,
					   G4double emax);

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin,
				 G4double maxEnergy);

  inline void SelectStationary(G4bool input); 

protected:

  G4ParticleChangeForGamma* fParticleChangeForGamma;

private:

  G4bool statCode;

  // Water density table
  const std::vector<G4double>* fpWaterDensity;

  //deexcitation manager to produce fluo photns and e-
  G4VAtomDeexcitation*      fAtomDeexcitation;

  std::map<G4String,G4double,std::less<G4String> > lowEnergyLimit;
  std::map<G4String,G4double,std::less<G4String> > highEnergyLimit;
  
  // ZF: 26-10-2010
  std::map<G4double,G4double> lowEnergyLimitForA, lowEnergyLimitOfModelForA, killBelowEnergyForA;

  G4bool isInitialised;
  G4int verboseLevel;
  
  // Cross section

  typedef std::map<G4String,G4String,std::less<G4String> > MapFile;
  MapFile tableFile;

  typedef std::map<G4String,G4DNACrossSectionDataSet*,std::less<G4String> > MapData;
  MapData tableData;
  
  // Final state
  
  G4DNAWaterIonisationStructure waterStructure;

  G4double RandomizeEjectedElectronEnergy(G4ParticleDefinition* particleDefinition, 
					  G4double incomingParticleEnergy, 
					  G4int shell);

  // SI: Not necessary anymore since we now use interface to angle generator
  /*
  void RandomizeEjectedElectronDirection(G4ParticleDefinition* particleDefinition, 
					 G4double incomingParticleEnergy, 
					 G4double outgoingParticleEnergy, 
					 G4double & cosTheta, 
					 G4double & phi, G4int shell);
  */

  G4double RejectionFunction(G4ParticleDefinition* particle, 
							      G4double k, 
							      G4double proposed_ws, 
							      G4int ionizationLevelIndex);

  G4double ProposedSampledEnergy(G4ParticleDefinition* particle, 
							      G4double k,  
							      G4int ionizationLevelIndex);
   
  G4double CorrectionFactor(G4ParticleDefinition* particleDefinition, G4double k, G4int shell);

  G4double S_1s(G4double t, 
		G4double energyTransferred, 
		G4double slaterEffectiveChg, 
		G4double shellNumber);

  G4double S_2s(G4double t, 
		G4double energyTransferred, 
		G4double slaterEffectiveChg, 
		G4double shellNumber);


  G4double S_2p(G4double t, 
		G4double energyTransferred, 
		G4double slaterEffectiveChg, 
		G4double shellNumber);

  G4double R(G4double t, 
	     G4double energyTransferred, 
	     G4double slaterEffectiveChg, 
	     G4double shellNumber) ;

  G4double slaterEffectiveCharge[3];
  G4double sCoefficient[3];
  
  // Partial cross section
  
  G4double PartialCrossSection(const G4Track& track);
			
  G4double Sum(G4double energy, const G4String& particle);

  G4int RandomSelect(G4double energy,const G4String& particle );
   
  //
   
  TsDNARuddIonisationExtendedModel & operator=(const  TsDNARuddIonisationExtendedModel &right);
  TsDNARuddIonisationExtendedModel(const  TsDNARuddIonisationExtendedModel&);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void TsDNARuddIonisationExtendedModel::SelectStationary (G4bool input)
{ 
    statCode = input; 
}		 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif