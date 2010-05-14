// -*- C++ -*-
//
// Package:    ZToMuMu
// Class:      ZToMuMu
// 
/**\class ZToMuMu ZToMuMu.cc Z0Ana/ZToMuMu/src/ZToMuMu.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Vineet Kumar,,,
//         Created:  Wed May 12 13:45:14 CEST 2010
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

class ZToMuMu : public edm::EDAnalyzer {
   public:
      explicit ZToMuMu(const edm::ParameterSet&);
      ~ZToMuMu();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
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
ZToMuMu::ZToMuMu(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


ZToMuMu::~ZToMuMu()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
ZToMuMu::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
ZToMuMu::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZToMuMu::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZToMuMu);
