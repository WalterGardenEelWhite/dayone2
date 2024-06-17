#ifndef __CINT__
#include <iostream>

#include <vector>
#include <TMath.h>
#include <TVector3.h>
#include <TLorentzVector.h>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TClonesArray.h"

//#include "TArtNEBULAPla.hh"
#endif

//void PlotNEBTree(Int_t run){
void PlotNEBTree_n(){

  TChain* tree = new TChain("tree","ALL tree clone");
  //tree -> Add(Form("./root/NEB/AnaNeb15Bnn%02d.root",run));
  //tree -> Add("./root/NEB/AnaNeb16Bn_nogate.root");
  tree -> Add("./root/NEB/AnaNeb16Bn.root");
  tree -> SetName("tree");

  const Double_t nan = TMath::QuietNaN();
  //TFile *outfile = new TFile(Form("./root/Plot/run%02d_2.root",run),"recreate");
  //TFile *outfile = new TFile("./root/Plot/run16Bn_nogate.root","recreate");
  TFile *outfile = new TFile("./root/Plot/run16Bn.root","recreate");

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



  TH2D * d1 = new TH2D("d1","Scat:Erel same wall",50,0,10,50,0,0.03);
  TH2D * d2 = new TH2D("d2","Scat:Erel diff wall",50,0,10,50,0,0.03);
  TH2D * d3 = new TH2D("d3","Scat:Erel all walls",50,0,10,50,0,0.03);
  TH2D * d4 = new TH2D("d4","Erel_in:Erel_det same wall",50,0,10,50,0,10);
  TH2D * d5 = new TH2D("d5","Erel_in:Erel_det diff wall",50,0,10,50,0,10);

  TH1D * h1 = new TH1D("h1","Scat same wall",50,0,0.03);
  TH1D * h2 = new TH1D("h2","Erel same wall",50,0,10);
  TH1D * h3 = new TH1D("h3","Scat diff wall",50,0,0.03);
  TH1D * h4 = new TH1D("h4","Erel diff wall",50,0,10);
  TH1D * h5 = new TH1D("h5","Scat all wall" ,50,0,0.03);
  TH1D * h6 = new TH1D("h6","Erel all wall" ,50,0,10);
  TH1D * h7 = new TH1D("h7","NEB Multipliticy",5,0.5,5.5);
  TH1D * h8 = new TH1D("h8","NEB Multipliticy nogate",5,0.5,5.5);

  TH2D * drdt = new TH2D("drdt","fNEBdt:fNEBdr",100,0,2000,100,0,10);
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


  //Event Loop=====================================================================================
  Int_t neve = tree -> GetEntries();

  for (Int_t ieve=0;ieve<neve;++ieve){
    tree -> GetEntry(ieve);
    if(ieve%1000==0){
      cout<<"\revent:  "<<ieve <<"/"<<neve;
    }
    h8 -> Fill(fNEBINVmulti);
    //    if(ftriggerbit[1]==1){
    if(fNEBWallbit<3 && fNEBWallbit>0){
      d1 -> Fill(E_rel_15Bn_in,Scat_17B_15Bn_in);//sameeff
      d4 -> Fill(E_rel_15Bn_in,Erel_gen);
      h1 -> Fill(Scat_17B_15Bn_in);
      h2 -> Fill(E_rel_15Bn_in);
    }
    if(fNEBWallbit==3){
      d2 -> Fill(E_rel_15Bn_in,Scat_17B_15Bn_in);//diffeff
      d5 -> Fill(E_rel_15Bn_in,Erel_gen);
      h3 -> Fill(Scat_17B_15Bn_in);
      h4 -> Fill(E_rel_15Bn_in);
    }
    if(fNEBWallbit>0 && fNEBWallbit<4){
      d3 -> Fill(E_rel_15Bn_in,Scat_17B_15Bn_in);//sumeff
      h5 -> Fill(Scat_17B_15Bn_in);
      h6 -> Fill(E_rel_15Bn_in);
      h7 -> Fill(fNEBINVmulti);
    }
    //}
  }
  
  tree->Draw("fNEBdt:fNEBdr>>drdt","","colz");
  tree->Draw("fNEBdtcut:fNEBdrcut>>drdtcut","","colz");
  
  tree->Draw("fW1Q2:fW1beta/fW1dbeta>>samewall","","colz");
  tree->Draw("fW1Q2:fW1betacut/fW1dbetacut>>samewallcut","","colz");
  tree->Draw("fW1Q2:1./fW1dbeta>>samewall2","","colz");
  tree->Draw("fW1Q2:1./fW1dbetacut>>samewall2cut","","colz");
  
  tree->Draw("fW2Q1:fW2beta/fW2dbeta>>diffwall","","colz");
  tree->Draw("fW2Q1:fW2betacut/fW2dbetacut>>diffwallcut","","colz");
  tree->Draw("fW2Q2:1./fW2dbeta>>diffwall2","","colz");
  tree->Draw("fW2Q2:1./fW2dbetacut>>diffwall2cut","","colz");

  tree->Draw("E_rel_15Bn_in:fW1betacut/fW1dbetacut>>sameErel","fNEBWallbit==1||fNEBWallbit==2","colz");
  tree->Draw("E_rel_15Bn_in:fW2betacut/fW2dbetacut>>diffErel","fNEBWallbit==3","colz");

  outfile -> Write();
  outfile -> Close();

}




