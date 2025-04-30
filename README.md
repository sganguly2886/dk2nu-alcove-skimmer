# dk2nu Skimming Tools

This repository contains code to reduce large DUNE flux `dk2nu` and `AlcoveTracks` ROOT files into lightweight skims for analysis.

## Contents

- `run_dk2nuTree.cpp`, `dk2nuTree.C/h` — Skimming of `dk2nuTree`
- `processOneFile.cpp`, `alcoveTree.C/h` — Skimming of `AlcoveTracks` tree
- `run_skimming_batch.sh` — Batch processor for all files
- `run_skimming_batch_test.sh` — Test run for 12 files
- `filelist_24.txt` — Example input list for testing

## How to Run

1. Compile the code

g++ -o run_dk2nuTree run_dk2nuTree.cpp dk2nuTree.C $(root-config --cflags --libs)

g++ -o processOneFile processOneFile.cpp alcoveTree.C $(root-config --cflags --libs)

2. Edit run_skimming_batch.sh to set

INPUT_DIR
NJOBS

3. Run the test batch (12 files)

bash run_skimming_batch_test.sh


4. Run full batch

bash run_skimming_batch.sh


Output

dk2nu_skims/ — reduced dk2nu trees

alcove_skims/ — reduced alcove trees

logs/ — logs for debugging

# dk2nu Skimming Tools (Grid Version)

This repository contains scripts for skimming large DUNE flux `dk2nu` files on the FNAL Grid (using jobsub/Condor).

## Contents

- `skim_job_wrapper_final.sh` — Job wrapper script for Condor jobs.
- `submit_batch.sh` — Batch submission script to process many files at once.
- `filelist_12.txt` — Example input list of `dk2nu.root` files for batch submission.

## How to Run

### 1. Setup your tarball

Prepare your tarball with the compiled `run_dk2nuTree` binary (no need to include the wrapper):

```bash
cd /your/workdir/
tar --exclude="*.tar.bz2" --exclude="skim_job_wrapper_final.sh" -cjvf skim_job_test_final.tar.bz2 -C skim_job_test_final .
ifdh cp --force skim_job_test_final.tar.bz2 /pnfs/dune/scratch/users/YOUR_USERNAME/


### 2. Submit a single job manually
# Alcove Skimming Workflow

This repository contains scripts and executables to perform skimming of dk2nu flux ROOT files for the DUNE Alcove setup using `processOneFile`.

## Contents

- `skim_job_wrapper_alcove.sh` – Job wrapper script used by `jobsub_submit`. Handles environment setup, input fetching, and file processing.
- `processOneFile` – Compiled binary that reads input `.root` files and writes skimmed output.
- `README.md` – This file.

## How to Use

### 1. Build Tarball

From within `g4lbne/`, run:

```bash
tar -cjvf skim_job_alcove_final.tar.bz2 -C skim_job_alcove_final .

### 2. Upload to pnfs

ifdh cp --force skim_job_alcove_final.tar.bz2 /pnfs/dune/scratch/users/YOUR_USERNAME/

### 3. Submit grid job
 
jobsub_submit \
  --memory=4000MB --disk=4GB --expected-lifetime=1h \
  --group=dune --role=Analysis \
  --resource-provides=usage_model=OPPORTUNISTIC \
<<<<<<< HEAD
  --tar_file_name=/pnfs/dune/scratch/users/YOUR_USERNAME/skim_job_test_final.tar.bz2 \
  -d OUTPUT /pnfs/dune/scratch/users/YOUR_USERNAME/skimmed_output/ \
  -L /pnfs/dune/scratch/users/YOUR_USERNAME/jobsub/skim_job_test_$RANDOM.log \
  file:///full/path/to/skim_job_wrapper_final.sh \
  /pnfs/dune/scratch/users/YOUR_USERNAME/path/to/your_input_file.dk2nu.root


### 3. Submit many jobs automatically

Prepare a list of input files (filelist_12.txt) then run:

./submit_batch.sh


### 4. Check Outputs
Skimmed ROOT files will appear in:
/pnfs/dune/scratch/users/YOUR_USERNAME/skimmed_output/

Logs will appear in:
/pnfs/dune/scratch/users/YOUR_USERNAME/jobsub/


### 5. Monitor jobs

jobsub_q --user YOUR_USERNAME -G dune



run_dk2nuTree must be built beforehand.

CVMFS and Spack environment is auto-loaded inside the job wrapper.

Jobs use OPPORTUNISTIC Grid slots by default.

  --tar_file_name=/pnfs/dune/scratch/users/YOUR_USERNAME/skim_job_alcove_final.tar.bz2 \
  -d OUTPUT /pnfs/dune/scratch/users/YOUR_USERNAME/skimmed_output_alcove/ \
  -L /pnfs/dune/scratch/users/YOUR_USERNAME/jobsub/skim_job_alcove_$RANDOM.log \
  file:///exp/dune/app/users/YOUR_USERNAME/duneML_nominal/g4lbne/skim_job_alcove_final/skim_job_wrapper_alcove.sh \
  /pnfs/dune/scratch/users/YOUR_USERNAME/fluxfiles/g4lbne/your_input_file.root


### 4. Check output
/pnfs/dune/scratch/users/YOUR_USERNAME/skimmed_output_alcove/

### 5. Monitor jobs
watch -n 5 'jobsub_q --user YOUR_USERNAME -G dune'


Grid Skimming (AlcoveTracks)

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

