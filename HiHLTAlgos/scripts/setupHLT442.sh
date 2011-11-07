#/bin/bash -
# setup HLT
# cf https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGlobalHLT?redirectedfrom=CMS.SWGuideGlobalHLT#in_progress_working_on_the_2011
cvs co -r V13-00-16      HLTrigger/Configuration
checkdeps -a

scram b
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

scram build -c
