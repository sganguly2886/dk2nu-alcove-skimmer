#include "dk2nuTree.h"
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

  // Open input ROOT file
  TFile* input = new TFile(inputFile, "READ");
  if (!input || input->IsZombie()) {
    std::cerr << "Error: Cannot open file " << inputFile << std::endl;
    return 1;
  }

  // Get trees
  TTree* dk2nuTreePtr = nullptr;
  TTree* alcoveTreePtr = nullptr;
  input->GetObject("dk2nuTree", dk2nuTreePtr);
  input->GetObject("AlcoveTracks", alcoveTreePtr);

  if (!dk2nuTreePtr || !alcoveTreePtr) {
    std::cerr << "Error: Missing dk2nuTree or AlcoveTracks in " << inputFile << std::endl;
    input->Close();
    delete input;
    return 1;
  }

  // Generate tag from input filename
  std::string fname = inputFile.Data();
  size_t lastSlash = fname.find_last_of('/');
  std::string tag = fname.substr(lastSlash + 1);
  tag = tag.substr(0, tag.find(".dk2nu.root"));

  // Create output file for both skims
  TFile* output = new TFile((tag + ".combined.root").c_str(), "RECREATE");

  // Run DK2NU skim using new constructor (tree + tag)
  dk2nuTree dkSkimmer(dk2nuTreePtr, tag.c_str());
  dkSkimmer.Loop(output);

  // Run Alcove skim
  alcoveTree alcSkimmer;
  alcSkimmer._fnameid = tag;
  alcoveTreePtr->SetMakeClass(1);  // Ensure compatibility
  alcSkimmer.SetAlcoveTree(alcoveTreePtr);
  alcSkimmer.Loop(output);

  // Clean up
  output->Close();
  delete output;
  input->Close();
  delete input;

  std::cout << "Combined skim written to: " << tag << ".combined.root" << std::endl;

  return 0;
}
