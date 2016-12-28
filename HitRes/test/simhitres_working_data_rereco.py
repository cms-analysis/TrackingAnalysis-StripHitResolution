# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --filein file:HIG-RunIIFall15DR76-00047_step1.root --fileout file:HIG-RunIIFall15DR76-00047.root --mc --eventcontent AODSIM,DQM --runUnscheduled --datatier AODSIM,DQMIO --conditions 76X_mcRun2_asymptotic_v12 --step RAW2DIGI,L1Reco,RECO,EI,DQM:DQMOfflinePOGMC --era Run2_25ns --python_filename /afs/cern.ch/cms/PPD/PdmV/work/McM/submit/HIG-RunIIFall15DR76-00047/HIG-RunIIFall15DR76-00047_2_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 82
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('reRECO',eras.Run2_2016)

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.AlCaRecoStreams_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('PhysicsTools.PatAlgos.slimming.metFilterPaths_cff')
process.load('DQMOffline.Configuration.DQMOffline_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:A0039DC3-729C-E511-B5AD-0025905A60C6.root'   # GEN-SIM-DIGI-RAW-HLTBEBUG
        # 'file:/lustre/cms/store/user/defilip/Tracker/RelValSingleMuPt1000_UP15_RECO/RelValSingleMuPt1000_UP15_RECO.root'
        #'file:02F6E5E1-EA4F-E511-8503-BCEE7B2FE075.root'  # ZZ
        #'file:21.0_SingleMuPt100+SingleMuPt100INPUT+DIGI+RECO+HARVEST/step2.root'
#        'file:500206.0_ZMM_13+ZMM_13INPUT+DIGIPRMXUP15_PU50+RECOPRMXUP15_PU50+HARVESTUP15_PU50/step2.root'
#'/store/mc/RunIIWinter15GS/GluGluHToZZTo4L_M125_13TeV_powheg_JHUgen_pythia8/GEN-SIM/MCRUN2_71_V1-v1/70000/00F09B02-D311-E511-863D-1CC1DE1D19FE.root'
        #'file:TT_TuneCUETP8M1_13TeV-powheg-pythia8_0024BC01-CC8D-E511-935E-441EA1733A7A.root'
        #'file:step2.root'
        #'file:MinBias_Alcareco_B2411E72-338E-E511-96D8-02163E0140E3.root'
#        'file:MinBias_Alcareco_02DFD1A4-9891-E511-AA47-02163E01388C.root'
#        'file:2015D_RAW-RECO_F61E786F-8884-E511-BF40-02163E0142CD.root'
        #'file:SingleMuon_RAW_FEF27273-8782-E511-B890-02163E0146EF.root'
        #'file:SingleMuon_Run2016H_RAW_005F10CC-447E-E611-99F4-FA163E6CD01D.root'
  #      'file:222F6965-2262-E611-8E8C-02163E0144B1_RAW.root'
        'file:000856E7-7462-E611-A599-02163E014537_RAW.root'
        ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:82'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_2016SeptRepro_v3','')


process.TFileService = cms.Service("TFileService",
    fileName = cms.string('HitRes_data_rereco.root')
)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.eventinterpretation_step = cms.Path(process.EIsequence)


## The good primary vertex filter ____________________________________________||
process.primaryVertexFilter = cms.EDFilter(
    "VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
    filter = cms.bool(True)
    )

## The beam scraping filter __________________________________________________||
process.noscraping = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25),
    src = cms.untracked.InputTag("generalTracks")
)


process.load("RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi")
process.load("RecoTracker.TrackProducer.TrackRefitter_cfi")
process.TrackRefitter.src = cms.InputTag("generalTracks")


process.analysis = cms.EDAnalyzer("SimHitRes",    
    tracks = cms.InputTag("TrackRefitter"),
    trajectories = cms.InputTag("TrackRefitter"),                
    associatePixel = cms.bool(False),
    associateStrip = cms.bool(False),
    associateRecoTracks = cms.bool(False),    
    pairsOnly = cms.bool(False),
    minMomentum = cms.double(10),
    genTruth = cms.bool(False),
    ROUList = cms.vstring(
        'g4SimHitsTrackerHitsTIBLowTof', 
        'g4SimHitsTrackerHitsTIBHighTof', 
        'g4SimHitsTrackerHitsTIDLowTof', 
        'g4SimHitsTrackerHitsTIDHighTof', 
        'g4SimHitsTrackerHitsTOBLowTof', 
        'g4SimHitsTrackerHitsTOBHighTof', 
        'g4SimHitsTrackerHitsTECLowTof', 
        'g4SimHitsTrackerHitsTECHighTof'
        'TrackerHitsTIBLowTof', 
        'TrackerHitsTIBHighTof', 
        'TrackerHitsTIDLowTof', 
        'TrackerHitsTIDHighTof', 
        'TrackerHitsTOBLowTof', 
        'TrackerHitsTOBHighTof', 
        'TrackerHitsTECLowTof', 
        'TrackerHitsTECHighTof'
        ),                                                                 
    stripSimLinkSrc = cms.InputTag("simSiStripDigis")
)


process.aPath = cms.Path(process.primaryVertexFilter+process.noscraping+
                         process.TrackRefitter+process.analysis)


#process.output = cms.OutputModule("PoolOutputModule",
#    splitLevel = cms.untracked.int32(0),
#    outputCommands = cms.untracked.vstring('keep *_*_*_*'),                              
#    fileName = cms.untracked.string('out_data.root'),
#)

#process.o = cms.EndPath(process.output)


# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.eventinterpretation_step,process.aPath)


# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.RecoTLR import customisePostEra_Run2_2016 

process = customisePostEra_Run2_2016(process)

from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# Automatic addition of the customisation function from SimGeneral.MixingModule.fullMixCustomize_cff                                
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn

#call to customisation function setCrossingFrameOn imported from SimGeneral.MixingModule.fullMixCustomize_cff
process = setCrossingFrameOn(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


