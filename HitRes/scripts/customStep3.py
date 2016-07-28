# Auto generated configuration file
# using: 
# Revision: 1.11 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 -s HARVESTING:validationHarvesting+dqmHarvesting --harvesting AtRunEnd --conditions auto:mc --mc --filein=file:/opt/ppd/cms/users/aph37105/hit_res/step2_RAW2DIGI_RECO_VALIDATION_DQM.root --fileout=file:/opt/ppd/cms/users/aph37105/hit_res/DQM_V0001_R000000001__Global__CMSSW_X_Y_Z__RECO.root --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('HARVESTING')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EDMtoMEAtRunEnd_cff')
process.load('Configuration.StandardSequences.Harvesting_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
    'file:///uscms/home/nmccoll/nobackup/2011-04-15-susyra2/WorkDPG/test2.root',
    
    
    ),
    processingMode = cms.untracked.string('RunsAndLumis')
)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    fileMode = cms.untracked.string('FULLMERGE')
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.11 $'),
    annotation = cms.untracked.string('step3 nevts:1'),
    name = cms.untracked.string('Applications')
)

# Output definition

# Additional output definition
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

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:mc', '')

# Path and EndPath definitions
process.edmtome_step = cms.Path(process.EDMtoME)
process.validationprodHarvesting = cms.Path(process.hltpostvalidation_prod+process.postValidation_gen)
process.dqmHarvestingPOGMC = cms.Path(process.DQMOffline_SecondStep_PrePOGMC)
process.validationHarvestingFS = cms.Path(process.HarvestingFastSim)
process.validationpreprodHarvesting = cms.Path(process.postValidation_preprod+process.hltpostvalidation_preprod+process.postValidation_gen)
process.validationHarvestingHI = cms.Path(process.postValidationHI)
process.genHarvesting = cms.Path(process.postValidation_gen)
process.dqmHarvestingPOG = cms.Path(process.DQMOffline_SecondStep_PrePOG)
process.alcaHarvesting = cms.Path()
process.dqmsave_step = cms.Path(process.DQMSaver)

# Schedule definition
process.schedule = cms.Schedule(process.edmtome_step,process.validationHarvesting,process.dqmHarvesting,process.dqmsave_step)

