#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TMath.h"

#include "TFragSimData.hh"
#include "TRunSimParameter.hh"
#include "TFragSimParameter.hh"

void calc_fdc2pos_func(TString filename="root/brho_func/func_29f_0000.root")
{
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
  Double_t brho;
  Double_t tgtx,tgty,tgta,tgtb;
  tree->SetBranchAddress("fragment",&gFragSimDataArray);  
  tree->AddFriend("tree_input","root/brho_func/input_A29Z9.root");
  tree->SetBranchAddress("brho",&brho);  
  tree->SetBranchAddress("tgtx",&tgtx);
  tree->SetBranchAddress("tgty",&tgty);
  tree->SetBranchAddress("tgta",&tgta);
  tree->SetBranchAddress("tgtb",&tgtb);

  TString ofilename = filename;
  ofilename.ReplaceAll("func_","analyzed_");
  TFile *ofile = new TFile(ofilename.Data(),"recreate");
  ofile->cd();
  TTree *otree = new TTree("tree","analyzed tree");
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Double_t tof,fl;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  otree->Branch("tgtx",&tgtx,"tgtx/D");
  otree->Branch("tgty",&tgty,"tgty/D");
  otree->Branch("tgta",&tgta,"tgta/D");
  otree->Branch("tgtb",&tgtb,"tgtb/D");
  
  otree->Branch("OK_FDC1",&OK_FDC1,"OK_FDC1/O");
  otree->Branch("fdc1x",&fdc1x,"fdc1x/D");
  otree->Branch("fdc1a",&fdc1a,"fdc1a/D");
  otree->Branch("fdc1y",&fdc1y,"fdc1y/D");
  otree->Branch("fdc1b",&fdc1b,"fdc1b/D");
  otree->Branch("OK_FDC2",&OK_FDC2,"OK_FDC2/O");
  otree->Branch("fdc2x",&fdc2x,"fdc2x/D");
  otree->Branch("fdc2a",&fdc2a,"fdc2a/D");
  otree->Branch("fdc2y",&fdc2y,"fdc2y/D");
  otree->Branch("fdc2b",&fdc2b,"fdc2b/D");
  otree->Branch("brho",&brho,"brho/D");
  otree->Branch("OK_HODF",&OK_HODF,"OK_HODF/O");
  otree->Branch("tof",&tof,"tof/D");
  otree->Branch("fl",&fl,"fl/D");

  Int_t neve = tree->GetEntries();
  for (Int_t ieve=0;ieve<neve;++ieve){
    //for (Int_t ieve=0;ieve<10;++ieve){
    gFragSimDataArray->clear();
    tree->GetEntry(ieve);

    OK_FDC1=false;
    OK_FDC2=false;
    OK_HODF=false;

    // Get fragment data
    Int_t nfrag = gFragSimDataArray->size();
    for (Int_t ifrag=0;ifrag<nfrag;++ifrag){
      TFragSimData frag = (*gFragSimDataArray)[ifrag];
      //-----------------------------------------
      if (frag.fDetectorName == "FDC1") {
	OK_FDC1 = true;
	TVector3 pos_pre = frag.fPrePosition;
	TVector3 pos_post = frag.fPostPosition;
	TVector3 pos = 0.5*(pos_pre + pos_post);

	fdc1x = pos.x();
	fdc1y = pos.y();
	fdc1a = (pos_post.x()-pos_pre.x())/(pos_post.z()-pos_pre.z())*1000.;
	fdc1b = (pos_post.y()-pos_pre.y())/(pos_post.z()-pos_pre.z())*1000.;
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

	fdc2x = pos.x();
	fdc2y = pos.y();
	fdc2a = (pos_post_cpy.x()-pos_pre_cpy.x())/(pos_post_cpy.z()-pos_pre_cpy.z())*1000.;
	fdc2b = (pos_post_cpy.y()-pos_pre_cpy.y())/(pos_post_cpy.z()-pos_pre_cpy.z())*1000.;
      //-----------------------------------------
      }else if (frag.fDetectorName == "HOD") {
	OK_HODF = true;
	tof = frag.fPreTime;
	fl  = frag.fFlightLength; 
	//cout<<"fl="<<fl<<endl;
      }
      //-----------------------------------------
    }//for (Int_t ifrag=0;ifrag<nfrag;++ifrag){
    otree->Fill();

  }
  ofile->Write();
}
