#
# \version $Id: HiMPAProduction_compCone.py,v 1.2 2010/10/22 12:49:47 kimy Exp $
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
#    'rfio:/castor/cern.ch/cms/store/relval/CMSSW_3_9_0_pre4/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_38Y_V11-v2/0027/0C4A6D7C-C5C3-DF11-90CE-0026189438ED.root'
    'dcache:///pnfs/cmsaf.mit.edu/t2bat/cms/store/user/kimy/MinimumBiasHI/Spring10-JulyAnalysisExercise_MC_37Y_V4-HardEnriched-GEN-SIM-RECO/6a7752772a3d4259db7cafda4079c785/hiRecoJEX_RAW2DIGI_RECO_262_1_dAt.root')
)

## # EGamma sequence
process.load("RecoHI.HiEgammaAlgos.HiEgamma_cff")

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('MC_38Y_V8::All')
#process.GlobalTag.globaltag = cms.string('MC_37Y_V4::All')

################# ESSource for the centrality #####
process.GlobalTag.toGet = cms.VPSet(
            cms.PSet(record = cms.string("HeavyIonRcd"),
                                                   tag = cms.string("CentralityTable_HFhits40_Hydjet2760GeV_v0_mc"),
                                                   connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS")
                                                   )
                        )
######################################################     


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                   randomSuperCluster = cms.PSet(
    engineName = cms.untracked.string("TRandom3"),
    initialSeed = cms.untracked.uint32(982346)
    ))


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
process.multiPhotonAnalyzer.VertexProducer = cms.InputTag("hiSelectedVertex")
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
process.complePhoton.primaryVertexProducer  = process.multiPhotonAnalyzer.VertexProducer
process.randomPhotonAnalyzer.vertexProducer = process.multiPhotonAnalyzer.VertexProducer
process.randomPhotonAnalyzer.useHICentrality = cms.untracked.bool(True)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('randomConeNtuple.root'),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

########### End process #################
process.load('Configuration.StandardSequences.EndOfProcess_cff')


# let it run
process.p = cms.Path(
  #  process.photonCore*process.photons*process.photonIDSequence*
    #    process.patHeavyIonDefaultSequence 
    #    process.heavyIon *
    process.makeHeavyIonPhotons *
    process.selectedPatPhotons *
    process.complePhotonSequence *
    process.randomConeSqeunce *
    process.multiPhotonAnalyzer *
    process.endOfProcess
    )


