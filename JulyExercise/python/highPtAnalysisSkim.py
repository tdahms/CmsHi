import FWCore.ParameterSet.Config as cms

process = cms.Process('PAT')

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("reco.root")
    )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
    )

process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

from PhysicsTools.PatAlgos.tools.jetTools import *

process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")

process.output = cms.OutputModule("PoolOutputModule",
    process.jetTrkSkimContent,
    fileName = cms.untracked.string("jetAnaSkim.root")
    )

process.output.outputCommands.extend( "keep recoPhotons_*_*_*" )

process.prod = cms.Path(process.allTracks +
    #process.heavyIon +
    process.makeHeavyIonJets
    )

from CmsHi.JulyExercise.DisableMC_cff import *
disableMC(process)

process.out_step = cms.EndPath(process.output)

