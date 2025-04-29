#!/bin/bash
set -e
set -o pipefail

echo "=== Starting skim_job_wrapper_final.sh ==="

# Load environment
echo "Loading CVMFS environment..."
source /cvmfs/dune.opensciencegrid.org/dune-spack/spack-v0.23.0-fermi/BIWG/setup-env.sh
spack env activate g4lbnf-geant4-10-4-3-gcc-12-2-0-cxx17-prof-almalinux9-x86_64_v2


echo "=== Workdir ==="
ls

echo "=== INPUT_TAR_DIR_LOCAL ==="
ls "$INPUT_TAR_DIR_LOCAL"

echo "=== INPUT_TAR_FILE ==="
ls "$INPUT_TAR_FILE"

# Move to condor scratch directory
echo "Moving to _CONDOR_SCRATCH_DIR..."
cd "$_CONDOR_SCRATCH_DIR"
echo "Current working dir: $(pwd)"
echo "Initial contents:"
ls -lh

echo "After tar extraction, contents of scratch dir:"
ls -lh

echo "Looking specifically for run_dk2nuTree:"
ls -l run_dk2nuTree || echo "run_dk2nuTree not found"


# Move unpacked files to current dir
if [ -d 0 ]; then
  echo "Found directory 0, moving contents up..."
  mv 0/* .
  rmdir 0
fi

if [ -d skim_job_test_final ]; then
  echo "Found skim_job_test_final directory, moving contents up..."
  mv skim_job_test_final/* .
  rmdir skim_job_test_final
fi

echo "Directory contents after moving:"
ls -lh

# Handle input
INPUT_FILE="$1"
if [ -z "$INPUT_FILE" ]; then
  echo "ERROR: No input file given!"
  exit 1
fi
echo "Input file: $INPUT_FILE"

# Copy input file locally
echo "Copying input file using ifdh cp..."
if ! ifdh cp "$INPUT_FILE" ./input.root; then
  echo "ERROR: Failed to copy input file with ifdh cp"
  exit 2
fi
echo "Successfully copied input file to input.root"

# Check if run_dk2nuTree is available
if [ ! -x "$INPUT_TAR_DIR_LOCAL/skim_job_test_final/run_dk2nuTree" ]; then
  echo "ERROR: run_dk2nuTree not found or not executable!"
  exit 127
fi
echo "Found run_dk2nuTree at $INPUT_TAR_DIR_LOCAL/skim_job_test_final/run_dk2nuTree"

# Run processing
echo "Running run_dk2nuTree..."
if ! "$INPUT_TAR_DIR_LOCAL/skim_job_test_final/run_dk2nuTree" input.root output.root; then
  echo "WARNING: run_dk2nuTree failed when creating output.root, trying fallback filename 'output'..."
  if ! "$INPUT_TAR_DIR_LOCAL/skim_job_test_final/run_dk2nuTree" input.root output; then
    echo "ERROR: run_dk2nuTree failed twice!"
    exit 3
  fi
fi

# Fix double extension case
if [ -f output.root.root ]; then
  echo "Found output.root.root — renaming to output.root"
  mv output.root.root output.root
fi

# Prepare output
mkdir -p "$CONDOR_DIR_OUTPUT"
if [ -f output.root ]; then
  OUTPUT_NAME="skimmed_output_${CLUSTER:-X}_${PROCESS:-Y}.root"
  mv output.root "$CONDOR_DIR_OUTPUT/$OUTPUT_NAME"
  echo "Output saved as $CONDOR_DIR_OUTPUT/$OUTPUT_NAME"
else
  echo "WARNING: output.root not found — writing dummy OUTPUT/.empty_file"
  touch $CONDOR_DIR_OUTPUT/.empty_file
fi


# List final contents                                                                                          
echo "Final OUTPUT directory listing:"
ls -lh $CONDOR_DIR_OUTPUT/


# Dummy file for condor
touch dummy.txt

echo "=== skim_job_wrapper_final.sh completed successfully ==="
