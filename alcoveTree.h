#ifndef alcoveTree_h
#define alcoveTree_h

#include <TTree.h>
#include <TBranch.h>
#include <TFile.h>
#include <TString.h>

const Int_t kMax = 20;

class alcoveTree {
 public:
  TTree *fAlcove = nullptr;
  TString ofname;
  TString _fnameid;

  Int_t run, event, np;
  Int_t ID[kMax][3], ParID[kMax][3], PDG[kMax][3];
  Bool_t alcoveBool[kMax][3];
  Double_t impwt[kMax][3], x[kMax][3], y[kMax][3], z[kMax][3];
  Double_t startx[kMax][3], starty[kMax][3], startz[kMax][3];
  Double_t startE[kMax][3], px[kMax][3], py[kMax][3], pz[kMax][3];
  Double_t m[kMax][3], E[kMax][3], dEdx[kMax][3], dEdx_ion[kMax][3];
  Double_t cosTheta[kMax][3], edep[kMax][3], edep_ion[kMax][3];
  Int_t nsteps[kMax][3], parPDG[kMax][3];
  Double_t parE[kMax][3], parX[kMax][3], parY[kMax][3], parZ[kMax][3];
  Double_t parPX[kMax][3], parPY[kMax][3], parPZ[kMax][3];

  // Branch pointers
  TBranch *b_run = nullptr, *b_event = nullptr, *b_np = nullptr;
  TBranch *b_ID = nullptr, *b_ParID = nullptr, *b_PDG = nullptr;
  TBranch *b_alcoveBool = nullptr, *b_impwt = nullptr;
  TBranch *b_x = nullptr, *b_y = nullptr, *b_z = nullptr;
  TBranch *b_startx = nullptr, *b_starty = nullptr, *b_startz = nullptr;
  TBranch *b_startE = nullptr, *b_px = nullptr, *b_py = nullptr, *b_pz = nullptr;
  TBranch *b_m = nullptr, *b_E = nullptr, *b_dEdx = nullptr, *b_dEdx_ion = nullptr;
  TBranch *b_cosTheta = nullptr, *b_edep = nullptr, *b_edep_ion = nullptr;
  TBranch *b_nsteps = nullptr, *b_parPDG = nullptr, *b_parE = nullptr;
  TBranch *b_parX = nullptr, *b_parY = nullptr, *b_parZ = nullptr;
  TBranch *b_parPX = nullptr, *b_parPY = nullptr, *b_parPZ = nullptr;

  alcoveTree(TTree *tree = nullptr) {}
  virtual ~alcoveTree() {}

  virtual void SetAlcoveTree(TTree *tree);
  virtual void Loop();
};

#endif
