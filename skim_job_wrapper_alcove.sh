#!/bin/bash
set -e
set -o pipefail

echo "=== Starting skim_job_wrapper_alcove.sh ==="

# Load environment
echo "Loading CVMFS environment..."
source /cvmfs/dune.opensciencegrid.org/dune-spack/spack-v0.23.0-fermi/BIWG/setup-env.sh
spack env activate g4lbnf-geant4-10-4-3-gcc-12-2-0-cxx17-prof-almalinux9-x86_64_v2

# Initial diagnostics
echo "=== Workdir ==="
pwd
ls -lh

echo "=== INPUT_TAR_DIR_LOCAL ==="
ls -lh "$INPUT_TAR_DIR_LOCAL"

echo "=== INPUT_TAR_FILE ==="
ls -lh "$INPUT_TAR_FILE"

# Move to condor scratch directory
echo "Moving to _CONDOR_SCRATCH_DIR..."
cd "$_CONDOR_SCRATCH_DIR"
echo "Current working dir: $(pwd)"
echo "Initial contents:"
ls -lh

# Unpacking tarball will happen automatically; now check results
echo "After tar extraction, contents of scratch dir:"
ls -lh

# Move unpacked contents up if needed
if [ -d 0 ]; then
  echo "Found directory 0, moving contents up..."
  mv 0/* .
  rmdir 0
fi

if [ -d skim_job_alcove_final ]; then
  echo "Found skim_job_alcove_final directory, moving contents up..."
  mv skim_job_alcove_final/* .
  rmdir skim_job_alcove_final
fi

echo "Directory contents after moving:"
ls -lh

# Handle input file
INPUT_FILE="$1"
if [ -z "$INPUT_FILE" ]; then
  echo "ERROR: No input file provided!"
  exit 1
fi
echo "Input file: $INPUT_FILE"

# Verify ifdh is available
command -v ifdh > /dev/null || { echo "ERROR: ifdh not found in PATH!"; exit 100; }

# Copy input file
echo "Copying input file using ifdh cp..."
if ! ifdh cp "$INPUT_FILE" ./input.root; then
  echo "ERROR: Failed to copy input file with ifdh cp"
  exit 2
fi
echo "Successfully copied input file to input.root"

# Check and run processOneFile
if [ ! -x "$INPUT_TAR_DIR_LOCAL/processOneFile" ]; then
  echo "ERROR: processOneFile not found or not executable!"
  exit 127
fi
echo "Found processOneFile"

echo "Running processOneFile..."
if ! "$INPUT_TAR_DIR_LOCAL/processOneFile" input.root output.root; then
  echo "ERROR: processOneFile failed!"
  exit 3
fi

# Rename if needed
if [ -f input.root.skim.root ]; then
  echo "Renaming input.root.skim.root to output.root"
  mv input.root.skim.root output.root
fi

# Prepare output
mkdir -p "$CONDOR_DIR_OUTPUT"
if [ -f output.root ]; then
  OUTPUT_NAME="alcove_skimmed_${CLUSTER:-X}_${PROCESS:-Y}.root"
  mv output.root "$CONDOR_DIR_OUTPUT/$OUTPUT_NAME"
  echo "Output saved as $CONDOR_DIR_OUTPUT/$OUTPUT_NAME"
else
  echo "WARNING: output.root not found — writing dummy OUTPUT/.empty_file"
  touch "$CONDOR_DIR_OUTPUT/.empty_file"
fi

echo "Final OUTPUT directory listing:"
ls -lh "$CONDOR_DIR_OUTPUT/"

# Dummy file for condor
touch dummy.txt

echo "=== skim_job_wrapper_alcove.sh completed successfully ==="
