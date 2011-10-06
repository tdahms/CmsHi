#ifndef CmsHi_PhotonAnalysis_SinglePhotonAnalyzerTree_h
#define CmsHi_PhotonAnalysis_SinglePhotonAnalyzerTree_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include <TLorentzVector.h>
#include "UserCode/HafHistogram/interface/HColumn.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"
#include "TTree.h"
#include "CmsHi/PhotonAnalysis/plugins/Limits.h"
#if MPA_VERSION < 2
#include "RecoEgamma/EgammaTools/interface/ConversionLikelihoodCalculator.h"
#endif

const int kMaxPhotons = 50;

class HTupleManager;
class HHistogram;
class HTuple;
class EcalClusterLazyTools;
class ConversionLikelihoodCalculator;
class PhotonMCTruthFinder;

// comparison operator needed by HTValVector

bool operator < (const TLorentzVector & a, const TLorentzVector & b);
bool operator < (const TVector3 & a,       const TVector3 & b      );

class SinglePhotonAnalyzerTree : public edm::EDAnalyzer { 
  
public:
  
  explicit SinglePhotonAnalyzerTree(const edm::ParameterSet&);
  ~SinglePhotonAnalyzerTree();

  math::XYZPoint vtx_;

#if MPA_VERSION < 2
  ConversionLikelihoodCalculator* theLikelihoodCalc_;
#endif


protected:
	
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
	
  virtual void storeGeneral(const edm::Event&, const edm::EventSetup& );
  virtual void storeL1Trigger(const edm::Event&);
  virtual void storeHLT(const edm::Event&);
  virtual void storeHF(const edm::Event&);
  virtual bool analyzeMC(const edm::Event&,  const edm::EventSetup& );
  virtual void storeVertex(const edm::Event&);
  virtual bool storeMET(const edm::Event&);
  virtual int  storeJets(const edm::Event&);
  virtual bool selectStorePhoton(const edm::Event&,const edm::EventSetup&);
  virtual void storeEvtPlane(const edm::Event&);
  virtual bool storeTracks(const edm::Event&);
  
	
  virtual void storePhotonAOD(pat::Photon *photon, const edm::Event& e, const edm::EventSetup &es, HTuple *tpl, const char* prefx = "PHO1_");
  virtual bool storeMCMatch( const edm::Event& e,pat::Photon *photon, const char* prefx = "PHO1_");	
  virtual Int_t getNumOfPreshClusters(pat::Photon *photon, const edm::Event&);
  virtual Float_t getESRatio(pat::Photon *photon, const edm::Event&, const edm::EventSetup&);


  // obtain generator-level calorimeter isolation and track isolation 
  // distribution, return number of particles and sumet surrounding the candidate
  
  virtual Float_t getGenCalIso(edm::Handle<reco::GenParticleCollection> handle,
			       reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);
  virtual Float_t getGenTrkIso(edm::Handle<reco::GenParticleCollection> handle,
			       reco::GenParticleCollection::const_iterator thisPho, const Float_t dRMax=0.4);

  // collection of gen particles that are converted photons
  virtual void storeConvMCTruth(const edm::Event& e, 
				reco::GenParticleCollection::const_iterator thisPho, HTuple *tpl, const char* prefx = "");

  PhotonMCTruthFinder*                            thePhotonMCTruthFinder_;

	
	// Configured fields
  bool verbose_;                 // verbose flag
  bool fillMCNTuple_;            // fill generator ntuple flag
  bool doL1Objects_;             // store L1 Object flag
  bool isMCData_;                // run over MCData flag
  bool storePhysVectors_;        // store TLorentzVector/TVector3 objects instead of plain floats
  std::string outputFile_;       // name of output file
	
  edm::InputTag hlTriggerResults_;    // Input tag for TriggerResults
  edm::InputTag l1gtReadout_;         // Input tag for L1 Global Trigger Readout
  edm::InputTag l1IsolTag_;           // Input tag for L1 EM isolated collection
  edm::InputTag l1NonIsolTag_;        // Input tag for L1 EM non isolated collection
  std::vector<std::string> triggerPathsToStore_;  // Vector to store list of HLT paths to store results of in ntuple

