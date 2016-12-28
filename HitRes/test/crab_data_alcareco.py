from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'ZeroBias_Run2016G-SiStripCalMinBias-23Sep2016-v1_ALCARECO'
config.General.workArea = 'Tracker/ZeroBias_Run2016G-SiStripCalMinBias-23Sep2016-v1_ALCARECO'
config.section_('JobType')
config.JobType.psetName = 'simhitres_working_data_alcareco.py'
config.JobType.pluginName = 'Analysis'
config.JobType.outputFiles = ['HitRes_data_alcareco.root']
config.JobType.maxMemoryMB = 2500
# config.JobType.numCores = 8
config.section_('Data')
config.Data.inputDataset = '/ZeroBias/Run2016G-SiStripCalMinBias-23Sep2016-v1/ALCARECO'
config.Data.outputDatasetTag = 'ZeroBias_Run2016G-SiStripCalMinBias-23Sep2016-v1_ALCARECO'
config.Data.publishDBS = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet'
config.Data.publication = False
config.Data.unitsPerJob = 4
config.Data.splitting = 'LumiBased'
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Cert_271036-280385_13TeV_PromptReco_Collisions16_JSON_NoL1T_v2.txt'
# config.Data.runRange = '280000-280385'
config.Data.outLFNDirBase = '/store/user/defilip/Tracker/ZeroBias_Run2016G-SiStripCalMinBias-23Sep2016-v1_ALCARECO'
config.section_('User')
config.section_('Site')
config.Site.storageSite = 'T2_IT_Bari'
