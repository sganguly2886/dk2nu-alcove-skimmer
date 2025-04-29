#!/bin/bash

# Set important paths
WRAPPER="/exp/dune/app/users/sganguly/duneML_nominal/g4lbne/skim_job_test_final/skim_job_wrapper_final.sh"
TARBALL="/pnfs/dune/scratch/users/sganguly/skim_job_test_final.tar.bz2"
OUTDIR="/pnfs/dune/scratch/users/sganguly/skimmed_output"
LOGDIR="/pnfs/dune/scratch/users/sganguly/jobsub"

# Loop over each line in the file
while read -r INPUT_FILE; do
  if [[ -n "$INPUT_FILE" && "$INPUT_FILE" == /* ]]; then
    echo "Submitting job for: $INPUT_FILE"
    jobsub_submit \
      --memory=4000MB --disk=4GB --expected-lifetime=1h \
      --group=dune --role=Analysis \
      --resource-provides=usage_model=OPPORTUNISTIC \
      --tar_file_name="$TARBALL" \
      -d OUTPUT "$OUTDIR" \
      -L "$LOGDIR/skim_job_test_$RANDOM.log" \
      file://"$WRAPPER" "$INPUT_FILE"
    sleep 1  # brief delay between submissions to be nice to the server
  fi
done < filelist_12.txt
