#include "alcoveTree.h"
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input.dk2nu.root>" << std::endl;
    return 1;
  }

  TString inputFile = argv[1];

  // Try to open the input ROOT file
  TFile* file = new TFile(inputFile, "READ");
  if (!file || file->IsZombie()) {
    std::cerr << "Error: Cannot open file " << inputFile << std::endl;
    return 1;
  }

  // Load AlcoveTracks tree
  TTree* alcoveTreePtr = nullptr;
  file->GetObject("AlcoveTracks", alcoveTreePtr);

  if (!alcoveTreePtr) {
    std::cerr << "Error: AlcoveTracks tree not found in " << inputFile << std::endl;
    file->Close();
    delete file;
    return 1;
  }

  // Generate a skim tag based on the input filename
  std::string fname = inputFile.Data();
  size_t lastSlash = fname.find_last_of('/');
  std::string tag = fname.substr(lastSlash + 1);  // e.g. neutrino_09913.dk2nu.root
  tag = tag.substr(0, tag.find(".dk2nu.root"));   // strip .dk2nu.root

  // Prepare and run the skimmer
  alcoveTree myTree;
  myTree._fnameid = tag;
  myTree.ofname = tag + ".skim.root";
  myTree.SetAlcoveTree(alcoveTreePtr);
  myTree.Loop();

  file->Close();
  delete file;

  return 0;
}
