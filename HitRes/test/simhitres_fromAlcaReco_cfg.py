import FWCore.ParameterSet.Config as cms
process = cms.Process("ReRec")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#        'file:///uscms/home/nmccoll/nobackup/2011-04-15-susyra2/WorkDPG/testAlcaReco.root'
        'file:MinBias_Alcareco_02DFD1A4-9891-E511-AA47-02163E01388C.root'
        )
)


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '76X_mcRun2_asymptotic_v12', '')

#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'FT_53_V18_AN3::All'
# process.GlobalTag.globaltag = 'START53_V27::All'

process.load("RecoTracker.MeasurementDet.MeasurementTrackerEventProducer_cfi")
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = cms.InputTag("ALCARECOTkAlMinBias")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('HitRes.root')
)


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
    src = cms.untracked.InputTag("ALCARECOTkAlMinBias")
)






process.hltFilter = cms.EDFilter("HLTHighLevel",
                              eventSetupPathsKey = cms.string(''),
                              andOr = cms.bool(True),
                              HLTPaths = cms.vstring(
                               'HLT_Physics_v*',
                               'HLT_ZeroBias_v*'),
                              throw = cms.bool(False),
                              TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
                              )



process.analysis = cms.EDAnalyzer("SimHitRes",
    tracks = cms.InputTag("TrackRefitter"),
    trajectories = cms.InputTag("TrackRefitter"),
    associatePixel = cms.bool(False),
    associateStrip = cms.bool(False),
    associateRecoTracks = cms.bool(False),
    genTruth = cms.bool(False),
    pairsOnly = cms.bool(True),
    minMomentum = cms.double(10),
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


#process.p = cms.Path(process.hltFilter * process.primaryVertexFilter  * process.noscraping* process.TrackRefitter*process.analysis)

process.p = cms.Path(process.primaryVertexFilter+process.noscraping+
                         process.TrackRefitter+process.analysis)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
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
