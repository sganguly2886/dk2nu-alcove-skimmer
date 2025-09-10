# dk2nu-alcove-skimmer

Scripts and skimmers for processing DUNE flux `dk2nu.root` files with optional `alcoveTree`.

---

## Quick Start

### 1. Setup environment

```bash
# Load DUNE spack environment
source /cvmfs/dune.opensciencegrid.org/dune-spack/spack-v0.23.0-fermi/BIWG/setup-env.sh
spacktivate g4lbnf-geant4-10-4-3-gcc-12-2-0-cxx17-prof-almalinux9-x86_64_v2

# Export helper vars
export G4LBNE_DIR=$PWD
alias ls='ls --color=auto'


2. Get a valid token (for xrootd / dCache access)

htgettoken -i dune -a htvaultprod.fnal.gov
export BEARER_TOKEN_FILE=/run/user/$(id -u)/bt_u$(id -u)


3. Compile
./compile.sh


4. Run skimming

Edit run_skimming_batch.sh to set:

INPUT_DIR → path to flux .dk2nu.root files (under /pnfs/dune/...)

OUTDIR_DCACHE → output directory (under /pnfs/fnal.gov/usr/dune/...)

Then run with desired parallelism:

# Example: run 4 jobs in parallel
NJOBS=4 bash run_skimming_batch.sh


5. Validate outputs

Check job completion:
# Count finished jobs
grep -H " END:" logs/*.log | wc -l

# List outputs and sizes
ls -lh /pnfs/dune/scratch/users/$USER/skimtests

# Verify with xrdfs
xrdfs fndca1.fnal.gov ls /pnfs/fnal.gov/usr/dune/scratch/users/$USER/skimtests \
  | grep -c '\.combined\.root$'
