#define dk2nuTree_cxx
#include "dk2nuTree.h"
#include <TH2.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// Constructor: From file and identifier
dk2nuTree::dk2nuTree(TString filename, TString fnameID) : fChain(0){
  _fnameid = fnameID;
  ofname = fnameID + ".root";

  TFile *f = TFile::Open(filename);
  if (!f || f->IsZombie()) {
    cerr << "Error opening file: " << filename << endl;
    return;
  }

  TTree *tree = nullptr;
  f->GetObject("dk2nuTree", tree);
  if (!tree) {
    cerr << "Error: dk2nuTree not found in file: " << filename << endl;
    return;
  }

  //if (!tree->GetListOfBranches()->FindObject("potnum")) {
  //cerr << "Error: Branch 'potnum' not found in dk2nuTree" << endl;
  //tree->Print();  // dump what's available
  //return;
  //}
  
  Init(tree);
  Loop();
  delete f;
}

// Destructor
dk2nuTree::~dk2nuTree() {
  if (!fChain)return;
  delete fChain->GetCurrentFile();
}

// Initialize branches
void dk2nuTree::Init(TTree *tree) {
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  //fChain->SetMakeClass(1);

  //fChain->SetBranchAddress("potnum", &potnum, &b_dk2nu_potnum);
  fChain->SetBranchAddress("dk2nu.decay.pdpx", &decay_pdpx, &b_dk2nu_decay_pdpx);
  fChain->SetBranchAddress("dk2nu.decay.pdpy", &decay_pdpy, &b_dk2nu_decay_pdpy);
  fChain->SetBranchAddress("dk2nu.decay.pdpz", &decay_pdpz, &b_dk2nu_decay_pdpz);
  fChain->SetBranchAddress("dk2nu.decay.ppenergy", &decay_ppenergy, &b_dk2nu_decay_ppenergy);
  fChain->SetBranchAddress("dk2nu.decay.muparpx", &decay_muparpx, &b_dk2nu_decay_muparpx);
  fChain->SetBranchAddress("dk2nu.decay.muparpy", &decay_muparpy, &b_dk2nu_decay_muparpy);
  fChain->SetBranchAddress("dk2nu.decay.muparpz", &decay_muparpz, &b_dk2nu_decay_muparpz);
  fChain->SetBranchAddress("dk2nu.decay.mupare", &decay_mupare, &b_dk2nu_decay_mupare);
  fChain->SetBranchAddress("dk2nu.decay.necm", &decay_necm, &b_dk2nu_decay_necm);
  fChain->SetBranchAddress("dk2nu.decay.nimpwt", &decay_nimpwt, &b_dk2nu_decay_nimpwt);
  fChain->SetBranchAddress("dk2nu.nuray", &nuray_, &b_dk2nu_nuray_);
  fChain->SetBranchAddress("dk2nu.nuray.px", nuray_px, &b_nuray_px);
  fChain->SetBranchAddress("dk2nu.nuray.py", nuray_py, &b_nuray_py);
  fChain->SetBranchAddress("dk2nu.nuray.pz", nuray_pz, &b_nuray_pz);
  fChain->SetBranchAddress("dk2nu.nuray.E", nuray_E, &b_nuray_E);
  fChain->SetBranchAddress("dk2nu.nuray.wgt", nuray_wgt, &b_nuray_wgt);
  fChain->SetBranchAddress("dk2nu.ancestor", &ancestor_, &b_dk2nu_ancestor_);
  fChain->SetBranchAddress("dk2nu.ancestor.pdg", ancestor_pdg, &b_ancestor_pdg);
  fChain->SetBranchAddress("dk2nu.ancestor.startx", ancestor_startx, &b_ancestor_startx);
  fChain->SetBranchAddress("dk2nu.ancestor.starty", ancestor_starty, &b_ancestor_starty);
  fChain->SetBranchAddress("dk2nu.ancestor.startz", ancestor_startz, &b_ancestor_startz);
  fChain->SetBranchAddress("dk2nu.ancestor.startpx", ancestor_startpx, &b_ancestor_startpx);
  fChain->SetBranchAddress("dk2nu.ancestor.startpy", ancestor_startpy, &b_ancestor_startpy);
  fChain->SetBranchAddress("dk2nu.ancestor.startpz", ancestor_startpz, &b_ancestor_startpz);
  fChain->SetBranchAddress("dk2nu.ancestor.parIndex", ancestor_parIndex, &b_ancestor_parIndex);
}

