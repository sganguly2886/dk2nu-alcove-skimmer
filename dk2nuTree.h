#ifndef dk2nuTree_h
#define dk2nuTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TTree.h>
#include <TBranch.h>
#include <iostream>
#include <vector>
#include <string> // Add this line

using namespace std; // Add this line

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxnuray = 3;
const Int_t kMaxancestor = 20;
const Int_t kMaxtraj = 10;

class dk2nuTree {
 public:
  TTree *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t fCurrent; //!current Tree number in a TChain
  Long64_t totalPOT;  

  TH1D* timesmear = new TH1D();
  TString ofname;
  //int theseed;
  TString _fnameid;

  // Declaration of leaf types
  Int_t job;
  Int_t potnum;
  Int_t decay_norig;
  Int_t decay_ndecay;
  Int_t decay_ntype;
  Double_t decay_vx;
  Double_t decay_vy;
  Double_t decay_vz;
  Double_t decay_pdpx;
  Double_t decay_pdpy;
  Double_t decay_pdpz;
  Double_t decay_ppdxdz;
  Double_t decay_ppdydz;
  Double_t decay_pppz;
  Double_t decay_ppenergy;
  Int_t decay_ppmedium;
  Int_t decay_ptype;
  Double_t decay_muparpx;
  Double_t decay_muparpy;
  Double_t decay_muparpz;
  Double_t decay_mupare;
  Double_t decay_necm;
  Double_t decay_nimpwt;
  Int_t nuray_;
  Double_t nuray_px[kMaxnuray];   //[nuray_]
  Double_t nuray_py[kMaxnuray];   //[nuray_]
  Double_t nuray_pz[kMaxnuray];   //[nuray_]
  Double_t nuray_E[kMaxnuray];   //[nuray_]
  Double_t nuray_wgt[kMaxnuray];   //[nuray_]
  Int_t ancestor_;
  Int_t ancestor_pdg[kMaxancestor];   //[ancestor_]
  Double_t ancestor_startx[kMaxancestor];   //[ancestor_]
  Double_t ancestor_starty[kMaxancestor];   //[ancestor_]
  Double_t ancestor_startz[kMaxancestor];   //[ancestor_]
  Double_t ancestor_startt[kMaxancestor];   //[ancestor_]
  Double_t ancestor_startpx[kMaxancestor];   //[ancestor_]
  Double_t ancestor_startpy[kMaxancestor];   //[ancestor_]
  Double_t ancestor_startpz[kMaxancestor];   //[ancestor_]
  Double_t ancestor_stoppx[kMaxancestor];   //[ancestor_]
  Double_t ancestor_stoppy[kMaxancestor];   //[ancestor_]
  Double_t ancestor_stoppz[kMaxancestor];   //[ancestor_]
  Double_t ancestor_polx[kMaxancestor];   //[ancestor_]
  Double_t ancestor_poly[kMaxancestor];   //[ancestor_]
  Double_t ancestor_polz[kMaxancestor];   //[ancestor_]
  Double_t ancestor_pprodpx[kMaxancestor];   //[ancestor_]
  Double_t ancestor_pprodpy[kMaxancestor];   //[ancestor_]
  Double_t ancestor_pprodpz[kMaxancestor];   //[ancestor_]
  Int_t ancestor_nucleus[kMaxancestor];   //[ancestor_]
  Int_t ancestor_parIndex[kMaxancestor];   //[ancestor_]
  string ancestor_proc[kMaxancestor];
  string ancestor_ivol[kMaxancestor];
  string ancestor_imat[kMaxancestor];
  Double_t ppvx;
  Double_t ppvy;
  Double_t ppvz;
  Double_t tgtexit_tvx;
  Double_t tgtexit_tvy;
  Double_t tgtexit_tvz;
  Double_t tgtexit_tpx;
  Double_t tgtexit_tpy;
  Double_t tgtexit_tpz;
  Int_t tgtexit_tptype;
  Int_t tgtexit_tgen;
  Int_t traj_;
  Double_t traj_trkx[kMaxtraj];   //[traj_]
  Double_t traj_trky[kMaxtraj];   //[traj_]
  Double_t traj_trkz[kMaxtraj];   //[traj_]
  Double_t traj_trkpx[kMaxtraj];   //[traj_]
  Double_t traj_trkpy[kMaxtraj];   //[traj_]
  Double_t traj_trkpz[kMaxtraj];   //[traj_]
  Int_t flagbits;
  vector<int> vint;
  vector<double> vdbl;

