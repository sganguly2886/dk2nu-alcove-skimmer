# dk2nu and AlcoveTree Skimming Tools

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
