#!/bin/sh

export CVSROOT=:gserver:cmscvs.cern.ch:/cvs_server/repositories/CMSSW

cd $CMSSW_BASE/src
eval `scramv1 ru -sh`

# latest configurations
cvs co -r V05-00-01 SimGeneral/MixingModule/python           # for mixHiSignal_cff
cvs co -r V01-17-10 Configuration/StandardSequences          # for HiSignal_cff 
cvs co -r V11-01-00 PhysicsTools/HepMCCandAlgos/python       # for HiGenParticles_cfi
cvs co Configuration/Generator                               # for hiSignal GEN configs
cvs co -r ap33X-branch Configuration/PyReleaseValidation     # for modified ConfigBuilder
cvs co -r CMSSW_3_3_0_pre4 IOMC/RandomEngine/python          # for hiSignalLHCTransport

# HI Event Content
cvs co RecoHI/Configuration        # for modified RecoHI_EventContent_cff
cvs co -r V02-00-00 SimGeneral/Configuration    # for HiMixing_EventContent
cvs co RecoHI/HiMuonAlgos/python   # for muonRecoPbPb sequence

cvs co -d Misc UserCode/edwenger/Misc
#mv Misc/DigiHiMix_cff.py Configuration/StandardSequences/python    # hiTrackingParticles
mv Misc/ConfigBuilder_cff.py Configuration/PyReleaseValidation/python  #hiTrackingParticles without DigiHiMix
mv Misc/ReconstructionHeavyIons_cff.py Configuration/StandardSequences/python # use imported globalRecoPbPb sequence
mv Misc/IOMC_cff.py IOMC/RandomEngine/python                       # with hiSignalLHCTransport for extended geometry
rm -r Misc

# Stuff needed for PAT and Jet Analysis
#cvs co UserCode/CmsHi/JetAnalysis
#cvs co UserCode/CmsHi/HiPatAlgos
#mv UserCode/CmsHi .
#rm -r UserCode

scramv1 b






