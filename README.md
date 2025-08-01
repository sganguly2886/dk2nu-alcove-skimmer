# dk2nu Skimming Tools

This repository contains code to reduce large DUNE flux `dk2nu` and `AlcoveTracks` ROOT files into lightweight skims for analysis.

Local Skimming (for interactive or small-scale use)

## Contents
run_dk2nuTree.cpp, dk2nuTree.C/h — Skimming of dk2nuTree
processOneFile.cpp, alcoveTree.C/h — Skimming of AlcoveTracks tree
run_skimming_batch.sh — Batch processor for full list
run_skimming_batch_test.sh — Test run (12 files)
filelist_24.txt — Example file list

## How to Run Locally
Compile the code:
g++ -o run_dk2nuTree run_dk2nuTree.cpp dk2nuTree.C $(root-config --cflags --libs)
g++ -o processOneFile processOneFile.cpp alcoveTree.C $(root-config --cflags --libs)

## Edit run_skimming_batch.sh to define:
INPUT_DIR=...
NJOBS=...

## Run a test batch:
bash run_skimming_batch_test.sh

## Run full batch:
bash run_skimming_batch.sh

## Outputs will appear in:

dk2nu_skims/     # skimmed dk2nu trees
alcove_skims/    # skimmed AlcoveTracks trees
logs/            # log files

## Grid Skimming (FNAL Jobsub/Condor)

Use this for large-scale submission to the FNAL grid.

## Grid-Specific Files
run_dk2nuTree — Compiled binary
skim_job_wrapper_final.sh — Job wrapper for the grid
filelist_12.txt — Input file list (1 line = 1 .root file)
submit_batch.sh — Optional batch submit script

## Environment Setup (on GPVM)
source /cvmfs/dune.opensciencegrid.org/dune-spack/spack-v0.23.0-fermi/BIWG/setup-env.sh
spacktivate g4lbnf-geant4-10-4-3-gcc-12-2-0-cxx17-prof-almalinux9-x86_64_v2

## Prepare the Working Directory
Your skim_job_test_final/ directory should contain:
skim_job_test_final/
├── run_dk2nuTree
├── filelist_12.txt
├── submit_batch.sh        # optional
 Do NOT include skim_job_wrapper_final.sh or .tar.bz2 files in this directory.

## Create the Tarball
From the parent directory:
tar --exclude="*.tar.bz2" --exclude="skim_job_wrapper_final.sh" \
    -cjvf skim_job_test_final.tar.bz2 -C . skim_job_test_final


## Setup VOMS Proxy (Required for Grid Access and ifdh)
Before copying files to /pnfs or submitting jobs using jobsub, you must generate a valid VOMS proxy:
voms-proxy-init -noregen -rfc -hours 24 -voms dune:/dune/Role=Analysis

You can verify your proxy is valid with:
voms-proxy-info

It should show a non-zero time left, like:
subject   : /DC=org/...
timeleft  : 23:59:59

    
## Upload to dCache

ifdh cp --force skim_job_test_final.tar.bz2 /pnfs/dune/scratch/users/<YOUR_USERNAME>/
Replace <YOUR_USERNAME> with your actual Fermilab username.

## Submit a Single Test Job
jobsub_submit \
  --group=dune \
  --role=Analysis \
  --memory=4000MB \
  --disk=4GB \
  --expected-lifetime=1h \
  --resource-provides=usage_model=OPPORTUNISTIC \
  --tar_file_name=/pnfs/dune/scratch/users/<YOUR_USERNAME>/skim_job_test_final.tar.bz2 \
  -d OUTPUT /pnfs/dune/scratch/users/<YOUR_USERNAME>/skimmed_output/ \
  -L /pnfs/dune/scratch/users/<YOUR_USERNAME>/jobsub_logs/skim_job_$RANDOM.log \
  file:///full/path/to/skim_job_wrapper_final.sh \
  /pnfs/dune/persistent/users/<YOUR_USERNAME>/your_input_file.root


## Submit Batch Jobs Using a File List
Make sure filelist_12.txt contains full paths to .root files.

Example submit_batch.sh:

#!/bin/bash

USERNAME=sganguly
WRAPPER=/exp/dune/app/users/$USERNAME/AL9/skim_job_test_final/skim_job_wrapper_final.sh
TARBALL=/pnfs/dune/scratch/users/$USERNAME/skim_job_test_final.tar.bz2
OUTDIR=/pnfs/dune/scratch/users/$USERNAME/skimmed_output
LOGDIR=/pnfs/dune/scratch/users/$USERNAME/jobsub_logs

while read -r INPUT_FILE; do
  jobsub_submit \
    --group=dune \
    --role=Analysis \
    --memory=4000MB \
    --disk=4GB \
    --expected-lifetime=1h \
    --resource-provides=usage_model=OPPORTUNISTIC \
    --tar_file_name=$TARBALL \
    -d OUTPUT $OUTDIR \
    -L $LOGDIR/skim_job_$RANDOM.log \
    file://$WRAPPER \
    $INPUT_FILE
done < filelist_12.txt


Make it executable and run:
chmod +x submit_batch.sh
./submit_batch.sh


## Monitor and Fetch Logs
Check status:
jobsub_q --user <YOUR_USERNAME>

Fetch job logs:
jobsub_fetchlog --jobid <JOB_ID> --user <YOUR_USERNAME>



## Grid Skimming (AlcoveTracks)

1. Prepare Tarball

cd /exp/dune/app/users/sganguly/duneML_nominal/g4lbne
tar -cjvf skim_job_alcove_final.tar.bz2 -C skim_job_alcove_final .
ifdh cp --force skim_job_alcove_final.tar.bz2 /pnfs/dune/scratch/users/sganguly/

2. Submit Job

jobsub_submit \
  --memory=4000MB --disk=4GB --expected-lifetime=1h \
  --group=dune --role=Analysis \
  --resource-provides=usage_model=OPPORTUNISTIC \
  --tar_file_name=/pnfs/dune/scratch/users/sganguly/skim_job_alcove_final.tar.bz2 \
  -d OUTPUT /pnfs/dune/scratch/users/sganguly/skimmed_output_alcove/ \
  -L /pnfs/dune/scratch/users/sganguly/jobsub/skim_job_alcove_$RANDOM.log \
  file:///exp/dune/app/users/sganguly/duneML_nominal/g4lbne/skim_job_alcove_final/skim_job_wrapper_alcove.sh \
  /pnfs/dune/scratch/users/sganguly/fluxfiles/g4lbne/your_input_file.root

3. Check Output

/pnfs/dune/scratch/users/sganguly/skimmed_output_alcove/

4. Monitor Jobs

jobsub_q --user sganguly -G dune

