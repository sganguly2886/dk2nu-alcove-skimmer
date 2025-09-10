#!/bin/bash
set -Eeuo pipefail

# --------- BASE PATHS (absolute) ---------------------------------------------
TOPDIR="$(cd "$(dirname "$0")" && pwd)"
LOGDIR="${TOPDIR}/logs"
WORKROOT="/dev/shm/${USER}_skim_xrd"
mkdir -p "${LOGDIR}" "${WORKROOT}"

# Keep files group-writable; quiet xrootd logs
umask 0002
export XRD_LOGLEVEL=Warning

# --------- USER CONFIG (env overrides allowed) --------------------------------
COMBINED_SKIMMER="${COMBINED_SKIMMER:-${TOPDIR}/processOneFile}"

# Local PNFS mount (what 'find' sees on the GPVM)
INPUT_DIR="${INPUT_DIR:-/pnfs/dune/scratch/users/sganguly/fluxfiles/g4lbne/v3r5p10-27-gcb7d966/QGSP_BERT/swappedshield_2pt7mmsigma_protonbeam-X2-hornA_tilt_up_2sigma_test/neutrino/flux}"

# Remote dCache namespace (what xrootd sees)
OUTDIR_DCACHE="${OUTDIR_DCACHE:-/pnfs/fnal.gov/usr/dune/scratch/users/sganguly/skimtests}"

# Parallelism (env overrides this default)
NJOBS="${NJOBS:-10}"

# --------- XROOTD ENDPOINT ----------------------------------------------------
STAGE_HOST="root://fndca1.fnal.gov"

# Ensure remote output directory exists
xrdfs fndca1.fnal.gov mkdir -p "${OUTDIR_DCACHE}" || true

echo "Finding input files in ${INPUT_DIR} ..."
mapfile -t FILES < <(find "${INPUT_DIR}" -maxdepth 1 -type f -name '*.dk2nu.root' | sort)
echo "Found ${#FILES[@]} files"

# Nothing to do?
if [[ ${#FILES[@]} -eq 0 ]]; then
  echo "No input files found. Check INPUT_DIR: ${INPUT_DIR}"
  exit 0
fi

export COMBINED_SKIMMER STAGE_HOST OUTDIR_DCACHE LOGDIR WORKROOT

stage_and_run() {
  local infile_local="$1"   # /pnfs/dune/... (local GPVM mount)
  local base fname wdir logf out_local_guess out_remote infile_xrd produced

  base="$(basename "${infile_local}" .dk2nu.root)"
  fname="${base}"

  # Map local mount -> remote dCache namespace for xrootd
  # /pnfs/dune/...  -> /pnfs/fnal.gov/usr/dune/...
  infile_xrd="${infile_local/\/pnfs\/dune/\/pnfs\/fnal.gov\/usr\/dune}"
  infile_xrd="${STAGE_HOST}${infile_xrd}"

  wdir="${WORKROOT}/${fname}"
  logf="${LOGDIR}/${fname}_combined.log"
  out_local_guess="${fname}.combined.root"
  out_remote="${OUTDIR_DCACHE}/${out_local_guess}"

  mkdir -p "${wdir}"
  echo "[$(date +%F_%T)] START: ${infile_xrd}" | tee "${logf}"
  echo "[INFO] Workdir: ${wdir}" | tee -a "${logf}"

  # ----- stage-in (xrdcp -> local /dev/shm) ----------------------------------
  echo "[STAGE-IN] xrdcp -> ${wdir}/input.dk2nu.root" | tee -a "${logf}"
  if ! xrdcp -f "${infile_xrd}" "${wdir}/input.dk2nu.root" >> "${logf}" 2>&1; then
    echo "[ERROR] xrdcp stage-in failed" | tee -a "${logf}"
    rm -rf "${wdir}"
    return 1
  fi

  # ----- run skimmer on local file -------------------------------------------
  (
    cd "${wdir}"
    echo "[RUN ] ${COMBINED_SKIMMER} input.dk2nu.root" | tee -a "${logf}"
    if ! "${COMBINED_SKIMMER}" "input.dk2nu.root" >> "${logf}" 2>&1; then
      echo "[ERROR] Skimmer failed" | tee -a "${logf}"
      exit 2
    fi

    produced="$(ls -1t *.root 2>/dev/null | head -n1 || true)"
    if [[ -z "${produced}" ]]; then
      echo "[ERROR] No ROOT output produced" | tee -a "${logf}"
      exit 3
    fi

    # Normalize name if needed
    if [[ "${produced}" != "${out_local_guess}" ]]; then
      mv -f "${produced}" "${out_local_guess}"
    fi
  ) || { rm -rf "${wdir}"; return 1; }

  # ----- stage-out (local -> dCache) -----------------------------------------
  echo "[STAGE-OUT] ${wdir}/${out_local_guess} -> ${STAGE_HOST}${out_remote}" | tee -a "${logf}"

  # If the destination already exists, skip upload (idempotent behavior)
  if xrdfs fndca1.fnal.gov stat "${out_remote}" >/dev/null 2>&1; then
    echo "[SKIP ] Destination exists: ${out_remote}" | tee -a "${logf}"
  else
    # Upload without -f so no DELETE/overwrite is attempted
    if ! xrdcp "${wdir}/${out_local_guess}" "${STAGE_HOST}${out_remote}" >> "${logf}" 2>&1; then
      # If failure was only due to missing DELETE permission during an implicit overwrite, treat as uploaded
      if grep -q "denied activity DELETE" "${logf}"; then
        echo "[OK* ] Treating as already uploaded (no DELETE perm): ${out_remote}" | tee -a "${logf}"
      else
        echo "[ERROR] xrdcp stage-out failed" | tee -a "${logf}"
        rm -rf "${wdir}"
        return 1
      fi
    fi
  fi

  # Verify upload or pre-existence
  if xrdfs fndca1.fnal.gov stat "${out_remote}" >> "${logf}" 2>&1; then
    echo "[OK  ] Wrote ${out_remote}" | tee -a "${logf}"
  else
    echo "[ERROR] stat failed after upload" | tee -a "${logf}"
    rm -rf "${wdir}"
    return 1
  fi

  echo "[$(date +%F_%T)] END: ${infile_xrd}" | tee -a "${logf}"
  rm -rf "${wdir}"
}

export -f stage_and_run

echo "Starting combined skimming with ${NJOBS} parallel jobs…"
printf '%s\0' "${FILES[@]}" | xargs -0 -n1 -P "${NJOBS}" bash -c 'stage_and_run "$@"' _

echo "All files processed. Outputs should be under:"
echo "  ${OUTDIR_DCACHE}"
echo "Logs saved in:"
echo "  ${LOGDIR}"