  // List of branches
  TBranch *b_dk2nu_job;   //!
  TBranch *b_dk2nu_potnum;   //!
  TBranch *b_dk2nu_decay_norig;   //!
  TBranch *b_dk2nu_decay_ndecay;   //!
  TBranch *b_dk2nu_decay_ntype;   //!
  TBranch *b_dk2nu_decay_vx;   //!
  TBranch *b_dk2nu_decay_vy;   //!
  TBranch *b_dk2nu_decay_vz;   //!
  TBranch *b_dk2nu_decay_pdpx;   //!
  TBranch *b_dk2nu_decay_pdpy;   //!
  TBranch *b_dk2nu_decay_pdpz;   //!
  TBranch *b_dk2nu_decay_ppdxdz;   //!
  TBranch *b_dk2nu_decay_ppdydz;   //!
  TBranch *b_dk2nu_decay_pppz;   //!
  TBranch *b_dk2nu_decay_ppenergy;   //!
  TBranch *b_dk2nu_decay_ppmedium;   //!
  TBranch *b_dk2nu_decay_ptype;   //!
  TBranch *b_dk2nu_decay_muparpx;   //!
  TBranch *b_dk2nu_decay_muparpy;   //!
  TBranch *b_dk2nu_decay_muparpz;   //!
  TBranch *b_dk2nu_decay_mupare;   //!
  TBranch *b_dk2nu_decay_necm;   //!
  TBranch *b_dk2nu_decay_nimpwt;   //!
  TBranch *b_dk2nu_nuray_;   //!
  TBranch *b_nuray_px;   //!
  TBranch *b_nuray_py;   //!
  TBranch *b_nuray_pz;   //!
  TBranch *b_nuray_E;   //!
  TBranch *b_nuray_wgt;   //!
  TBranch *b_dk2nu_ancestor_;   //!
  TBranch *b_ancestor_pdg;   //!
  TBranch *b_ancestor_startx;   //!
  TBranch *b_ancestor_starty;   //!
  TBranch *b_ancestor_startz;   //!
  TBranch *b_ancestor_startt;   //!
  TBranch *b_ancestor_startpx;   //!
  TBranch *b_ancestor_startpy;   //!
  TBranch *b_ancestor_startpz;   //!
  TBranch *b_ancestor_stoppx;   //!
  TBranch *b_ancestor_stoppy;   //!
  TBranch *b_ancestor_stoppz;   //!
  TBranch *b_ancestor_polx;   //!
  TBranch *b_ancestor_poly;   //!
  TBranch *b_ancestor_polz;   //!
  TBranch *b_ancestor_pprodpx;   //!
  TBranch *b_ancestor_pprodpy;   //!
  TBranch *b_ancestor_pprodpz;   //!
  TBranch *b_ancestor_nucleus;   //!
  TBranch *b_ancestor_parIndex;   //!
  TBranch *b_ancestor_proc;   //!
  TBranch *b_ancestor_ivol;   //!
  TBranch *b_ancestor_imat;   //!
  TBranch *b_dk2nu_ppvx;   //!
  TBranch *b_dk2nu_ppvy;   //!
  TBranch *b_dk2nu_ppvz;   //!
  TBranch *b_dk2nu_tgtexit_tvx;   //!
  TBranch *b_dk2nu_tgtexit_tvy;   //!
  TBranch *b_dk2nu_tgtexit_tvz;   //!
  TBranch *b_dk2nu_tgtexit_tpx;   //!
  TBranch *b_dk2nu_tgtexit_tpy;   //!
  TBranch *b_dk2nu_tgtexit_tpz;   //!
  TBranch *b_dk2nu_tgtexit_tptype;   //!
  TBranch *b_dk2nu_tgtexit_tgen;   //!
  TBranch *b_dk2nu_traj_;   //!
  TBranch *b_traj_trkx;   //!
  TBranch *b_traj_trky;   //!
  TBranch *b_traj_trkz;   //!
  TBranch *b_traj_trkpx;   //!
  TBranch *b_traj_trkpy;   //!
  TBranch *b_traj_trkpz;   //!
  TBranch *b_dk2nu_flagbits;   //!
  TBranch *b_dk2nu_vint;   //!
  TBranch *b_dk2nu_vdbl;   //!

  // Constructor declarations
  dk2nuTree(TTree *tree = 0);
  dk2nuTree(int type, TString fnameID, TTree *tree = 0);
  dk2nuTree(TString filename, TString fnameID);
  virtual ~dk2nuTree();

  // Method declarations
  virtual Int_t GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void Init(TTree *tree);
  virtual void Loop(TFile* tf);
  virtual Bool_t Notify();
  virtual void Show(Long64_t entry = -1);
  virtual Int_t Cut(Long64_t entry);

};

#endif
