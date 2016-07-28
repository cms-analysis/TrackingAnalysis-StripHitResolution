import FWCore.ParameterSet.Config as cms
process = cms.Process("ReRec")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.Validation_cff')
process.load('DQMOffline.Configuration.DQMOfflineMC_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load("CondCore.DBCommon.CondDBSetup_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    #'file:////eos/uscms/store/user/nmccoll/GenSamples/SingleMuPt100_10PerEvent/job_1.root'
    #'file:A0039DC3-729C-E511-B5AD-0025905A60C6.root',
    #'file:BA6C6DCA-729C-E511-B7CC-003048FFD770.root'
    'file:AA167045-C49C-E511-9852-0025905A607A.root',
    'file:C21FEA3B-C49C-E511-A039-0025905A60BE.root'
    )
)


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Random generator
#process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#    LuminosityBlock = cms.PSet(
#        initialSeed = cms.untracked.uint32(1),
#        engineName = cms.untracked.string('TRandom3')
#    )
#)

# process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
# process.load('Configuration/StandardSequences/MagneticField_AutoFromDBCurrent_cff')



process.mix.playback = True
process.mix.digitizers = cms.PSet()
#for a in process.aliases: delattr(process, a)
#process.RandomNumberGeneratorService.restoreStateLabel=cms.untracked.string("randomEngineStateProducer")
from Configuration.AlCa.GlobalTag import GlobalTag
# process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc_25ns14e33_v4', '')

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('out.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)


process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.TrackRefitter.src = cms.InputTag("generalTracks")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('HitRes.root')
)



# import FWCore.ParameterSet.VarParsing as VarParsing
# options = VarParsing.VarParsing ('analysis')
# options.register ('numEv',
#                   7145, # default value
#                   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
#                   VarParsing.VarParsing.varType.int,          # string, int, or float
#                   "Number of events to process")
# options.register ('inPath',
#                   "in.root", # default value
#                   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
#                   VarParsing.VarParsing.varType.string,          # string, int, or float
#                   "inputFile")
# options.register ('outPath',
#                   "test.root", # default value
#                   VarParsing.VarParsing.multiplicity.singleton, # singleton or list
#                   VarParsing.VarParsing.varType.string,          # string, int, or float
#                   "outputFile")
# options.parseArguments()
# process.source.fileNames = cms.untracked.vstring(options.inPath)
# process.maxEvents.input = options.numEv 
# process.FEVTDEBUGHLToutput.fileName = options.outPath


def InputConfiguration(postfix, process):
  import os
  allNames                = vars(process).keys()
  for name in allNames:
    object                = getattr(process, name)
    try:
      if hasattr(object, "fileName"):
        (name, extension) = os.path.splitext(object.fileName.value())
        object.fileName   = "%s%s%s" % (name, postfix, extension)
      elif hasattr(object, "outputPath"):
        (name, extension) = os.path.splitext(object.outputPath.value())
        object.outputPath = "%s%s%s" % (name, postfix, extension)
    except AttributeError:
      pass
InputConfiguration("", process)


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
#process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.prevalidation_step = cms.Path(process.prevalidation)
process.dqmoffline_step = cms.Path(process.DQMOffline)
process.validation_step = cms.EndPath(process.validation)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)



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



process.analysis = cms.EDAnalyzer("SimHitRes",    
    trajectories = cms.InputTag("generalTracks"),
    associatePixel = cms.bool(False),
    associateStrip = cms.bool(True),
    associateRecoTracks = cms.bool(False),    
    pairsOnly = cms.bool(False),
    minMomentum = cms.double(10),
    genTruth = cms.bool(True),
    ROUList = cms.vstring(
        'g4SimHitsTrackerHitsTIBLowTof', 
        'g4SimHitsTrackerHitsTIBHighTof', 
        'g4SimHitsTrackerHitsTIDLowTof', 
        'g4SimHitsTrackerHitsTIDHighTof', 
        'g4SimHitsTrackerHitsTOBLowTof', 
        'g4SimHitsTrackerHitsTOBHighTof', 
        'g4SimHitsTrackerHitsTECLowTof', 
        'g4SimHitsTrackerHitsTECHighTof'
        ),

)

process.aPath = cms.Path(process.mix * process.primaryVertexFilter  * process.noscraping*process.analysis)

#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step, process.reconstruction_step,process.aPath )

