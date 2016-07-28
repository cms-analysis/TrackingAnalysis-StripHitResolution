import FWCore.ParameterSet.Config as cms
process = cms.Process("ReRec")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:///uscms/home/nmccoll/nobackup/2011-04-15-susyra2/WorkDPG/testAlcaReco.root'
        )
)


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')


process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'FT_53_V18_AN3::All'
# process.GlobalTag.globaltag = 'START53_V27::All'


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



process.analysis = cms.EDAnalyzer("HitRes",
    usePXB = cms.bool(False),
    usePXF = cms.bool(False),
    useTIB = cms.bool(True),
    useTOB = cms.bool(True),
    useTID = cms.bool(False),
    useTEC = cms.bool(False),
    ROUList = cms.vstring('TrackerHitsTIBLowTof',
                          'TrackerHitsTIBHighTof',
                          'TrackerHitsTOBLowTof',
                          'TrackerHitsTOBHighTof'),        
    trajectories = cms.InputTag("TrackRefitter"),
    associatePixel = cms.bool(False),
    associateStrip = cms.bool(False),
    associateRecoTracks = cms.bool(False),
    tracks = cms.InputTag("TrackRefitter"),
    barrelOnly = cms.bool(False)
)


process.p = cms.Path(process.hltFilter * process.primaryVertexFilter  * process.noscraping* process.TrackRefitter*process.analysis)