#define dk2nuTree_cxx
#include "dk2nuTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

void dk2nuTree::Loop(TFile* tf)
{
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  
  // Tree variables
  int nparticles, nutype, nuparent, mpotnum,pdg;
  double nustartx, nustarty, nustartz, nustartT;
  double nuE, bwt0, bwt1, bwt2, decay_wt;
  double nupx, nupy, nupz, nupperp, nuang;
  double parentpx, parentpy, parentpz, parentpperp;
  double parentE, parentang;
  double dT, dTsm0, dTsm1, dTsm2, dTsm3, dTsm4;

  bwt0 = 0.; bwt1 = 0.; bwt2 = 0.; decay_wt = 0.;

  double nuendx = 0.;
  double nuendy = 0.;
  double nuendz = 0.;

  // TFile* tf = new TFile(ofname, "recreate");
  TTree *outtree = new TTree("dk2nuOutTree", "Reduced_dk2nu_OutTree");

  outtree->Branch("potnum", &mpotnum);
  outtree->Branch("pdg", &pdg);
  outtree->Branch("nutype", &nutype);
  outtree->Branch("nuparent", &nuparent);
  outtree->Branch("nparticles", &nparticles);
  outtree->Branch("nuE", &nuE);
  outtree->Branch("nupx", &nupx);
  outtree->Branch("nupy", &nupy);
  outtree->Branch("nupz", &nupz);
  outtree->Branch("nupperp", &nupperp);
  outtree->Branch("nuang", &nuang);
  outtree->Branch("nustartx", &nustartx);
  outtree->Branch("nustarty", &nustarty);
  outtree->Branch("nustartz", &nustartz);
  outtree->Branch("nustartT", &nustartT);
  outtree->Branch("nuendx", &nuendx);
  outtree->Branch("nuendy", &nuendy);
  outtree->Branch("nuendz", &nuendz);
  outtree->Branch("dT", &dT);
  outtree->Branch("dTs0", &dTsm0);
  outtree->Branch("dTs1", &dTsm1);
  outtree->Branch("dTs2", &dTsm2);
  outtree->Branch("dTs3", &dTsm3);
  outtree->Branch("dTs4", &dTsm4);
  outtree->Branch("parentE", &parentE);
  outtree->Branch("parentpx", &parentpx);
  outtree->Branch("parentpy", &parentpy);
  outtree->Branch("parentpz", &parentpz);
  outtree->Branch("parentpperp", &parentpperp);
  outtree->Branch("parentang", &parentang);

  outtree->Branch("bwt0", &bwt0);
  outtree->Branch("bwt1", &bwt1);
  outtree->Branch("bwt2", &bwt2);
  outtree->Branch("decaywt", &decay_wt);


  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    if (jentry % 250000 == 0) cout << jentry << " out of " << nentries << " " << potnum << " POT " << endl;

    mpotnum = potnum;
    
    // Debugging: Print the current potnum value
    if (jentry % 250000 == 0) cout << jentry << " out of " << nentries << " " << "Current POT: " << potnum << endl;
  
    nparticles = ancestor_;
    nutype = ancestor_pdg[ancestor_ - 1];
    nuparent = ancestor_pdg[ancestor_ - 2];

    nustartx = ancestor_startx[ancestor_ - 1];
    nustarty = ancestor_starty[ancestor_ - 1];
    nustartz = ancestor_startz[ancestor_ - 1];
    nustartT = ancestor_startt[ancestor_ - 1];

    nupx = nuray_px[1];
    nupy = nuray_py[1];
    nupz = nuray_pz[1];
    nupperp = sqrt(nupx * nupx + nupy * nupy);
    nuang = TMath::ATan(nupperp / nupz);

    parentpx = ancestor_startpx[ancestor_ - 2];
    parentpy = ancestor_startpy[ancestor_ - 2];
    parentpz = ancestor_startpz[ancestor_ - 2];
    parentpperp = sqrt(parentpx * parentpx + parentpy * parentpy);
    parentE = decay_ppenergy;
    parentang = TMath::ATan(parentpperp / parentpz);

    nuE = nuray_E[1];
    decay_wt = (double)decay_nimpwt;
    bwt0 = (double)decay_nimpwt * nuray_wgt[0];
    bwt1 = (double)decay_nimpwt * nuray_wgt[1];
    bwt2 = (double)decay_nimpwt * nuray_wgt[2];

    outtree->Fill();
  }

  totalPOT += potnum;

  cout << "writing tree" << endl;

  std::ofstream myoutfile;
 

  outtree->Write();

  cout << "File: " << _fnameid<< " Total POT: " << totalPOT << endl;  
  
    // tf->Close();

}

