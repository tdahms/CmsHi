#/bin/bash -
# setup HLT
cvs co -r V13-00-01 HLTrigger/Configuration
#cvs co -r V03-00-07     L1TriggerConfig/L1GtConfigProducers
checkdeps -a
addpkg RecoTracker/MeasurementDet
cp -v /afs/cern.ch/user/g/gruen/public/MeasurementTrackerESProducer.cc RecoTracker/MeasurementDet/plugins/
addpkg DQM/HcalMonitorTasks
cp -v /afs/cern.ch/user/g/gruen/public/HcalDataIntegrityTask.cc DQM/HcalMonitorTasks/src/

# get hlt menu cff
hltGetConfiguration --cff --offline --data /dev/CMSSW_4_4_2/HIon --type HIon --unprescale > $CMSSW_BASE/src/HLTrigger/Configuration/python/HLT_HIon_data_cff.py
#hltGetConfiguration --cff --offline --data /users/frankma/devCMSSW_4_2_0/HIonV1047 --type HIon --unprescale > $CMSSW_BASE/src/HLTrigger/Configuration/python/HLT_HIon_Jet_data_cff.py

# get hlt menu cfg
#hltGetConfiguration --full --offline --data /users/frankma/devCMSSW_4_2_0/HLT1047HIonJetOnRepackedRaw --type HIon --unprescale --process HLTHIonJet --globaltag GR_R_44_V6::All --l1 L1GtTriggerMenu_L1Menu_CollisionsHeavyIons2011_v0_mc --input file:/net/hisrv0001/home/davidlw/scratch1/HLTStudies/CMSSW_4_4_1_L1Repack/src/l1EmulatorFromRawRepackRaw_RAW2DIGI_L1_DIGI2RAW.root > HLT_HIonJet.py
#hltGetConfiguration --full --offline --data /dev/CMSSW_4_2_0/HIon --type HIon --unprescale --process HLTHIon --globaltag START44_V6::All --l1-emulator gmt,gct,gt --l1 L1GtTriggerMenu_L1Menu_CollisionsHeavyIons2011_v0_mc --input file:/mnt/hadoop/cms/store/results/heavy-ions/HICorePhysics/StoreResults-HICorePhysics_Skim_MinimumBias_RAW-a606dc809a29a92e17749e5652319ad0-SD_MBHI/HICorePhysics/USER/StoreResults-HICorePhysics_Skim_MinimumBias_RAW-a606dc809a29a92e17749e5652319ad0-SD_MBHI/0000/0EDFD1BD-C9AE-E011-963E-003048CB82AC.root > HLT_HIon.py

# muon l1 fix
cvs co -d       CmsHi/HiHLTAlgos UserCode/CmsHi/HiHLTAlgos
addpkg L1Trigger/Configuration
cp -v CmsHi/HiHLTAlgos/modifiedFiles/L1Trigger_custom.py L1Trigger/Configuration/python/

# analyzers
cvs co UserCode/L1TriggerDPG

cvs co -d       edwenger/HiVertexAnalyzer UserCode/edwenger/HiVertexAnalyzer
cvs co -d       edwenger/HiTrkEffAnalyzer UserCode/edwenger/HiTrkEffAnalyzer

cvs co          RecoHI/HiJetAlgos
cvs co          HeavyIonsAnalysis/Configuration
cvs co -r hi441_1 -d CmsHi/JetAnalysis UserCode/CmsHi/JetAnalysis

cvs co -r V02-02-01  RecoHI/HiEgammaAlgos
cvs co -r V00-00-15  RecoHI/Configuration
cvs co -d CmsHi/PhotonAnalysis UserCode/CmsHi/PhotonAnalysis

cvs co MuTrig/HLTMuTree UserCode/Miheejo/MuTrig/HLTMuTree

scram build -c
