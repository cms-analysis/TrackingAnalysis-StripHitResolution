# Auto generated configuration file
# using: 
# Revision: 1.11 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 -s RAW2DIGI,RECO,VALIDATION,DQM -n 1000 --filein /store/relval/CMSSW_6_1_1-START61_V11/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/62545890-DD76-E211-BFFE-002590494FDC.root --fileout /opt/ppd/cms/users/aph37105/hit_res/step2_RAW2DIGI_RECO_VALIDATION_DQM.root --eventcontent FEVTDEBUGHLT --conditions auto:mc --mc --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
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

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(9000)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_6_1_1-START61_V11/RelValSingleMuPt100/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/62545890-DD76-E211-BFFE-002590494FDC.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.11 $'),
    annotation = cms.untracked.string('step2 nevts:1000'),
    name = cms.untracked.string('Applications')
)

# Output definition

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

# Additional output definition

# Other statements
process.mix.playback = True
process.mix.digitizers = cms.PSet()
for a in process.aliases: delattr(process, a)
process.RandomNumberGeneratorService.restoreStateLabel=cms.untracked.string("randomEngineStateProducer")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')

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
process.reconstruction_step = cms.Path(process.reconstruction)
process.prevalidation_step = cms.Path(process.prevalidation)
process.dqmoffline_step = cms.Path(process.DQMOffline)
process.validation_step = cms.EndPath(process.validation)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.prevalidation_step,process.validation_step,process.dqmoffline_step,process.endjob_step,process.FEVTDEBUGHLToutput_step)

