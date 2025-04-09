#!/bin/bash

# CONFIG ----------------------------
DK2NU_SKIMMER=./run_dk2nuTree        # dk2nu skimmer executable
ALCOVE_SKIMMER=./processOneFile      # alcove skimmer executable
INPUT_DIR=/pnfs/dune/scratch/users/sganguly/fluxfiles/g4lbne/v3r5p10/QGSP_BERT/OfficialEngDesignSept2021_defaultshielding_hornB_move_left_2sigma_pbeam_plus1mm/neutrino/flux
NJOBS=12                             # number of parallel jobs
# -----------------------------------

mkdir -p logs dk2nu_skims alcove_skims

echo "Finding input files in $INPUT_DIR..."
find "$INPUT_DIR" -name "*.dk2nu.root" > filelist.txt

echo "Starting combined skimming with $NJOBS parallel jobs..."
cat filelist.txt | xargs -n 1 -P $NJOBS -I{} bash -c '
  infile="{}"
  fname=$(basename "$infile" .dk2nu.root)

  dklog="logs/${fname}_dk2nu.log"
  alclog="logs/${fname}_alcove.log"

  echo "Processing DK2NU: $infile" > "$dklog"
  '"$DK2NU_SKIMMER"' "$infile" "$fname" >> "$dklog" 2>&1

  echo "Processing ALCOVE: $infile" > "$alclog"
  '"$ALCOVE_SKIMMER"' "$infile" >> "$alclog" 2>&1

  # Output filenames
  dkout="${fname}.root"
  alcout="${fname}.skim.root"

  # Move outputs
  if [ -f "$dkout" ]; then
    mv "$dkout" dk2nu_skims/
    echo "Moved DK2NU skim to dk2nu_skims/" >> "$dklog"
  else
    echo "DK2NU skim not found!" >> "$dklog"
  fi

  if [ -f "$alcout" ]; then
    mv "$alcout" alcove_skims/
    echo "Moved ALCOVE skim to alcove_skims/" >> "$alclog"
  else
    echo "ALCOVE skim not found!" >> "$alclog"
  fi
'

echo "All jobs completed."
echo "DK2NU skims in: dk2nu_skims/"
echo "Alcove skims in: alcove_skims/"
echo "Logs in: logs/"