  edm::InputTag genParticleProducer_; // MC particle producer
  edm::InputTag hepMCProducer_; // MC hepMC producer
  edm::InputTag genEventScale_; // MC gen Event Scale
  edm::InputTag photonProducer_;      // photon producer
  edm::InputTag trackProducer_;       // track producer 
  edm::InputTag jetProducer_;         // jet producer 
  edm::InputTag metProducer_;         // input tag for MET 
  edm::InputTag vertexProducer_;      // vertecies producer
  edm::InputTag beamSpotProducer_;    // beam spot producer
	
  edm::InputTag compPhotonProducer_;      // photon producer                                                                                                          
  
  edm::InputTag ebReducedRecHitCollection_;
  edm::InputTag eeReducedRecHitCollection_;
  edm::InputTag srcTowers_;           // CaloTowers
  
  edm::TriggerNames triggerNames_;    // TriggerNames class
	
  edm::InputTag evtPlaneLabel;
  edm::InputTag EleTag_;  // gsf electrons
  

  // HepMC switch for HI July Exercise.
  bool         isMC_; 
  
  // basiccluster inputtags for heavy ion
  edm::InputTag basicClusterBarrel_;
  edm::InputTag basicClusterEndcap_;
  edm::InputTag hbhe_;
  edm::InputTag hf_;
  edm::InputTag ho_;
     
  double       ptMin_;            // Photon pt threshold
  double       etaMax_;           // Maximum photon |eta|
  double       ecalBarrelMaxEta_; // Begining of ECAL Barrel/Endcap gap
  double       ecalEndcapMinEta_; // End of ECAL Barrel/Endcap gap
  double       ptJetMin_;         // Jet pt threshold
  double       ptTrackMin_;
  double       etaTrackMax_;
  int          pdgId_;            // PDG ID of expected MC particle
  std::vector<int> otherPdgIds_;  // PDG ID of other MC particles to match
  double       mcPtMin_;          // min MC particle pt
  double       mcEtaMax_;         // max MC particle eta

  double       etCutGenMatch_;    // cut for the genMatching.. 
  double       etaCutGenMatch_;   // cut for the genMatching..
  // TupleManager, Histograms, ntuples
  HTupleManager  *tplmgr; 
  
  // Leading photon  MC truth histograms
  HHistogram     *_ptHist;       // leading photon pt histo
  HHistogram     *_ptHatHist;       // leading photon pt histo                                                                    
  HHistogram     *_etaHist;      // leading photon eta histo
  HHistogram     *_vtxX;         // generated vertex X
  HHistogram     *_vtxY;         // generated vertex Y
  HHistogram     *_vtxZ;         // generated vertex Z
	
  // Reco histograms
  HHistogram     *_gammaPtHist;  // leading photon pt histo
  HHistogram     *_gammaEtaHist; // leading photon eta histo
  HHistogram     *_gammaPhiModHist; // leading photon phi_mod = phi * 180/pi mod 20 - 10
  HHistogram     *_metHist;      // MET histo
  HHistogram     *_nVtxHist;     // number of vertecies
  HHistogram     *_primVtxX;     // reconstructed primary vertex X
  HHistogram     *_primVtxY;     // reconstructed primary vertex Y
  HHistogram     *_primVtxZ;     // reconstructed primary vertex Z
  HHistogram     *_nJetsHist;    // number of jets
  HHistogram     *_nPhotonsHist;  //number of Photons

  // Flags for the fillers
  bool 	doStoreGeneral_; 	 // Store General information
  bool 	doStoreCentrality_; 	 // Store Centrality information
  bool 	doStoreHLT_;     	 // Store HLT Trigger	
  bool	doStoreL1Trigger_;	 // Store L1 Trigger
  bool 	doStoreHF_;		 // Store HF
  bool	doStoreVertex_;		 // Store Vertex
  bool 	doStoreMET_; 		 // Store MET
  bool 	doStoreJets_;		 // Store Jets
  bool  doStoreConversions_;
  bool  doStoreCompCone_;
  bool doStoreTracks_;
  HTuple         *_ntuple;        // Analysis ntuple
  HTuple         *_ntupleMC;      // MC truth ntuple

