1. Setup env

source /cvmfs/dune.opensciencegrid.org/dune-spack/spack-v0.23.0-fermi/BIWG/setup-env.sh 
spacktivate g4lbnf-geant4-10-4-3-gcc-12-2-0-cxx17-prof-almalinux9-x86_64_v2
export G4LBNE_DIR=`pwd`
alias ls='ls --color=auto'

2. Compile

   ./compile.sh


3. Go to run_skimming_batch.sh and change the INPUT_DIR and OUTPUT_DIR directories. Run the script in the screen session

screen -S myscreen
source /cvmfs/dune.opensciencegrid.org/dune-spack/spack-v0.23.0-fermi/BIWG/setup-env.sh 
spacktivate g4lbnf-geant4-10-4-3-gcc-12-2-0-cxx17-prof-almalinux9-x86_64_v2
export G4LBNE_DIR=`pwd`
alias ls='ls --color=auto'
bash run_skimming_batch.sh