dk2nuTree::dk2nuTree(int type, TString fnameID, TTree *tree) : fChain(0), totalPOT(0)
{
  TString fname;
  if (type == 0) fname += "/pnfs/dune/persistent/users/ljf26/fluxfiles/g4lbne/v3r5p10/QGSP_BERT/OfficialEngDesignSept2021_forPPFX/antineutrino/flux/g4lbne_v3r5p10_QGSP_BERT_OfficialEngDesignSept2021_forPPFX_antineutrino_";
  if (type == 1) fname += "/pnfs/dune/persistent/users/ljf26/fluxfiles/g4lbne/v3r5p10/QGSP_BERT/OfficialEngDesignSept2021_forPPFX/neutrino/flux/g4lbne_v3r5p10_QGSP_BERT_OfficialEngDesignSept2021_forPPFX_neutrino_";
 
  fname += fnameID;
  fname += ".dk2nu.root";

  _fnameid = fnameID;
 

  TString tofname;
  tofname += "outfile";
  if (type == 0) tofname += "RHC_";
  if (type == 1) tofname += "FHC_";
  tofname += fnameID;
  tofname += ".root";

  ofname = tofname;

  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
    if (!f || !f->IsOpen()) {
      f = new TFile(fname);
    }
    f->GetObject("dk2nuTree", tree);
  }
  Init(tree);
}

dk2nuTree::dk2nuTree(TString filename, TString fnameID) : fChain(0), totalPOT(0)
{
 
  TString tofname;
  
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
    if (!f || !f->IsOpen()) {
        f = new TFile(filename);
    }
    
    TTree *tree = nullptr;
    f->GetObject("dk2nuTree", tree);
    if (tree) {
      //fnameID = fnameID+"_"+index;
	_fnameid = fnameID;
 
        tofname += fnameID;
        tofname += ".root";
	ofname = tofname;
        
        Init(tree);
        void Loop(TFile* tf);
    }
    delete f;
}

dk2nuTree::dk2nuTree(TTree *tree) : fChain(0), totalPOT(0)
{
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("g4lbne_v3r5p4_QGSP_BERT_OptimizedEngineeredNov2017_neutrino_00118.dk2nu.root");
    if (!f || !f->IsOpen()) {
      f = new TFile("g4lbne_v3r5p4_QGSP_BERT_OptimizedEngineeredNov2017_antineutrino_00010.dk2nu.root");
    }
    f->GetObject("dk2nuTree", tree);
  }
  Init(tree);
}

