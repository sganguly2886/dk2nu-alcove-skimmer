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

jobsub_submit \
  --memory=4000MB --disk=4GB --expected-lifetime=1h \
  --group=dune --role=Analysis \
  --resource-provides=usage_model=OPPORTUNISTIC \
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
