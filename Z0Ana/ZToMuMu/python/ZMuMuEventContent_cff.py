import FWCore.ParameterSet.Config as cms

ZMuMuContent = cms.PSet(
    outputCommands = cms.untracked.vstring(
    #'keep *',    
    'keep *_generator_*_*',
    'keep *_genParticles_*_*',
    'keep *_hiCentrality_*_*',
    'keep *_hiEvtPlane_*_*',
    'keep patMuons_*_*_*',
    'keep recoMuons_muons_*_*',
    'keep *_dimuonsGlobal_*_*',
    'keep *_dimuonsGlobalSTA_*_*',
    'keep *_dimuonsSTA_*_*',
    'keep *_GenDimuons_*_*',
    'keep *_Dimuons_*_*',
    'keep *_ditracksSiliconTrk_*_*',
    'keep *_dimuonsTrk_*_*'
    )
    )




