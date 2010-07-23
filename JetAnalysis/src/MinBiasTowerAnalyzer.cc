// -*- C++ -*-
//
// Package:    MinBiasTowerAnalyzer
// Class:      MinBiasTowerAnalyzer
// 
/**\class MinBiasTowerAnalyzer MinBiasTowerAnalyzer.cc yetkin/MinBiasTowerAnalyzer/src/MinBiasTowerAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Yetkin Yilmaz
//         Created:  Wed Oct  3 08:07:18 EDT 2007
// $Id: MinBiasTowerAnalyzer.cc,v 1.13 2010/07/22 15:50:46 nart Exp $
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/EvtPlane.h"

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/JetReco/interface/JetCollection.h"
#include "DataFormats/JetReco/interface/Jet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "HepMC/GenEvent.h"
#include "HepMC/HeavyIon.h"

#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TString.h>
#include <TRandom.h>

using namespace std;
using namespace edm;
using namespace reco;

static const int nEtaBins = 4;
static const int nPhiBins = 4;
static const int nCentBins = 11;


//
// class decleration
//

static double rhoBins[11] = {-5,-4,-3,-2,-1,0,1,2,3,4,5};

double getRho(double eta, const vector<double> rhos){
   int j = 0;
   for(unsigned int i = 0; i < rhos.size()-1 ; ++i){
     if(eta > rhoBins[i]) j = i+1;
   }
   double r = rhos[j-1]*fabs(eta - rhoBins[j]) + rhos[j]*fabs(eta - rhoBins[j-1]);
   r /= fabs(rhoBins[j]-rhoBins[j-1]);
   
   return r;
}


class MinBiasTowerAnalyzer : public edm::EDAnalyzer {
public:
  explicit MinBiasTowerAnalyzer(const edm::ParameterSet&);
  ~MinBiasTowerAnalyzer();
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void loadEvent(const edm::Event&, const edm::EventSetup&);
  void sumET();
  void analyzeMC();
  int analyzeGenParticles();
  void analyzeTowers();
  void analyzeRandomCones();
  double       getEt(const DetId &id, double energy);
  double       getEta(const DetId &id);
  double       getPhi(const DetId &id);
  
  // ----------member data ---------------------------
  
  
  int iEtamax_;
  double SumEtMin_;
  double SumEtMax_;
  double jetEtMin_;
  double sumET_;
  double Et_Min_;
  double Et_Max_;
  vector<double> missingTowersMean_;
  vector<double> missingTowersRMS_;
  
  double nBins_;
  double cone_;
  double genJetPtMin_;
  double towersize_;
  double jetEtaMax_;
  double genpartEtamax_;
  double b_;
  double npart_;
  double ncoll_;
  double nhard_;
  double phi0MC_;
  double phi0_;
  
  bool doMC_;
  bool doGenParticles_;
  bool excludeJets_;
  bool doRandomCone_;
  bool doTowers_;
  bool isSignal_;
  
  InputTag ktSrc_;
  InputTag akSrc_;
  InputTag evtPlaneSrc_;
  
  InputTag PatJetSrc_;
  InputTag HiSrc_;
  InputTag HiCentSrc_;
  InputTag TowersSrc_;
  InputTag FakeJetSrc_;
  InputTag GenJetSrc_;
  InputTag DirectSrc_;
  
  const CaloGeometry *geo;
  edm::Service<TFileService> fs;
  
  TNtuple* nt;
  TRandom * rand;
  
  edm::Handle<CaloTowerCollection> towers;
  edm::Handle<pat::JetCollection> jets;
  edm::Handle<reco::CaloJetCollection> calojets;
  edm::Handle<reco::CaloJetCollection> fakejets;
  edm::Handle<std::vector<bool> > directions;
  edm::Handle<reco::GenJetCollection> genjets;
  edm::Handle<reco::GenParticleCollection> genparticles;
  edm::Handle<vector<double> > ktRhos;
  edm::Handle<vector<double> > akRhos;
  edm::Handle<edm::GenHIEvent> mc;
  edm::Handle<reco::Centrality> centrality;
  
  edm::Handle<std::vector<reco::EvtPlane> > evtPlanes;
  const CentralityBins* cbins_;
  
  vector<TH1D*> hNtowers;
  vector< TH1D*> hNtowers2;
  vector<TH1D*> hGenpt;
  vector<TH1D*> hhGeneta;
  vector<TH1D*> hGenet;
  vector<TH1D*> hETeta;
  vector<TH1D*> hETphi;
  vector<TH2D*> hMeanETdNdEta;
  vector<TH1D*> hTowerEta;
  vector<TH1D*> hTowerPT;
  vector<TH1D*> hhSumET;
  vector<TH1D*> hTowerETjet;
  vector<TH1D*> hTowerETnojet;
  vector<TH1D*> hTowerET;
  vector< TH1D*> hPtGen;
  vector<TH1D*> hDndeta;
  vector<TH1D*> hRMS;
  vector<TH1D*> hMean;
  vector<TH1D*> hMeanNoJets;
  vector<TH1D*> hMeanRMS;
  vector<TH1D*> hMeanRMSnoJets;
  vector<TH1D*> hRMSnoJets;
  vector<TH1D*> hJetET;
  vector<TH2D*> hTowerJet;  
  vector<TF1*> fNtowers;
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MinBiasTowerAnalyzer::MinBiasTowerAnalyzer(const edm::ParameterSet& iConfig) : 
  sumET_(0),
  phi0_(0),
  phi0MC_(0),
  cbins_(0),
  geo(0)
{
   //now do what ever initialization is needed
	TH1D::SetDefaultSumw2();
	SumEtMin_ = iConfig.getUntrackedParameter<double>("sumEtMin",500);
	SumEtMax_ = iConfig.getUntrackedParameter<double>("sumEtMax",750);

	jetEtMin_ = iConfig.getUntrackedParameter<double>("jetEtMin",0);
	missingTowersMean_ = iConfig.getUntrackedParameter<vector<double> >("jetTowersMean",vector<double>(0));
        missingTowersRMS_ = iConfig.getUntrackedParameter<vector<double> >("jetTowersRMS",vector<double>(0));
	cone_ = iConfig.getUntrackedParameter<double>("coneSize",0.5);
        nBins_ = iConfig.getUntrackedParameter<double>("nBins",10);

	genJetPtMin_ =  iConfig.getUntrackedParameter<double>("genJetPtMin",20);
	towersize_ = iConfig.getUntrackedParameter<int>("nTowers",1584);
	jetEtaMax_ = iConfig.getUntrackedParameter<double>("jetEtaMax",1);
	genpartEtamax_ =iConfig.getUntrackedParameter<double>("genPartEtaMax",1);
	iEtamax_ = iConfig.getUntrackedParameter<double>("iEtaMax",11.5);
	
        doRandomCone_ = iConfig.getUntrackedParameter<bool>("doRandomCone",true);
        doTowers_ = iConfig.getUntrackedParameter<bool>("doTowers",true);
	doMC_ = iConfig.getUntrackedParameter<bool>("doMC",false);
	isSignal_ = iConfig.getUntrackedParameter<bool>("isSignal",false);
	doGenParticles_  = iConfig.getUntrackedParameter<bool>("doGenParticles",false);
        excludeJets_ = iConfig.getUntrackedParameter<bool>("excludeJets",false);
	
	PatJetSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("patJetSrc",edm::InputTag("icPu5patJets"));
	HiSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("hiSrc",edm::InputTag("heavyIon"));
	HiCentSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("centralitySrc",edm::InputTag("hiCentrality"));
	TowersSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("towersSrc",edm::InputTag("towerMaker"));
	FakeJetSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("fakeJetSrc",edm::InputTag("bkg5Jets"));
	DirectSrc_ = edm::InputTag(FakeJetSrc_.label(),"directions");
	GenJetSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("genJetSrc",edm::InputTag("iterativeCone5HiGenJets"));
        ktSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("ktSrc",edm::InputTag("kt4CaloJets"));
        akSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("akSrc",edm::InputTag("ak5CaloJets"));
        evtPlaneSrc_ = iConfig.getUntrackedParameter<edm::InputTag>("evtPlaneSrc",edm::InputTag("hiEvtPlane","recoLevel"));

}


MinBiasTowerAnalyzer::~MinBiasTowerAnalyzer()
{
  
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------


void
MinBiasTowerAnalyzer::analyze(const edm::Event& ev, const edm::EventSetup& iSetup)
{
  loadEvent(ev, iSetup);
  sumET();
  if(doMC_){
    analyzeMC();
  
    if(doGenParticles_) analyzeGenParticles();
  }

  if(doTowers_) analyzeTowers();
  if(doRandomCone_) analyzeRandomCones();

}

void MinBiasTowerAnalyzer::loadEvent(const edm::Event& ev, const edm::EventSetup& iSetup){
  
  using namespace edm;
  if(!geo){
    edm::ESHandle<CaloGeometry> pGeo;
    iSetup.get<CaloGeometryRecord>().get(pGeo);
    geo = pGeo.product();
  }
  if(!cbins_) cbins_ = getCentralityBinsFromDB(iSetup);
  
  if(cbins_->getNbins() != (int)(missingTowersMean_.size())) edm::LogError("BadConfig")<<"Number of bins is inconsistent in centrality table "<<cbins_->getNbins()<<" and the number of towers table!"<<missingTowersMean_.size();
  
   ev.getByLabel(HiSrc_,mc);
   ev.getByLabel(HiCentSrc_,centrality);
   ev.getByLabel(TowersSrc_,towers);
   ev.getByLabel(PatJetSrc_,jets);
   ev.getByLabel(DirectSrc_,directions);
   ev.getByLabel(FakeJetSrc_,fakejets);
   ev.getByLabel(GenJetSrc_,genjets);
   ev.getByLabel(edm::InputTag(ktSrc_.label(),"rhos"),ktRhos);
   ev.getByLabel(edm::InputTag(akSrc_.label(),"rhos"),akRhos);
   ev.getByLabel(edm::InputTag(evtPlaneSrc_),evtPlanes);
   
   //   cout<<"Using event plane determined by : "<<(*evtPlanes)[0].label()<<endl;
   phi0_ = (*evtPlanes)[0].angle();
}

void MinBiasTowerAnalyzer::sumET(){
   sumET_ = centrality->EtMidRapiditySum();
   double hf = centrality->EtHFhitSum();
   int bin = cbins_->getBin(hf); 
   if(isSignal_) bin = 0;

   if(sumET_ == 0){
     for(unsigned int i = 0 ; i < towers->size(); ++i){
       const CaloTower& tower = (*towers)[i];
       if(abs(tower.ieta())>iEtamax_) continue;
       double tower_et = tower.et();
       sumET_=sumET_+tower_et;
     }
   }
   hhSumET[bin]->Fill(sumET_);
}

void MinBiasTowerAnalyzer::analyzeMC(){
  
  b_ = mc->b();
  npart_ = mc->Npart();
  ncoll_ = mc->Ncoll();
  nhard_ = mc->Nhard();
  phi0MC_ = mc->evtPlane(); 
}

void MinBiasTowerAnalyzer::analyzeTowers(){
  
  
  bool interest = sumET_>SumEtMin_ && sumET_<SumEtMax_;
  double hf = centrality->EtHFhitSum();
  int bin = cbins_->getBin(hf); 
  if(isSignal_)  bin = 0;

  double sumofTowerpt = 0;
  double sumofTowerNojetpt = 0;
  double T_ptsquare = 0;
  double T_nojet_ptsquare = 0;
  double rmsofTowerpt =0;
  double rmsofNojetTowerpt = 0;
  double numberofTower = 0;
  double numberofNojetTower = 0;
  double average_ptrms = 0;
  double average_Nojetptrms = 0;
  double Zeroptcount = 0;
  double average_pt=0;
  double average_nojetpt=0;
  int genparticlecount = 0;
  
  if(doMC_ && doGenParticles_) genparticlecount = analyzeGenParticles();
  
    // Run analysis on towers
    
    for(unsigned int i = 0 ; i < towers->size(); ++i){
      const CaloTower& tower = (*towers)[i];
      if(abs(tower.ieta())>iEtamax_) continue;
      
      numberofTower++;
      
      double eta = tower.eta();
      double ieta = tower.ieta();
      double pt = tower.pt();
      double et = tower.et();
      double phi = tower.phi();      
      double phiRel = reco::deltaPhi(phi,phi0_);
      bool recomatched = false;
      double rawJetEt = 0;
      sumofTowerpt=sumofTowerpt+pt;
      T_ptsquare=T_ptsquare+pt*pt;
      hTowerPT[bin]->Fill(pt);
      if(pt==0) Zeroptcount++;
      hTowerEta[bin]->Fill(ieta);
      hTowerET[bin]->Fill(et);
      hETeta[bin]->Fill(eta,pt);
      hETphi[bin]->Fill(phiRel,et);
      
      
      for(unsigned int j = 0 ; j < jets->size(); ++j){
	//         const reco::CaloJet& jet = (*jets)[j];
	const pat::Jet& jet = (*jets)[j];
	rawJetEt = 0;
        rawJetEt = jet.correctedJet("raw").et();
	hJetET[bin]->Fill(rawJetEt);
	if(rawJetEt < jetEtMin_ ) continue;
	double dr = reco::deltaR(tower.eta(),tower.phi(),jet.eta(),jet.phi());
	if(dr < cone_){
	  recomatched = true;
	}
      }
      if(recomatched){
	hTowerETjet[bin]->Fill(pt);
	hTowerJet[bin]->Fill(pt,rawJetEt);     
 }
      else {
	hTowerETnojet[bin]->Fill(pt);
	sumofTowerNojetpt=sumofTowerNojetpt+pt;
	numberofNojetTower++;
	T_nojet_ptsquare=T_nojet_ptsquare+pt*pt;
      }
      
    }
    
    for(unsigned int k=0; k< (towersize_- numberofTower) ; k++)
      {	
	hTowerPT[bin]->Fill(0); 
	cout<<"missed tower in Calotower collection"<<endl;
      }
    
    	average_pt=sumofTowerpt/(numberofTower);
	hMean[bin]->Fill(average_pt);
	average_nojetpt=sumofTowerNojetpt/numberofNojetTower;
	hMeanNoJets[bin]->Fill(average_nojetpt);	
	rmsofNojetTowerpt=sqrt( (T_nojet_ptsquare/numberofNojetTower) - (average_nojetpt*average_nojetpt));
	hRMSnoJets[bin]->Fill(rmsofNojetTowerpt);
	rmsofTowerpt=sqrt( ( T_ptsquare/numberofTower ) - (average_pt*average_pt) );
	hRMS[bin]->Fill(rmsofTowerpt);
	average_ptrms=average_pt+rmsofTowerpt;
	hMeanRMS[bin]->Fill(average_ptrms);	
	average_Nojetptrms=average_nojetpt+rmsofNojetTowerpt;
	hMeanRMSnoJets[bin]->Fill(average_Nojetptrms);
	hMeanETdNdEta[bin]->Fill(average_ptrms,genparticlecount);
	
 
}

/// LOOP OVER gen particles

int MinBiasTowerAnalyzer::analyzeGenParticles(){
  
  bool interest = sumET_>SumEtMin_ && sumET_<SumEtMax_;
  int genparticlecount = 0;
  double hf = centrality->EtHFhitSum();
  int bin = cbins_->getBin(hf); 
  if(isSignal_)  bin = 0;

  for(unsigned int i = 0 ; i < genparticles->size(); ++i){
    const reco::GenParticle& genparticle = (*genparticles)[i];
    if(fabs(genparticle.eta())>genpartEtamax_) continue;
    if(genparticle.charge() != 0) continue;
    if(genparticle.status() == 1) {
      double pt = genparticle.pt();
      hPtGen[bin]->Fill(pt);
      genparticlecount++;
      }
  } 
 
  hDndeta[bin]->Fill(genparticlecount); 
  return genparticlecount;
}

void MinBiasTowerAnalyzer::analyzeRandomCones(){
  
  bool interest = sumET_>SumEtMin_ && sumET_<SumEtMax_;
  int njet = jets->size();
  int njet20 = 0;
  int njet30 = 0;
  
  double hf = centrality->EtHFhitSum();
  int bin = cbins_->getBin(hf); 
  if(isSignal_) int bin = 0;

  for(unsigned int i = 0 ; i < fakejets->size(); ++i){
    const reco::CaloJet& fakejet = (*fakejets)[i];
    vector<edm::Ptr<CaloTower> > constits = fakejet.getCaloConstituents();
    double aveta = 0;
    double totpt = 0;
    double avphi = 0;
    int ncons = constits.size();
    vector<int> used;    
    double area = fakejet.towersArea();
    double sign = (int)((*directions)[i])*2 - 1;   
    double fpt = sign*fakejet.pt();    
    double fpu = fakejet.pileup();
    double eta = fakejet.eta();
    double phiRel = reco::deltaPhi(fakejet.phi(),phi0_);
    double phi =fakejet.phi();
    
    double ktRho = getRho(eta,*ktRhos);
    double akRho = getRho(eta,*akRhos);
    
    int nc = 0;
    
    int nTow = -1;
    while(nTow < 0 || nTow > ncons) nTow = (int)(fNtowers[bin]->GetRandom());
    for(int ic1 = 0; ic1 < nTow; ++ic1){
      int ic = -1; 
      
      while(find(used.begin(),used.end(), ic) != used.end() || ic < 0 || ic >= ncons){
	
	double r = rand->Rndm();
	
	ic = (int)(ncons*r);
      }
      used.push_back(ic);
      double toweta = constits[ic]->eta();
      double towphi = reco::deltaPhi(constits[ic]->phi(),phi0_);
      double towpt = constits[ic]->et();
      aveta += toweta*towpt;
      avphi += towphi*towpt;
      totpt += towpt;
      nc++;
      
    }
    aveta /= totpt;
    avphi /= totpt;
    
    float entry[21] = {eta,phi,phiRel,fpt+fpu,totpt,bin,fpt+fpu,fpu,fpt,sign,njet,njet20,njet30,ncons,area,nc,sumET_,centrality->EtHFhitSum(),ktRho,akRho};
    nt->Fill(entry);
    
    if(interest){
      float entry[21] = {eta,phi,phiRel,fpt+fpu,totpt,-1,fpt+fpu,fpu,fpt,sign,njet,njet20,njet30,ncons,area,nc,sumET_,centrality->EtHFhitSum(),ktRho,akRho};
      nt->Fill(entry);
    }
    
  }
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
MinBiasTowerAnalyzer::beginJob()
{
  nt = fs->make<TNtuple>("nt","","eta:phi:phiRel:pt1:pt2:bin:et:pu:subt:sign:njet:njet20:njet30:ncons:area:nc:sumet:hf:kt:ak");
  rand = new TRandom();
  
   TH1::SetDefaultSumw2();
   
   for(unsigned int i = 0; i < missingTowersMean_.size(); ++i){
     TF1* f = fs->make<TF1>(Form("fNtowers%d",i),"gaus(0)",0,80);
     
     f->SetParameter(0,1);
     f->SetParameter(1,missingTowersMean_[i]);
     f->SetParameter(2,missingTowersRMS_[i]);
     fNtowers.push_back(f);
   }
  
   for(unsigned int i = 0; i< nBins_; ++i){

     hNtowers.push_back(fs->make<TH1D>(Form("nTowers%d",i),"histogram;N_{towers};entries",480,-0.5,60.5));
     hNtowers2.push_back(fs->make<TH1D>(Form("nTowers2%d",i),"histogram;N_{towers}; entries",240,-0.5,60.5));          
     hGenpt.push_back(fs->make<TH1D>(Form("hGenpt%d",i),"histogram",100,0,100));
     hhGeneta.push_back(fs->make<TH1D>(Form("hhGeneta%d",i),"histogram",100,-10,10));
     hGenet.push_back(fs->make<TH1D>(Form("hGenet%d",i),"histogram",100,0,100));
     hETeta.push_back(fs->make<TH1D>(Form("etavsPT%d",i),"fhisto; eta ;pt(GeV/c) ",1000,-15,15));
     hETphi.push_back(fs->make<TH1D>(Form("phivsET%d",i),"fhisto; phi ;et(GeV) ",1000,-5,5));
     hMeanETdNdEta.push_back(fs->make<TH2D>(Form("hMeanETdNdEta%d",i),"mean_dndeta; mean tower p_{T} [GeV/c]; dNdeta",100,0,2,300,0,3000));
     hTowerEta.push_back(fs->make<TH1D>(Form("hTowerEta%d",i),"histogram; eta",1000,-15,15));
     hTowerPT.push_back(fs->make<TH1D>(Form("hTowerPT%d",i),"histogram; p_{T} [GeV/c]; entries",505,-1,50));
     hhSumET.push_back(fs->make<TH1D>(Form("hhSumET%d",i),"histogram;SumTower E_{T}[GeV]; entries",250,-1,2501));
     hTowerETjet.push_back(fs->make<TH1D>(Form("hTowerETjet%d",i),"histo; p_{T} [GeV/c]; entries",505,-1,50));
     hTowerJet.push_back(fs->make<TH2D>(Form("hTowerJet%d",i),"histo; Tower E_{T}; Jet p_{T}",505,-1,50,250,0,250));
     hTowerETnojet.push_back(fs->make<TH1D>(Form("hnojetPT%d",i),"histo; p_{T} [GeV/c]; entries",505,-1,50));
     hTowerET.push_back(fs->make<TH1D>(Form("htowerET%d",i),"histogram; E_{T} [GeV]; entries",505,-1,50));
     hPtGen.push_back(fs->make<TH1D>(Form("hPtGen%d",i),"histogram; p_{T} [GeV/c]; entries",100,0,100));
     hDndeta.push_back(fs->make<TH1D>(Form("hDndeta%d",i),"histogram; dNdeta; entries",300,0,3000));
     hRMS.push_back(fs->make<TH1D> (Form("htowerRMS%d",i),"histo",100,0,2.5));
     hMean.push_back(fs->make<TH1D>(Form("hPTaverage%d",i),"histo;p_{T} GeV/c; entries",100, 0,2.5));
     hMeanNoJets.push_back(fs->make<TH1D>(Form("hnojetmeanPT%d",i),"histo;p_{T} GeV/c",100, 0,2.5));
     hMeanRMS.push_back(fs->make<TH1D>(Form("htowerMeanRMS%d",i),"histo; Tower mean+rms p_{T} [GeV/c]; entries",100,0,2.5));
     hMeanRMSnoJets.push_back(fs->make<TH1D>(Form("hMeanRMSnoJets%d",i),"histo; No-Jet Tower mean+rms p_{T} [GeV/c]; entries",100,0,2.5));
     hRMSnoJets.push_back(fs->make<TH1D>(Form("hRMSnoJets%d",i),"histo; hRMSnoJets",100,0,2.5));
     hJetET.push_back(fs->make<TH1D>(Form("hJetET%d",i),"histogram;Jet E_{T} [GeV]; entries",150,0,150));
   }
   
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MinBiasTowerAnalyzer::endJob() {

  cout<<"--- The End --- "<<endl;
}

double MinBiasTowerAnalyzer::getEt(const DetId &id, double energy){
   const GlobalPoint& pos=geo->getPosition(id);
   double et = energy*sin(pos.theta());
   return et;
}
 
double MinBiasTowerAnalyzer::getEta(const DetId &id){
   const GlobalPoint& pos=geo->getPosition(id);
   double et = pos.eta();
   return et;
}

double MinBiasTowerAnalyzer::getPhi(const DetId &id){
   const GlobalPoint& pos=geo->getPosition(id);
   double et = pos.phi();
   return et;
}

//define this as a plug-in
DEFINE_FWK_MODULE(MinBiasTowerAnalyzer);
