import FWCore.ParameterSet.Config as cms

from GeneratorInterface.Pythia6Interface.pythiaDefault_cff import *

signal = cms.EDProducer("Pythia6PtYDistGun",
                    pythiaHepMCVerbosity = cms.untracked.bool(False),
                    pythiaPylistVerbosity = cms.untracked.int32(0),
                    
                        pythiaFrame = cms.string("cms"),
                        comEnergy = cms.double(4000),
                        
                    ParticleID = cms.untracked.int32(443),
                    DoubleParticle = cms.untracked.bool(False),
                    kinematicsFile = cms.untracked.string('HeavyIonsAnalysis/Configuration/data/jpsipbpb.root'),
                    
                    ptBinning = cms.untracked.int32(100000),
                    Ptmin = cms.untracked.double(0.0),
                    Ptmax = cms.untracked.double(100.0),
                    yBinning = cms.untracked.int32(500),
                    ymin = cms.untracked.double(-10.0),
                    ymax = cms.untracked.double(10.0),
                    
                    PGunParameters = cms.PSet(pythiaDefaultBlock,
                                                parameterSets = cms.vstring('pythiaDefault','jpsiDecay'),
                                                jpsiDecay = cms.vstring('BRAT(858) = 0 ! switch off',
                                                                        'BRAT(859) = 1 ! switch on',
                                                                        'BRAT(860) = 0 ! switch off',
                                                                        'MDME(858,1) = 0 ! switch off',
                                                                        'MDME(859,1) = 1 ! switch on',
                                                                        'MDME(860,1) = 0 ! switch off'),
                                                upsilonDecay = cms.vstring('BRAT(1034) = 0 ! switch off',
                                                                           'BRAT(1035) = 1 ! switch on',
                                                                           'BRAT(1036) = 0 ! switch off',
                                                                           'BRAT(1037) = 0 ! switch off',
                                                                           'BRAT(1038) = 0 ! switch off',
                                                                           'BRAT(1039) = 0 ! switch off',
                                                                           'BRAT(1040) = 0 ! switch off',
                                                                           'BRAT(1041) = 0 ! switch off',
                                                                           'BRAT(1042) = 0 ! switch off',
                                                                           'MDME(1034,1) = 0 ! switch off',
                                                                           'MDME(1035,1) = 1 ! switch on',
                                                                           'MDME(1036,1) = 0 ! switch off',
                                                                           'MDME(1037,1) = 0 ! switch off',
                                                                           'MDME(1038,1) = 0 ! switch off',
                                                                           'MDME(1039,1) = 0 ! switch off',
                                                                           'MDME(1040,1) = 0 ! switch off',
                                                                           'MDME(1041,1) = 0 ! switch off',
                                                                           'MDME(1042,1) = 0 ! switch off')
                                                )
                    )

# For upsilon generation, add in your configuration the uncommented lines :
#
#process.signal.kinematicsFile = cms.untracked.string('HeavyIonsAnalysis/Configuration/data/upsipbpb.root')
#process.signal.ParticleID = cms.untracked.int32(553)