// Main loop
template<typename T>
static void clear_vector(std::vector<T>& vec) {
  std::vector<T>().swap(vec);
}

void dk2nuTree::Loop() {
  if (!fChain) return;

  fChain->GetEntry(0);
  std::cout << "Debug: Entry 0 ancestor_ = " << ancestor_ << std::endl;
  for (int i=0; i<ancestor_; ++i) {
      std::cout << "   ancestor_pdg[" << i << "] = " << ancestor_pdg[i] << std::endl;
  }

  
  TFile *tf = new TFile(ofname, "recreate");
  TTree *outtree = new TTree("dk2nuOutTree", "Filtered dk2nu Tree");

  std::cout << "Starting Loop on " << fChain->GetEntriesFast() << " entries" << std::endl;

  
  // Output vars
  //std::vector<float> proton_startx, proton_starty, proton_startz;
  //std::vector<float> proton_startpx, proton_startpy, proton_startpz;
  //std::vector<int> proton_parIndex;
  int nuray_size;
  double nuray_px_out[10], nuray_py_out[10], nuray_pz_out[10];
  double nuray_E_out[10], nuray_wgt_out[10];
  //int mpotnum;
  
  // Reuse decay variables
  outtree->Branch("decay_pdpx", &decay_pdpx);
  outtree->Branch("decay_pdpy", &decay_pdpy);
  outtree->Branch("decay_pdpz", &decay_pdpz);
  outtree->Branch("decay_ppenergy", &decay_ppenergy);
  outtree->Branch("decay_muparpx", &decay_muparpx);
  outtree->Branch("decay_muparpy", &decay_muparpy);
  outtree->Branch("decay_muparpz", &decay_muparpz);
  outtree->Branch("decay_mupare", &decay_mupare);
  outtree->Branch("decay_necm", &decay_necm);
  outtree->Branch("decay_nimpwt", &decay_nimpwt);

  outtree->Branch("nuray_px", nuray_px_out, "nuray_px[10]/D");
  outtree->Branch("nuray_py", nuray_py_out, "nuray_py[10]/D");
  outtree->Branch("nuray_pz", nuray_pz_out, "nuray_pz[10]/D");
  outtree->Branch("nuray_E", nuray_E_out, "nuray_E[10]/D");
  outtree->Branch("nuray_wgt", nuray_wgt_out, "nuray_wgt[10]/D");
  outtree->Branch("nuray_size", &nuray_size, "nuray_size/I");

  outtree->Branch("proton_startx", &proton_startx);
  outtree->Branch("proton_starty", &proton_starty);
  outtree->Branch("proton_startz", &proton_startz);
  outtree->Branch("proton_startpx", &proton_startpx);
  outtree->Branch("proton_startpy", &proton_startpy);
  outtree->Branch("proton_startpz", &proton_startpz);
  outtree->Branch("proton_parIndex", &proton_parIndex);
  //outtree->Branch("potnum", &mpotnum);

  
  Long64_t nentries = fChain->GetEntriesFast();
  for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
    LoadTree(jentry);
    fChain->GetEntry(jentry);
   
    //std::cout << "Entry " << jentry << " has ancestor_ = " << ancestor_ << std::endl;
  

    //if (jentry % 250000 == 0) cout << jentry << " out of " << nentries << " " << potnum << " POT " << endl;
    //mpotnum = potnum;
    // Debugging: Print the current potnum value
    //if (jentry % 250000 == 0) cout << jentry << " out of " << nentries << " " << "Current POT: " << potnum << endl;

    //if (ancestor_ == 0) {
    //std::cout << "Entry " << jentry << " has zero ancestors!" << std::endl;
    //}

    if (ancestor_ > kMaxancestor) {
        std::cerr << "Error: ancestor_ size " << ancestor_ << " exceeds max at entry " << jentry
                  << " in file " << _fnameid << std::endl;
    }

    //if (jentry < 5) {
    //  std::cout << "Entry " << jentry << " ancestor_ = " << ancestor_ << std::endl;
    //  for (int i = 0; i < ancestor_; ++i) {
    //      std::cout << " ancestor_pdg[" << i << "] = " << ancestor_pdg[i] << std::endl;
    //  }
    //}


     // Clear vectors at the start of each event
    proton_startx.clear();
    proton_starty.clear();
    proton_startz.clear();
    proton_startpx.clear();
    proton_startpy.clear();
    proton_startpz.clear();
    proton_parIndex.clear();
    
     // clear_vector(proton_startx);
     //clear_vector(proton_starty);
     //clear_vector(proton_startz);
     //clear_vector(proton_startpx);
     //clear_vector(proton_startpy);
     //clear_vector(proton_startpz);
     //clear_vector(proton_parIndex);

    //nuray_size = std::min((int)nuray_, 10);

    if (nuray_ < 0 || nuray_ > kMaxnuray) {
    std::cerr << "Invalid nuray_ size: " << nuray_ << std::endl;
    continue;
    }
     if (ancestor_ < 0 || ancestor_ > kMaxancestor) {
    std::cerr << "Entry " << jentry << ": Invalid ancestor_ size = " << ancestor_ << ", skipping entry." << std::endl;
    continue;
  }
    nuray_size = std::min(nuray_, 10);  // cap output to 10

    for (int i = 0; i < nuray_size; ++i) {
      nuray_px_out[i] = nuray_px[i];
      nuray_py_out[i] = nuray_py[i];
      nuray_pz_out[i] = nuray_pz[i];
      nuray_E_out[i]  = nuray_E[i];
      nuray_wgt_out[i]= nuray_wgt[i];
    }


    
    for (int i = 0; i < ancestor_; ++i) {
      if (ancestor_pdg[i] == 2212) {  // proton
      //std::cout << "   ancestor_pdg[" << i << "] = " << ancestor_pdg[i] << std::endl;
        proton_startx.push_back(ancestor_startx[i]);
        proton_starty.push_back(ancestor_starty[i]);
        proton_startz.push_back(ancestor_startz[i]);
        proton_startpx.push_back(ancestor_startpx[i]);
        proton_startpy.push_back(ancestor_startpy[i]);
        proton_startpz.push_back(ancestor_startpz[i]);
        proton_parIndex.push_back(ancestor_parIndex[i]);
	//std::cout << "  --> Proton found at i = " << i << std::endl;
	}
    }

    //std::cout << "Proton vector size at entry " << jentry << ": " << proton_startx.size() << std::endl;

    
    outtree->Fill();
  }
  //totalPOT += potnum;

  //std::ofstream myoutfile;
  //myoutfile.open("POTnum.txt", std::ios_base::app);
  //myoutfile << _fnameid << " " << mpotnum << " " <<totalPOT<<endl;

  outtree->Write();
  tf->Close();
  delete tf;
}
Bool_t dk2nuTree::Notify() {
  return kTRUE;
}

Int_t dk2nuTree::GetEntry(Long64_t entry) {
  return fChain ? fChain->GetEntry(entry) : 0;
}

Long64_t dk2nuTree::LoadTree(Long64_t entry) {
  if (!fChain)return -5; 
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  fCurrent = fChain->GetTreeNumber();
  return centry;
}

void dk2nuTree::Show(Long64_t entry) {
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t dk2nuTree::Cut(Long64_t) {
  return 1;  // Accept all entries
}
