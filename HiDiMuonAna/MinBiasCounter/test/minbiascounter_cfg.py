import FWCore.ParameterSet.Config as cms

process = cms.Process("MBCounter")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR10_P_V12::All'

from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
    centralityVariable = cms.string("HFhits"),
    nonDefaultGlauberModel = cms.string(""),
    centralitySrc = cms.InputTag("hiCentrality")
    )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/cms/store/hidata/HIRun2010/HIAllPhysics/RECO/PromptReco-v3/000/152/760/00AC8E90-91FD-DF11-86DC-0030487CAF0E.root'
        #'rfio:/castor/cern.ch/cms/store/hidata/HIRun2010/HIAllPhysics/RECO/PromptReco-v3/000/153/021/B2160434-0E00-E011-9EA6-001D09F25208.root'
        #'rfio:/castor/cern.ch/cms/store/hidata/HIRun2010/HIAllPhysics/RECO/PromptReco-v2/000/150/619/E8F34510-27ED-DF11-95F2-0030487CD7CA.root'
    )
)

process.mbcounter = cms.EDAnalyzer('MinBiasCounter',
                              triggerName = cms.string("HLT_HIMinBiasHfOrBSC"),
                              histFileName = cms.string("MinBiasCentrality_Histo.root")
                              #hltPreHIMinBiasBSCCore ||hltPreHIMinBiasHF_Core
)


process.p = cms.Path(process.mbcounter)