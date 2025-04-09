#include "alcoveTree.h"
#include <TFile.h>
#include <TTree.h>
#include <iostream>

using namespace std;

void alcoveTree::SetAlcoveTree(TTree *tree) {
  if (!tree) return;
  fAlcove = tree;
  fAlcove->SetMakeClass(1);

  fAlcove->SetBranchAddress("run", &run, &b_run);
  fAlcove->SetBranchAddress("event", &event, &b_event);
  fAlcove->SetBranchAddress("np", &np, &b_np);
  fAlcove->SetBranchAddress("ID", ID, &b_ID);
  fAlcove->SetBranchAddress("alcoveBool", alcoveBool, &b_alcoveBool);
  fAlcove->SetBranchAddress("ParID", ParID, &b_ParID);
  fAlcove->SetBranchAddress("PDG", PDG, &b_PDG);
  fAlcove->SetBranchAddress("impwt", impwt, &b_impwt);
  fAlcove->SetBranchAddress("x", x, &b_x);
  fAlcove->SetBranchAddress("y", y, &b_y);
  fAlcove->SetBranchAddress("z", z, &b_z);
  fAlcove->SetBranchAddress("startx", startx, &b_startx);
  fAlcove->SetBranchAddress("starty", starty, &b_starty);
  fAlcove->SetBranchAddress("startz", startz, &b_startz);
  fAlcove->SetBranchAddress("startE", startE, &b_startE);
  fAlcove->SetBranchAddress("px", px, &b_px);
  fAlcove->SetBranchAddress("py", py, &b_py);
  fAlcove->SetBranchAddress("pz", pz, &b_pz);
  fAlcove->SetBranchAddress("m", m, &b_m);
  fAlcove->SetBranchAddress("E", E, &b_E);
  fAlcove->SetBranchAddress("dEdx", dEdx, &b_dEdx);
  fAlcove->SetBranchAddress("dEdx_ion", dEdx_ion, &b_dEdx_ion);
  fAlcove->SetBranchAddress("cosTheta", cosTheta, &b_cosTheta);
  fAlcove->SetBranchAddress("edep", edep, &b_edep);
  fAlcove->SetBranchAddress("edep_ion", edep_ion, &b_edep_ion);
  fAlcove->SetBranchAddress("nsteps", nsteps, &b_nsteps);
  fAlcove->SetBranchAddress("parPDG", parPDG, &b_parPDG);
  fAlcove->SetBranchAddress("parE", parE, &b_parE);
  fAlcove->SetBranchAddress("parX", parX, &b_parX);
  fAlcove->SetBranchAddress("parY", parY, &b_parY);
  fAlcove->SetBranchAddress("parZ", parZ, &b_parZ);
  fAlcove->SetBranchAddress("parPX", parPX, &b_parPX);
  fAlcove->SetBranchAddress("parPY", parPY, &b_parPY);
  fAlcove->SetBranchAddress("parPZ", parPZ, &b_parPZ);
}

void alcoveTree::Loop() {
  if (!fAlcove) {
    cerr << "Error: Alcove tree not set!" << endl;
    return;
  }

  Long64_t nentries = fAlcove->GetEntriesFast();
  TFile* tf = new TFile(ofname, "RECREATE");
  TTree* outtree = fAlcove->CloneTree(0);  // Create empty copy

  for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
    fAlcove->LoadTree(jentry);
    fAlcove->GetEntry(jentry);
    // Here you could apply skimming conditions if needed
    outtree->Fill();
  }

  outtree->Write();
  tf->Close();
  cout << "Full skim written: " << ofname << endl;
}
