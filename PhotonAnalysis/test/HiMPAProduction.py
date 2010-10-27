#
# \version $Id: HiMPAProduction.py,v 1.1 2010/09/30 14:21:44 yjlee Exp $
#

import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

## Source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
#    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_9_0_pre1/RelValPyquen_GammaJet_pt20_2760GeV/GEN-SIM-RECO/MC_38Y_V8-v1/0010/B6921A75-FA9B-DF11-92E8-001A92971B80.root'
#    'file:B6921A75-FA9B-DF11-92E8-001A92971B80.root'
    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_9_0_pre4/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_38Y_V11-v2/0027/0C4A6D7C-C5C3-DF11-90CE-0026189438ED.root'
    )
)

################# ESSource for the centrality #####
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = "sqlite_file:/afs/cern.ch/cms/slc5_ia32_gcc434/cms/cmssw/CMSSW_3_9_0_pre4/src/RecoHI/HiCentralityAlgos/data/CentralityTables.db"
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
                                      process.CondDBCommon,
                                      toGet = cms.VPSet(cms.PSet(record = cms.string('HeavyIonRcd'),
                                                                 tag = cms.string('HFhits40_MC_Hydjet2760GeV_MC_3XY_V24_v0')
                                                                 )
                                                        )
                                      )
######################################################


## # EGamma sequence
process.load("RecoHI.HiEgammaAlgos.HiEgamma_cff")

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_38Y_V8::All')
# process.GlobalTag.globaltag = cms.string('GR10_P_V4::All')

process.load("Configuration.StandardSequences.MagneticField_cff")

## Standard PAT Configuration File
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.patHeavyIonSequences_cff")
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

process.patPhotons.addGenMatch = cms.bool(False)
process.patPhotons.embedGenMatch= cms.bool(False)

#from PhysicsTools.PatAlgos.tools.coreTools import *
# turn off MC matching for the process -- ONLY needed if running over data -- for MC comment out
#removeMCMatching(process, ['All'])

# reduce verbosity
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

# configure HLT
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('40 OR 41')


# Modification for HI
process.load("CmsHi.PhotonAnalysis.MultiPhotonAnalyzer_cfi")
process.multiPhotonAnalyzer.GenParticleProducer = cms.InputTag("hiGenParticles")
process.multiPhotonAnalyzer.PhotonProducer = cms.InputTag("selectedPatPhotons")
process.multiPhotonAnalyzer.VertexProducer = cms.InputTag("hiPixelAdaptiveVertex")
process.multiPhotonAnalyzer.doStoreMET = cms.untracked.bool(False)
process.multiPhotonAnalyzer.doStoreJets = cms.untracked.bool(False)
process.multiPhotonAnalyzer.OutputFile = cms.string('ntuple-HiMPA.root')
process.multiPhotonAnalyzer.isMC_      = cms.untracked.bool(False)
process.singlePhotonAnalyzer.isMC_      = cms.untracked.bool(False)

# Comp Cone Analysis
process.multiPhotonAnalyzer.doStoreCompCone = cms.untracked.bool(True)


# Change the track collection
process.photons.isolationSumsCalculatorSet.trackProducer = process.multiPhotonAnalyzer.TrackProducer



#process.photonMatch.matched = cms.InputTag("hiGenParticles")
process.selectDigi.barrelSuperClusterProducer = cms.InputTag("correctedIslandBarrelSuperClusters")
process.selectDigi.endcapSuperClusterProducer = cms.InputTag("correctedIslandEndcapSuperClusters")

# timing
process.Timing = cms.Service("Timing")                                         

# random Cone
process.load("RandomConeAna.RandomPhotonProducer.randomConeSequence_cff")
process.compleSuperCluster.etCut            = process.multiPhotonAnalyzer.GammaPtMin
process.compleSuperCluster.etaCut           = process.multiPhotonAnalyzer.GammaEtaMax
process.compleSuperCluster.hoeCut           = cms.untracked.double(0.5)
process.complePhoton.isolationSumsCalculatorSet.trackProducer = process.multiPhotonAnalyzer.TrackProducer
#for HI setting
process.complePhoton.primaryVertexProducer = process.multiPhotonAnalyzer.VertexProducer


########### End process #################
process.load('Configuration.StandardSequences.EndOfProcess_cff')


# let it run
process.p = cms.Path(
  #  process.photonCore*process.photons*process.photonIDSequence*
    #    process.patHeavyIonDefaultSequence 
    #    process.heavyIon *
    process.makeHeavyIonPhotons *
    process.selectedPatPhotons *
    process.multiPhotonAnalyzer *
    process.endOfProcess
    )

