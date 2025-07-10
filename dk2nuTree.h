#ifndef dk2nuTree_h
#define dk2nuTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TString.h>
#include <vector>

const Int_t kMaxnuray = 10000;
const Int_t kMaxancestor = 80;

class dk2nuTree {
public:
  TTree *fChain = nullptr;
  Int_t fCurrent = -1;
  //Long64_t totalPOT;
  
  TString _fnameid;
  TString ofname;

  // // Input variables
  double decay_nimpwt;
  double decay_pdpx, decay_pdpy, decay_pdpz;
  double decay_ppenergy;
  double decay_muparpx, decay_muparpy, decay_muparpz, decay_mupare;
  double decay_necm;

// Add these to your public section
  std::vector<float> proton_startx, proton_starty, proton_startz;
  std::vector<float> proton_startpx, proton_startpy, proton_startpz;
  std::vector<int> proton_parIndex;

  
  int nuray_;
  double nuray_px[kMaxnuray];
  double nuray_py[kMaxnuray];
  double nuray_pz[kMaxnuray];
  double nuray_E[kMaxnuray];
  double nuray_wgt[kMaxnuray];

  Int_t ancestor_;
  Int_t ancestor_pdg[kMaxancestor];
  double ancestor_startx[kMaxancestor];
  double ancestor_starty[kMaxancestor];
  double ancestor_startz[kMaxancestor];
  double ancestor_startpx[kMaxancestor];
  double ancestor_startpy[kMaxancestor];
  double ancestor_startpz[kMaxancestor];
  int ancestor_parIndex[kMaxancestor];
  //Int_t potnum;

  // // Branches
  // //TBranch *b_dk2nu_potnum;
  TBranch *b_dk2nu_decay_pdpx;
  TBranch *b_dk2nu_decay_pdpy;
  TBranch *b_dk2nu_decay_pdpz;
  TBranch *b_dk2nu_decay_ppenergy;
  TBranch *b_dk2nu_decay_muparpx;
  TBranch *b_dk2nu_decay_muparpy;
  TBranch *b_dk2nu_decay_muparpz;
  TBranch *b_dk2nu_decay_mupare;
  TBranch *b_dk2nu_decay_necm;
  TBranch *b_dk2nu_decay_nimpwt;

  TBranch *b_dk2nu_nuray_;
  TBranch *b_nuray_px;
  TBranch *b_nuray_py;
  TBranch *b_nuray_pz;
  TBranch *b_nuray_E;
  TBranch *b_nuray_wgt;

  TBranch *b_dk2nu_ancestor_;
  TBranch *b_ancestor_pdg;
  TBranch *b_ancestor_startx;
  TBranch *b_ancestor_starty;
  TBranch *b_ancestor_startz;
  TBranch *b_ancestor_startpx;
  TBranch *b_ancestor_startpy;
  TBranch *b_ancestor_startpz;
  TBranch *b_ancestor_parIndex;

  // Methods
  dk2nuTree(TTree *tree = nullptr);
  dk2nuTree(TTree* tree, TString fnameID);
  dk2nuTree(int type, TString fnameID, TTree *tree = nullptr);
  dk2nuTree(TString filename, TString fnameID);
  virtual ~dk2nuTree();

  virtual void Init(TTree *tree);
  virtual void Loop(TFile* tf);  // new version with output file argument
  virtual Bool_t Notify();
  virtual Int_t GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void Show(Long64_t entry = -1);
  virtual Int_t Cut(Long64_t entry);
};

#endif
