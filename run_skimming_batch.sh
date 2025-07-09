#!/bin/bash

# CONFIG ----------------------------
COMBINED_SKIMMER=./processOneFile    # Combined skimmer executable
INPUT_DIR=/pnfs/dune/scratch/users/sganguly/fluxfiles/g4lbne/v3r5p10-22-gf42572f/QGSP_BERT/OfficialEngDesignSept2021_nominal_pbeamX_plus0mm_QGSP_BERT_seed1/neutrino/flux/
OUTPUT_DIR=/pnfs/dune/scratch/users/ebarlas/skimmed_files/boosted_stats/0mm 
NJOBS=12                             # Number of parallel jobs
# -----------------------------------

mkdir -p logs "$OUTPUT_DIR"

echo "Finding input files in $INPUT_DIR..."
find "$INPUT_DIR" -name "*.dk2nu.root" > filelist_boosted_0mm.txt

echo "Starting combined skimming with $NJOBS parallel jobs on all files..."

cat filelist_boosted_0mm.txt | xargs -P $NJOBS -I{} bash -c '
  infile="{}"
  fname=$(basename "$infile" .dk2nu.root)
  outfile="${fname}.combined.root"
  outpath="'"$OUTPUT_DIR"'/${outfile}"
  logf="logs/${fname}_combined.log"

  # Function to print CPU and memory info
  print_resources() {
    echo "Resource usage at $(date +"%T"):"
    echo "-------------------------"
    # Total CPU load avg (1,5,15 min)
    uptime | awk -F "load average:" "{ print \"Load average:\" \$2 }"
    # Memory usage summary
    free -h | head -n 2
    echo ""
  }

  if [ -f "$outpath" ]; then
    echo "[$(date +"%T")] SKIP: $outpath already exists."
    echo "Already exists: $outpath. Skipping." > "$logf"
    exit 0
  fi

  echo "[$(date +"%T")] START: Processing $infile" > "$logf"
  print_resources >> "$logf"

  '"$COMBINED_SKIMMER"' "$infile" >> "$logf" 2>&1

  print_resources >> "$logf"
  echo "[$(date +"%T")] END: Processing $infile" >> "$logf"

  if [ -f "$outfile" ]; then
    mv "$outfile" "'"$OUTPUT_DIR"'"/
    echo "[$(date +"%T")] DONE: Moved $outfile to '"$OUTPUT_DIR"'/" >> "$logf"
    echo "✔ Processed: $outfile"
  else
    echo "[$(date +"%T")] ERROR: Combined skim not found for $fname" >> "$logf"
    echo "❌ Error: Combined skim not created for $infile"
  fi
'

echo "All files processed."
echo "Combined skims moved to $OUTPUT_DIR/"
echo "Logs saved in logs/"
