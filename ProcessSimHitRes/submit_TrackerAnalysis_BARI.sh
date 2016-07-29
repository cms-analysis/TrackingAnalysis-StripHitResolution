#!/bin/bash


mkdir -p /lustre/cms/store/user/defilip/MonoHiggs/TrackerReco/80X/jobdir

echo "Running TrackerAnalysis Analysis with executables RunTrackerAnalysis"
source /cvmfs/cms.cern.ch/cmsset_default.sh

export LD_LIBRARY_PATH=lib:$LD_LIBRARY_PATH
export PATH=path:$PATH


if [ -d "$_CONDOR_SCRATCH_DIR" ]; then
    workdir=`echo $_CONDOR_SCRATCH_DIR`;
    cd ${workdir};
else 
    workdir=`echo $PWD`;
    cd ${workdir};
fi

savedir=`echo /lustre/cms/store/user/defilip/MonoHiggs/TrackerReco/80X/jobdir`

echo "Working dir is $workdir"
echo "Saving dir is $savedir"

echo "Compiling the macros"
bash compileTrackerAnalysis.sh 

./RunTrackerAnalysis >& ${workdir}/TrackerAnalysis.log

mv -f ${workdir}/TrackerAnalysis.log  ${savedir}/.
mv -f ${workdir}/HitResolution_compOutputs.root ${savedir}/.



