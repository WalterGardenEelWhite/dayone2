#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TMath.h"

#include "TFragSimData.hh"
#include "TRunSimParameter.hh"
#include "TFragSimParameter.hh"

void calc_fdc2pos(Int_t nRun=466)
{
  TString filename = Form("root/brho_func/run%04d_0000.root",nRun);

  TFile *file = new TFile(filename.Data());

//  TRunSimParameter *RunPrm = 0;
//  file->GetObject("RunParameter",RunPrm);
//  RunPrm->Print();

  TFragSimParameter *FragPrm = 0;
  file->GetObject("FragParameter",FragPrm);
//  FragPrm->Print();

  Double_t FDC2Angle = FragPrm->fFDC2Angle;
  TVector3 posFDC2  = FragPrm->fFDC2Position;
//  cout<<"FDC2pos"<<endl;
//  posFDC2.Print();
//  cout<<"FDC2ang="<<FDC2Angle<<endl;

  Double_t HODAngle = FragPrm->fHODAngle;
  TVector3 posHOD  = FragPrm->fHODPosition;
//  cout<<"HODpos"<<endl;
//  posHOD.Print();
//  cout<<"HODang="<<HODAngle<<endl;

  TTree *tree = (TTree*)file->Get("tree");
  tree->SetBranchAddress("fragment",&gFragSimDataArray);  

  TFile *ofile = new TFile(Form("root/brho_func/analyzed_%04d.root",nRun),"recreate");
  ofile->cd();
  TTree *otree = new TTree("tree","analyzed tree");
  Double_t fdc1xsim,fdc1ysim,fdc1asim,fdc1bsim;
  Double_t fdc2xsim,fdc2ysim,fdc2asim,fdc2bsim;
  otree->Branch("fdc1xsim",&fdc1xsim,"fdc1xsim/D");
  otree->Branch("fdc1asim",&fdc1asim,"fdc1asim/D");
  otree->Branch("fdc1ysim",&fdc1ysim,"fdc1ysim/D");
  otree->Branch("fdc1bsim",&fdc1bsim,"fdc1bsim/D");
  otree->Branch("fdc2xsim",&fdc2xsim,"fdc2xsim/D");
  otree->Branch("fdc2asim",&fdc2asim,"fdc2asim/D");
  otree->Branch("fdc2ysim",&fdc2ysim,"fdc2ysim/D");
  otree->Branch("fdc2bsim",&fdc2bsim,"fdc2bsim/D");

  Int_t neve = tree->GetEntries();
  for (Int_t ieve=0;ieve<neve;++ieve){
    //for (Int_t ieve=0;ieve<10;++ieve){
    gFragSimDataArray->clear();
    tree->GetEntry(ieve);

    // Get fragment data
    Bool_t OK_FDC2    = false;
    Bool_t OK_HODF    = false;
    Int_t nfrag = gFragSimDataArray->size();
    for (Int_t ifrag=0;ifrag<nfrag;++ifrag){
      TFragSimData frag = (*gFragSimDataArray)[ifrag];
      //-----------------------------------------
      if (frag.fDetectorName == "FDC1") {
	OK_FDC2 = true;
	TVector3 pos_pre = frag.fPrePosition;
	TVector3 pos_post = frag.fPostPosition;
	TVector3 pos = 0.5*(pos_pre + pos_post);

	fdc1xsim = pos.x();
	fdc1ysim = pos.y();
	fdc1asim = (pos_post.x()-pos_pre.x())/(pos_post.z()-pos_pre.z())*1000.;
	fdc1bsim = (pos_post.y()-pos_pre.y())/(pos_post.z()-pos_pre.z())*1000.;
      //-----------------------------------------
      }else if (frag.fDetectorName == "FDC2") {
	OK_FDC2 = true;
	TVector3 pos_pre = frag.fPrePosition;
	TVector3 pos_post = frag.fPostPosition;
	TVector3 pos_pre_cpy = pos_pre-posFDC2;
	TVector3 pos_post_cpy = pos_post-posFDC2;
	pos_pre_cpy.RotateY(-FDC2Angle/180.*TMath::Pi());
	pos_post_cpy.RotateY(-FDC2Angle/180.*TMath::Pi());
	TVector3 pos = 0.5*(pos_pre_cpy + pos_post_cpy);

	fdc2xsim = pos.x();
	fdc2ysim = pos.y();
	fdc2asim = (pos_post_cpy.x()-pos_pre_cpy.x())/(pos_post_cpy.z()-pos_pre_cpy.z())*1000.;
	fdc2bsim = (pos_post_cpy.y()-pos_pre_cpy.y())/(pos_post_cpy.z()-pos_pre_cpy.z())*1000.;
      }
      //-----------------------------------------
    }//for (Int_t ifrag=0;ifrag<nfrag;++ifrag){
    otree->Fill();

  }
  ofile->Write();
}