  // heavy ion stuffs

  const CentralityBins * cbins_;
  CentralityProvider *centrality_;

  TTree *theTree;


  
  int run;
  int event;
  int bunchCrossing;
  int luminosityBlock;

  int nPho;
  // float p[kMaxPhotons];
  float pt[kMaxPhotons];
  float et[kMaxPhotons]; // same to pt
  float energy[kMaxPhotons]; // same to scEnergy                                                                                        
  float rawEnergy[kMaxPhotons];
  float px[kMaxPhotons];
  float py[kMaxPhotons];
  float pz[kMaxPhotons];
  float eta[kMaxPhotons];
  float phi[kMaxPhotons];
  float r9[kMaxPhotons];
  float isEBGap[kMaxPhotons];
  float isEEGap[kMaxPhotons];
  float isEBEEGap[kMaxPhotons];
  float isTransGap[kMaxPhotons];


  float preshowerEnergy[kMaxPhotons];
  float numOfPreshClusters[kMaxPhotons];
  float ESRatio[kMaxPhotons];
  float clustersSize[kMaxPhotons];
  float scSize[kMaxPhotons];
  float phiWidth[kMaxPhotons];
  float etaWidth[kMaxPhotons];
  float scEta[kMaxPhotons];
  float scPhi[kMaxPhotons];

  float sigmaEtaEta[kMaxPhotons];
  float sigmaIetaIeta[kMaxPhotons];
  float sigmaIphiIphi[kMaxPhotons];
  float sieie50[kMaxPhotons];
  float sieie45[kMaxPhotons];
  float sieie42[kMaxPhotons];
  float sieie39[kMaxPhotons];

  float covPhiPhi[kMaxPhotons];
  float covEtaPhi[kMaxPhotons];
  float covEtaEta[kMaxPhotons];

  
  float r1x5[kMaxPhotons];
  float r2x5[kMaxPhotons];
  float e1x5[kMaxPhotons];
  float e2x5[kMaxPhotons];
  float eMax[kMaxPhotons];
  float e2nd[kMaxPhotons];
  float e2x2[kMaxPhotons];
  float e3x3[kMaxPhotons];
  float e3x2[kMaxPhotons];
  float e4x4[kMaxPhotons];
  float e5x5[kMaxPhotons];
  float e2overe8[kMaxPhotons];
  float eRight[kMaxPhotons];
  float eLeft[kMaxPhotons];
  float eTop[kMaxPhotons];
  float eBottom[kMaxPhotons];
  float e2x5Right[kMaxPhotons];
  float e2x5Left[kMaxPhotons];
  float e2x5Top[kMaxPhotons];
  float e2x5Bottom[kMaxPhotons];
  
  
  float seedTime[kMaxPhotons];
  float seedChi2[kMaxPhotons];
  float seedOutOfTimeChi2[kMaxPhotons];
  float seedRecoFlag[kMaxPhotons];
  float seedSeverity[kMaxPhotons];
  float tRight[kMaxPhotons];
  float tLeft[kMaxPhotons];
  float tTop[kMaxPhotons];
  float tBottom[kMaxPhotons];
  float swissCrx[kMaxPhotons];
  

  float hadronicOverEm[kMaxPhotons];
  float hadronicDepth1OverEm[kMaxPhotons];
  float hadronicDepth2OverEm[kMaxPhotons];

  float ecalRecHitSumEtConeDR04[kMaxPhotons];
  float hcalTowerSumEtConeDR04[kMaxPhotons];
  float hcalDepth1TowerSumEtConeDR04[kMaxPhotons];
  float hcalDepth2TowerSumEtConeDR04[kMaxPhotons];
  float trkSumPtHollowConeDR04[kMaxPhotons];

  float ecalRecHitSumEtConeDR03[kMaxPhotons];
  float hcalTowerSumEtConeDR03[kMaxPhotons];
  float hcalDepth1TowerSumEtConeDR03[kMaxPhotons];
  float hcalDepth2TowerSumEtConeDR03[kMaxPhotons];
  float trkSumPtHollowConeDR03[kMaxPhotons];