dk2nuTree::~dk2nuTree()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t dk2nuTree::GetEntry(Long64_t entry)
{
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t dk2nuTree::LoadTree(Long64_t entry)
{
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void dk2nuTree::Init(TTree *tree)
{
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("job", &job, &b_dk2nu_job);
  fChain->SetBranchAddress("potnum", &potnum, &b_dk2nu_potnum);
  fChain->SetBranchAddress("decay.norig", &decay_norig, &b_dk2nu_decay_norig);
  fChain->SetBranchAddress("decay.ndecay", &decay_ndecay, &b_dk2nu_decay_ndecay);
  fChain->SetBranchAddress("decay.ntype", &decay_ntype, &b_dk2nu_decay_ntype);
  fChain->SetBranchAddress("decay.vx", &decay_vx, &b_dk2nu_decay_vx);
  fChain->SetBranchAddress("decay.vy", &decay_vy, &b_dk2nu_decay_vy);
  fChain->SetBranchAddress("decay.vz", &decay_vz, &b_dk2nu_decay_vz);
  fChain->SetBranchAddress("decay.pdpx", &decay_pdpx, &b_dk2nu_decay_pdpx);
  fChain->SetBranchAddress("decay.pdpy", &decay_pdpy, &b_dk2nu_decay_pdpy);
  fChain->SetBranchAddress("decay.pdpz", &decay_pdpz, &b_dk2nu_decay_pdpz);
  fChain->SetBranchAddress("decay.ppdxdz", &decay_ppdxdz, &b_dk2nu_decay_ppdxdz);
  fChain->SetBranchAddress("decay.ppdydz", &decay_ppdydz, &b_dk2nu_decay_ppdydz);
  fChain->SetBranchAddress("decay.pppz", &decay_pppz, &b_dk2nu_decay_pppz);
  fChain->SetBranchAddress("decay.ppenergy", &decay_ppenergy, &b_dk2nu_decay_ppenergy);
  fChain->SetBranchAddress("decay.ppmedium", &decay_ppmedium, &b_dk2nu_decay_ppmedium);
  fChain->SetBranchAddress("decay.ptype", &decay_ptype, &b_dk2nu_decay_ptype);
  fChain->SetBranchAddress("decay.muparpx", &decay_muparpx, &b_dk2nu_decay_muparpx);
  fChain->SetBranchAddress("decay.muparpy", &decay_muparpy, &b_dk2nu_decay_muparpy);
  fChain->SetBranchAddress("decay.muparpz", &decay_muparpz, &b_dk2nu_decay_muparpz);
  fChain->SetBranchAddress("decay.mupare", &decay_mupare, &b_dk2nu_decay_mupare);
  fChain->SetBranchAddress("decay.necm", &decay_necm, &b_dk2nu_decay_necm);
  fChain->SetBranchAddress("decay.nimpwt", &decay_nimpwt, &b_dk2nu_decay_nimpwt);
  fChain->SetBranchAddress("nuray", &nuray_, &b_dk2nu_nuray_);
  fChain->SetBranchAddress("nuray.px", nuray_px, &b_nuray_px);
  fChain->SetBranchAddress("nuray.py", nuray_py, &b_nuray_py);
  fChain->SetBranchAddress("nuray.pz", nuray_pz, &b_nuray_pz);
  fChain->SetBranchAddress("nuray.E", nuray_E, &b_nuray_E);
  fChain->SetBranchAddress("nuray.wgt", nuray_wgt, &b_nuray_wgt);
  fChain->SetBranchAddress("ancestor", &ancestor_, &b_dk2nu_ancestor_);
  fChain->SetBranchAddress("ancestor.pdg", ancestor_pdg, &b_ancestor_pdg);
  fChain->SetBranchAddress("ancestor.startx", ancestor_startx, &b_ancestor_startx);
  fChain->SetBranchAddress("ancestor.starty", ancestor_starty, &b_ancestor_starty);
  fChain->SetBranchAddress("ancestor.startz", ancestor_startz, &b_ancestor_startz);
  fChain->SetBranchAddress("ancestor.startt", ancestor_startt, &b_ancestor_startt);
  fChain->SetBranchAddress("ancestor.startpx", ancestor_startpx, &b_ancestor_startpx);
  fChain->SetBranchAddress("ancestor.startpy", ancestor_startpy, &b_ancestor_startpy);
  fChain->SetBranchAddress("ancestor.startpz", ancestor_startpz, &b_ancestor_startpz);
  fChain->SetBranchAddress("ancestor.stoppx", ancestor_stoppx, &b_ancestor_stoppx);
  fChain->SetBranchAddress("ancestor.stoppy", ancestor_stoppy, &b_ancestor_stoppy);
  fChain->SetBranchAddress("ancestor.stoppz", ancestor_stoppz, &b_ancestor_stoppz);
  fChain->SetBranchAddress("ancestor.polx", ancestor_polx, &b_ancestor_polx);
  fChain->SetBranchAddress("ancestor.poly", ancestor_poly, &b_ancestor_poly);
  fChain->SetBranchAddress("ancestor.polz", ancestor_polz, &b_ancestor_polz);
  fChain->SetBranchAddress("ancestor.pprodpx", ancestor_pprodpx, &b_ancestor_pprodpx);
  fChain->SetBranchAddress("ancestor.pprodpy", ancestor_pprodpy, &b_ancestor_pprodpy);
  fChain->SetBranchAddress("ancestor.pprodpz", ancestor_pprodpz, &b_ancestor_pprodpz);
  fChain->SetBranchAddress("ancestor.nucleus", ancestor_nucleus, &b_ancestor_nucleus);
  fChain->SetBranchAddress("ancestor.parIndex", ancestor_parIndex, &b_ancestor_parIndex);
  fChain->SetBranchAddress("ancestor.proc", ancestor_proc, &b_ancestor_proc);
  fChain->SetBranchAddress("ancestor.ivol", ancestor_ivol, &b_ancestor_ivol);
  fChain->SetBranchAddress("ancestor.imat", ancestor_imat, &b_ancestor_imat);
  fChain->SetBranchAddress("ppvx", &ppvx, &b_dk2nu_ppvx);
  fChain->SetBranchAddress("ppvy", &ppvy, &b_dk2nu_ppvy);
  fChain->SetBranchAddress("ppvz", &ppvz, &b_dk2nu_ppvz);
  fChain->SetBranchAddress("tgtexit.tvx", &tgtexit_tvx, &b_dk2nu_tgtexit_tvx);
  fChain->SetBranchAddress("tgtexit.tvy", &tgtexit_tvy, &b_dk2nu_tgtexit_tvy);
  fChain->SetBranchAddress("tgtexit.tvz", &tgtexit_tvz, &b_dk2nu_tgtexit_tvz);
  fChain->SetBranchAddress("tgtexit.tpx", &tgtexit_tpx, &b_dk2nu_tgtexit_tpx);
  fChain->SetBranchAddress("tgtexit.tpy", &tgtexit_tpy, &b_dk2nu_tgtexit_tpy);
  fChain->SetBranchAddress("tgtexit.tpz", &tgtexit_tpz, &b_dk2nu_tgtexit_tpz);
  fChain->SetBranchAddress("tgtexit.tptype", &tgtexit_tptype, &b_dk2nu_tgtexit_tptype);
  fChain->SetBranchAddress("tgtexit.tgen", &tgtexit_tgen, &b_dk2nu_tgtexit_tgen);
  fChain->SetBranchAddress("traj", &traj_, &b_dk2nu_traj_);
  fChain->SetBranchAddress("traj.trkx", traj_trkx, &b_traj_trkx);
  fChain->SetBranchAddress("traj.trky", traj_trky, &b_traj_trky);
  fChain->SetBranchAddress("traj.trkz", traj_trkz, &b_traj_trkz);
  fChain->SetBranchAddress("traj.trkpx", traj_trkpx, &b_traj_trkpx);
  fChain->SetBranchAddress("traj.trkpy", traj_trkpy, &b_traj_trkpy);
  fChain->SetBranchAddress("traj.trkpz", traj_trkpz, &b_traj_trkpz);
  fChain->SetBranchAddress("flagbits", &flagbits, &b_dk2nu_flagbits);
  fChain->SetBranchAddress("vint", &vint, &b_dk2nu_vint);
  fChain->SetBranchAddress("vdbl", &vdbl, &b_dk2nu_vdbl);
  Notify();
}

Bool_t dk2nuTree::Notify()
{
  return kTRUE;
}

void dk2nuTree::Show(Long64_t entry)
{
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t dk2nuTree::Cut(Long64_t entry)
{
  return 1;
}

