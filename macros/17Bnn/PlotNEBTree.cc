#ifndef __CINT__
#include <iostream>

#include <vector>
#include <TMath.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TString.h>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TClonesArray.h"

//#include "TArtNEBULAPla.hh"
#endif

Double_t Erel_Max = 7;//[MeV]
Double_t Erel_Max_2 = 2.;//[MeV]
Double_t Erel_Max_n = 5.;//[MeV]
Double_t Scat_Max = 0.03;//[rad]

Double_t Ebin = Erel_Max/0.2;//[mrad/200keV]
Double_t Ebin_2 = Erel_Max_2/0.02;//[mrad/20keV]
Double_t Ebin_n = Erel_Max_n/0.01;//[mrad/10keV]
Double_t Sbin = Scat_Max/0.003;//[mrad/3mrad]

void PlotNEBTree(Int_t irun, TString tag){
//void PlotNEBTree(){

  TChain* tree = new TChain("tree","ALL tree clone");
  tree -> Add(Form("./root/NEB/%s/AnaNeb17Bnn%02d_%s.root",tag.Data(),irun,tag.Data()));
  //tree -> Add("./root/NEB/AnaNeb17Bnn.root");
  tree -> SetName("Neutron analysis");
  TChain* tree2 = new TChain("tree","ALL tree clone");
  tree2 -> Add(Form("./root/Gen/%s/17BnnInpParam%02d_%s.root",tag.Data(),irun,tag.Data()));
  tree2 -> SetName("Input Parameter");

  const Double_t nan = TMath::QuietNaN();
  TFile *outfile = new TFile(Form("./root/Plot/%s/run17Bnn%02d_%s.root",tag.Data(),irun,tag.Data()),"recreate");
  //TFile *outfile = new TFile(Form("./root/Plot/run17Bnn.root",irun),"recreate");

  Double_t scat_gen_cm[3];
  Double_t Erel_in;
  Double_t Erel_gen;
  Int_t fNEBWallbit;
  Int_t fNEBINVmulti;
  Double_t Mass_inv_15Bnn;
  Double_t Mass_inv_15Bn;
  Double_t E_rel_15Bnn;
  Double_t E_rel_15Bn;
  Double_t E_rel_15Bnn_in;
  Double_t E_rel_15Bn_in;
  Double_t Scat_17B_15Bnn;
  Double_t Scat_17B_15Bn;
  Double_t Scat_17B_15Bnn_in;
  Double_t Scat_17B_15Bn_in;
  Double_t Scat_17B_15Bnn_cm_c;
  Double_t Scat_17B_15Bnn_cm_emp;
  Double_t Scat_17B_15Bnn_cm_pb;
  Double_t Scat_17B_15Bn_cm_c;
  Double_t Scat_17B_15Bn_cm_emp;
  Double_t Scat_17B_15Bn_cm_pb;

  Double_t fNEBdr[50];
  Double_t fNEBdrcut[50];
  Double_t fNEBdt[50];
  Double_t fNEBdtcut[50];

  Double_t fW1beta[50];
  Double_t fW1dbeta[50];
  Double_t fW1betacut[50];
  Double_t fW1dbetacut[50];
  Double_t fW1Q1[50];
  Double_t fW1Q2[50];

  Double_t fW2beta[50];
  Double_t fW2dbeta[50];
  Double_t fW2betacut[50];
  Double_t fW2dbetacut[50];
  Double_t fW2Q1[50];
  Double_t fW2Q2[50];

  tree2->SetBranchAddress("scat_gen_cm[3]", scat_gen_cm);
  tree2->SetBranchAddress("Erel_in", &Erel_in);

  tree->SetBranchAddress("Erel_gen", &Erel_gen);
  tree->SetBranchAddress("fNEBWallbit", &fNEBWallbit);
  tree->SetBranchAddress("fNEBINVmulti", &fNEBINVmulti);
  tree->SetBranchAddress("Mass_inv_15Bnn", &Mass_inv_15Bnn);
  tree->SetBranchAddress("Mass_inv_15Bn", &Mass_inv_15Bn);
  tree->SetBranchAddress("E_rel_15Bnn", &E_rel_15Bnn);
  tree->SetBranchAddress("E_rel_15Bn", &E_rel_15Bn);
  tree->SetBranchAddress("E_rel_15Bnn_in", &E_rel_15Bnn_in);
  tree->SetBranchAddress("E_rel_15Bn_in", &E_rel_15Bn_in);
  tree->SetBranchAddress("Scat_17B_15Bnn", &Scat_17B_15Bnn);
  tree->SetBranchAddress("Scat_17B_15Bn", &Scat_17B_15Bn);
  tree->SetBranchAddress("Scat_17B_15Bnn_in", &Scat_17B_15Bnn_in);
  tree->SetBranchAddress("Scat_17B_15Bn_in", &Scat_17B_15Bn_in);
  tree->SetBranchAddress("Scat_17B_15Bnn_cm_c", &Scat_17B_15Bnn_cm_c);
  tree->SetBranchAddress("Scat_17B_15Bnn_cm_emp", &Scat_17B_15Bnn_cm_emp);
  tree->SetBranchAddress("Scat_17B_15Bnn_cm_pb", &Scat_17B_15Bnn_cm_pb);
  tree->SetBranchAddress("Scat_17B_15Bn_cm_c", &Scat_17B_15Bn_cm_c);
  tree->SetBranchAddress("Scat_17B_15Bn_cm_emp", &Scat_17B_15Bn_cm_emp);
  tree->SetBranchAddress("Scat_17B_15Bn_cm_pb", &Scat_17B_15Bn_cm_pb);
  
  tree->SetBranchAddress("fNEBdr[50]", fNEBdr);
  tree->SetBranchAddress("fNEBdrcut[50]", fNEBdrcut);
  tree->SetBranchAddress("fNEBdt[50]", fNEBdt);
  tree->SetBranchAddress("fNEBdtcut[50]", fNEBdtcut);
  
  tree->SetBranchAddress("fW1beta[50]", fW1beta);
  tree->SetBranchAddress("fW1dbeta[50]", fW1dbeta);
  tree->SetBranchAddress("fW1betacut[50]", fW1betacut);
  tree->SetBranchAddress("fW1dbetacut[50]", fW1dbetacut);
  tree->SetBranchAddress("fW1Q1[50]", fW1Q1);
  tree->SetBranchAddress("fW1Q2[50]", fW1Q2);

  tree->SetBranchAddress("fW2beta[50]", fW2beta);
  tree->SetBranchAddress("fW2dbeta[50]", fW2dbeta);
  tree->SetBranchAddress("fW2betacut[50]", fW2betacut);
  tree->SetBranchAddress("fW2dbetacut[50]", fW2dbetacut);
  tree->SetBranchAddress("fW2Q1[50]", fW2Q1);
  tree->SetBranchAddress("fW2Q2[50]", fW2Q2);

  TH2D * a0 = new TH2D("a0","Erel:Scat_gen_cm[0]",Ebin,0,Erel_Max,Sbin+10.,0,0.06);
  TH2D * a1 = new TH2D("a1","Erel:Scat_gen_cm[1]",Ebin,0,Erel_Max,Sbin,0,Scat_Max);
  TH2D * a2 = new TH2D("a2","Erel:Scat_gen_cm[2]",Ebin,0,Erel_Max,Sbin+1.,0,Scat_Max+0.003);


  TH2D * c1 = new TH2D("c1","fNEBdtcut:fNEBdrcut Multi>0",100,0,2000,100,0,20);
  TH2D * c2 = new TH2D("c2","fNEBdtcut:fNEBdrcut Multi>1",100,0,2000,100,0,20);
  //TH2D * c3 = new TH2D("c3","fW1Q2:fW1betacut/fW1dbetacut Multi>0",100,-10,10,100,0,200);
  //TH2D * c4 = new TH2D("c4","fW1Q2:fW1betacut/fW1dbetacut Multi>1",100,-10,10,100,0,200);
  //TH2D * c5 = new TH2D("c5","fW2Q1:fW2betacut/fW2dbetacut Multi>0",100,-10,10,100,0,200);
  //TH2D * c6 = new TH2D("c6","fW2Q1:fW2betacut/fW2dbetacut Multi>1",100,-10,10,100,0,200);

  TH1D * h0 = new TH1D("h0","Erel 1n",Ebin_n,0,Erel_Max_n);

  TH2D * d1 = new TH2D("d1","Scat:Erel same wall",Ebin,0,Erel_Max,Sbin,0,Scat_Max);
  TH2D * d2 = new TH2D("d2","Scat:Erel diff wall",Ebin,0,Erel_Max,Sbin,0,Scat_Max);
  TH2D * d3 = new TH2D("d3","Scat:Erel all walls",Ebin,0,Erel_Max,Sbin,0,Scat_Max);
  TH2D * d4 = new TH2D("d4","Scat_CM:Erel same wall C target",Ebin,0,Erel_Max,Sbin+10.,0,0.06);
  TH2D * d5 = new TH2D("d5","Scat_CM:Erel diff wall C target",Ebin,0,Erel_Max,Sbin+10.,0,0.06);
  TH2D * d6 = new TH2D("d6","Scat_CM:Erel all walls C target",Ebin,0,Erel_Max,Sbin+10.,0,0.06);
  TH2D * d7 = new TH2D("d7","Scat_CM:Erel same wall Emp target",Ebin,0,Erel_Max,Sbin,0,Scat_Max);
  TH2D * d8 = new TH2D("d8","Scat_CM:Erel diff wall Emp target",Ebin,0,Erel_Max,Sbin,0,Scat_Max);
  TH2D * d9 = new TH2D("d9","Scat_CM:Erel all walls Emp target",Ebin,0,Erel_Max,Sbin,0,Scat_Max);
  TH2D * d10 = new TH2D("d10","Scat_CM:Erel same wall Pb target",Ebin,0,Erel_Max,Sbin+1.,0,Scat_Max+0.003);
  TH2D * d11 = new TH2D("d11","Scat_CM:Erel diff wall Pb target",Ebin,0,Erel_Max,Sbin+1.,0,Scat_Max+0.003);
  TH2D * d12 = new TH2D("d12","Scat_CM:Erel all walls Pb target",Ebin,0,Erel_Max,Sbin+1.,0,Scat_Max+0.003);
  TH2D * d13 = new TH2D("d13","Scat:Erel (<2MeV) same wall",Ebin_2,0,Erel_Max_2,Sbin,0,Scat_Max);
  TH2D * d14 = new TH2D("d14","Scat:Erel (<2MeV) diff wall",Ebin_2,0,Erel_Max_2,Sbin,0,Scat_Max);
  TH2D * d15 = new TH2D("d15","Scat:Erel (<2MeV) all walls",Ebin_2,0,Erel_Max_2,Sbin,0,Scat_Max);
  
  TH2D * e1 = new TH2D("e1","Erel_in:Erel_det same wall",Ebin,0,Erel_Max,Ebin,0,Erel_Max);
  TH2D * e2 = new TH2D("e2","Erel_in:Erel_det diff wall",Ebin,0,Erel_Max,Ebin,0,Erel_Max);

  //Crosstalk check
  TH2D * drdt = new TH2D("drdt","fNEBdt:fNEBdr",100,0,1000,100,0,10);
  TH2D * drdtcut = new TH2D("drdtcut","fNEBdtcut:fNEBdrcut",100,0,2000,100,0,10);
  TH2D * samewall     = new TH2D("samewall","fW1Q2:fW1beta/fW1dbeta",200,-10,10,200,0,200);
  TH2D * samewallcut  = new TH2D("samewallcut","fW1Q2:fW1betacut/fW1dbetacut",200,-10,10,200,0,200);
  TH2D * samewall2    = new TH2D("samewall2","fW1Q2:1./fW1dbeta",200,-2,2,200,0,200);
  TH2D * samewall2cut = new TH2D("samewall2cut","fW1Q2:1./fW1dbetacut",200,-2,2,200,0,200);

  TH2D * diffwall     = new TH2D("diffwall","fW2Q1:fW2beta/fW2dbeta",200,-5,5,200,0,200);
  TH2D * diffwallcut  = new TH2D("diffwallcut","fW2Q1:fW2betacut/fW2dbetacut",200,-5,5,200,0,200);
  TH2D * diffwall2    = new TH2D("diffwall2","fW2Q2:1./fW2dbeta",200,-2,2,200,0,200);
  TH2D * diffwall2cut = new TH2D("diffwall2cut","fW2Q2:1./fW2dbetacut",200,-2,2,200,0,200);

  TH2D * sameErel = new TH2D("sameErel","Erel:fW1beta/fW1dbeta",200,-5,5,200,0,10);
  TH2D * diffErel = new TH2D("diffErel","Erel:fW2beta/fW2dbeta",200,-5,5,200,0,10);

  TH1D * n0 = new TH1D("n0","NEB Multipliticy nogate",5,0.5,5.5);
  TH1D * n1 = new TH1D("n1","NEB Multipliticy samewall",5,0.5,5.5);
  TH1D * n2 = new TH1D("n2","NEB Multipliticy diffwall",5,0.5,5.5);
  TH1D * n3 = new TH1D("n3","NEB Multipliticy all wall",5,0.5,5.5);

  //Event Loop=====================================================================================

  Int_t neve2 = tree2 -> GetEntries();
  for (Int_t ieve=0;ieve<neve2;++ieve){
    tree2 -> GetEntry(ieve);
    if(ieve%1000==0){
      cout<<"\revent:  "<<ieve <<"/"<<neve2;
    }
    a0 ->Fill(Erel_in,scat_gen_cm[0]);
    a1 ->Fill(Erel_in,scat_gen_cm[1]);
    a2 ->Fill(Erel_in,scat_gen_cm[2]);
  }

  Int_t neve = tree -> GetEntries();

  for (Int_t ieve=0;ieve<neve;++ieve){
    tree -> GetEntry(ieve);
    if(ieve%1000==0){
      cout<<"\revent:  "<<ieve <<"/"<<neve;
    }

    h0 ->Fill(E_rel_15Bn_in);
    n0-> Fill(fNEBINVmulti);

    //Same Wall
    if(fNEBWallbit<3 && fNEBWallbit>0){
      d1  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_in);
      d4  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bn_cm_c);
      d7  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_cm_emp);
      d10 -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_cm_pb);
      d13 -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_in);

      e1  -> Fill(E_rel_15Bnn_in,Erel_gen);
      n1  -> Fill(fNEBINVmulti);
    }
    //Diff Wall
    if(fNEBWallbit==3){
      d2  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_in);
      d5  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bn_cm_c);
      d8  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_cm_emp);
      d11 -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_cm_pb);
      d14 -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_in);

      e2 -> Fill(E_rel_15Bnn_in,Erel_gen);
      n2 -> Fill(fNEBINVmulti);
    }
    //All Wall
    if(fNEBWallbit>0 && fNEBWallbit<4){
      d3  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_in);
      d6  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bn_cm_c);
      d9  -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_cm_emp);
      d12 -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_cm_pb);
      d15 -> Fill(E_rel_15Bnn_in,Scat_17B_15Bnn_in);

      n3 -> Fill(fNEBINVmulti);
    }
  }
  
  tree->Draw("fNEBdt:fNEBdr>>drdt","fNEBdt>0&&fNEBdr>0","colz");
  tree->Draw("fNEBdtcut:fNEBdrcut>>drdtcut","fNEBdtcut>0&&fNEBdrcut>0","colz");
  tree->Draw("fNEBdtcut:fNEBdrcut>>c1","fNEBdtcut>0&&fNEBdrcut>0&&fNEBINVmulti>0","colz");
  tree->Draw("fNEBdtcut:fNEBdrcut>>c2","fNEBdtcut>0&&fNEBdrcut>0&&fNEBINVmulti>1","colz");

  
  tree->Draw("fW1Q2:fW1beta/fW1dbeta>>samewall","","colz");
  tree->Draw("fW1Q2:fW1betacut/fW1dbetacut>>samewallcut","","colz");
  //tree->Draw("fW1Q2:fW1betacut/fW1dbetacut>>c3","fW1betacut>0&&fW1dbetacut>0&&fNEBINVmulti>0","colz");
  //tree->Draw("fW1Q2:fW1betacut/fW1dbetacut>>c4","fW1betacut>0&&fW1dbetacut>0&&fNEBINVmulti>1","colz");
  tree->Draw("fW1Q2:1./fW1dbeta>>samewall2","","colz");
  tree->Draw("fW1Q2:1./fW1dbetacut>>samewall2cut","","colz");

  
  tree->Draw("fW2Q1:fW2beta/fW2dbeta>>diffwall","","colz");
  tree->Draw("fW2Q1:fW2betacut/fW2dbetacut>>diffwallcut","","colz");
  //tree->Draw("fW2Q1:fW2betacut/fW2dbetacut>>c5","fW2betacut>0&&fW2dbetacut>0&&fNEBINVmulti>0","colz");
  //tree->Draw("fW2Q1:fW2betacut/fW2dbetacut>>c6","fW2betacut>0&&fW2dbetacut>0&&fNEBINVmulti>1","colz");
  tree->Draw("fW2Q2:1./fW2dbeta>>diffwall2","","colz");
  tree->Draw("fW2Q2:1./fW2dbetacut>>diffwall2cut","","colz");

  tree->Draw("E_rel_15Bnn_in:fW1betacut/fW1dbetacut>>sameErel","fNEBWallbit==1||fNEBWallbit==2","colz");
  tree->Draw("E_rel_15Bnn_in:fW2betacut/fW2dbetacut>>diffErel","fNEBWallbit==3","colz");

  outfile -> Write();
  outfile -> Close();

}