  float isEle[kMaxPhotons];
  float detaEle[kMaxPhotons];
  float dphiEle[kMaxPhotons];
  float eleCharge[kMaxPhotons];
  float eleEoverP[kMaxPhotons];


  float c1[kMaxPhotons];
  float c2[kMaxPhotons];
  float c3[kMaxPhotons];
  float c4[kMaxPhotons];
  float c5[kMaxPhotons];

  float r1[kMaxPhotons];
  float r2[kMaxPhotons];
  float r3[kMaxPhotons];
  float r4[kMaxPhotons];
  float r5[kMaxPhotons];

  float t1PtCut[kMaxPhotons];
  float t2PtCut[kMaxPhotons];
  float t3PtCut[kMaxPhotons];
  float t4PtCut[kMaxPhotons];
  float t5PtCut[kMaxPhotons];
  
  float cc1[kMaxPhotons];
  float cc2[kMaxPhotons];
  float cc3[kMaxPhotons];
  float cc4[kMaxPhotons];
  float cc5[kMaxPhotons];
  
  float cr1[kMaxPhotons];
  float cr2[kMaxPhotons];
  float cr3[kMaxPhotons];
  float cr4[kMaxPhotons];
  float cr5[kMaxPhotons];

  float ct1[kMaxPhotons];
  float ct2[kMaxPhotons];
  float ct3[kMaxPhotons];
  float ct4[kMaxPhotons];
  float ct5[kMaxPhotons];

  float ct1PtCut20[kMaxPhotons];
  float ct2PtCut20[kMaxPhotons];
  float ct3PtCut20[kMaxPhotons];
  float ct4PtCut20[kMaxPhotons];
  float ct5PtCut20[kMaxPhotons];

  float ct1j20[kMaxPhotons];
  float ct2j20[kMaxPhotons];
  float ct3j20[kMaxPhotons];
  float ct4j20[kMaxPhotons];
  float ct5j20[kMaxPhotons];

  float ct4j10[kMaxPhotons];
  float ct4j15[kMaxPhotons];
  float ct4j05[kMaxPhotons];

  float dr11[kMaxPhotons];
  float dr21[kMaxPhotons];
  float dr31[kMaxPhotons];
  float dr41[kMaxPhotons];
  float dr12[kMaxPhotons];
  float dr22[kMaxPhotons];
  float dr32[kMaxPhotons];
  float dr42[kMaxPhotons];
  float dr13[kMaxPhotons];
  float dr23[kMaxPhotons];
  float dr33[kMaxPhotons];
  float dr43[kMaxPhotons];
  float dr14[kMaxPhotons];
  float dr24[kMaxPhotons];
  float dr34[kMaxPhotons];
  float dr44[kMaxPhotons];

  float t11[kMaxPhotons];
  float t21[kMaxPhotons];
  float t31[kMaxPhotons];
  float t41[kMaxPhotons];
  float t12[kMaxPhotons];
  float t22[kMaxPhotons];
  float t32[kMaxPhotons];
  float t42[kMaxPhotons];
  float t13[kMaxPhotons];
  float t23[kMaxPhotons];
  float t33[kMaxPhotons];
  float t43[kMaxPhotons];
  float t14[kMaxPhotons];
  float t24[kMaxPhotons];
  float t34[kMaxPhotons];
  float t44[kMaxPhotons];
  
  int isGenMatched[kMaxPhotons];

  float genMatchedPt[kMaxPhotons];
  float genMatchedEta[kMaxPhotons];
  float genMatchedPhi[kMaxPhotons];
  float genMomId[kMaxPhotons];
  float genMatchedCollId[kMaxPhotons];

  float genGrandMomId[kMaxPhotons];
  float genNSiblings[kMaxPhotons];
  float genCalIsoDR03[kMaxPhotons];
  float genCalIsoDR04[kMaxPhotons];
  float genTrkIsoDR04[kMaxPhotons];
  
    
};




#endif
