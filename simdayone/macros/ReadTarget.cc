#ifndef __CINT__
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TVector3.h>
#include <TCut.h>
#include <TMath.h>
#include <TEntryList.h>

#include <iostream>
#endif

using namespace std;
class TChain;
class TEntryList;

void SetBranchAddress(TChain* tree);

//bring parameter
Int_t    ftriggerbit[4];
Double_t tgtx,tgty,tgta,tgtb;
Double_t z_beam;
Double_t aoz_beam;
Double_t beta_beam;

//Start Function
void ReadTarget(){

  TChain* tree = new TChain("tree","tree");
  tree->Add("$TARTSYS/run/root/All/17BPb.root");
  TFile* file = new TFile("./ReadTarget.root","recreate");
  SetBranchAddress(tree);
  
  //Histogram---------------------------------------------------------------------------------------------

  //PID
  TH1* hPbpid =       new TH2D("hPbpid","upstream particle identification",500,2.8,4,500,3.5,7.5);
  TH1* hPbb17_bg =    new TH2D("hPbb17bg","17B particle identification before gate",500,2.8,4,500,3.5,7.5);
  //Beam Beta
  TH1* hPbbeta = new TH1D("hPbbeta","17B Beam beta",100,0.61,0.65);
  //Target
  TH1* hPbtgtxy   = new TH2D("hPbtgtxy","target x : y",100,-50,50,100,-50,50);
  TH1* hPbtgtab   = new TH2D("hPbtgtab","target a : b",100,-0.05,0.05,100,-0.05,0.05);

  //Tree--------------------------------------------------------------------------------------------------
  tree->Draw(">>elist","","entrylist");
  TEntryList *elist = (TEntryList*)gDirectory->FindObject("elist");
  tree->SetEntryList(elist);
  
  Int_t itree=0;
  Int_t listEntries = elist->GetN();
  for(Long64_t j=0;j<listEntries;j++){
    Long64_t treeEntry = elist->GetEntryAndTree(j,itree);
    Long64_t chainEntry = treeEntry + tree->GetTreeOffset()[itree];
    
    if(j%1000==0){
      cout<<"\revent: "<<j <<"/"<<listEntries;
      cout<<flush;
    }
    tree->GetEntry(chainEntry);

    //BigRIPS Particle Identification-------------------------------------------------------------
    if(ftriggerbit[0]==1){ //DSB trigger
      //All event
      hPbpid->Fill(aoz_beam,z_beam);
      Bool_t B17 = aoz_beam>3.387668&&aoz_beam<3.428252&&z_beam>4.4231&&z_beam<5.72888;
      if(B17)hPbb17_bg->Fill(aoz_beam,z_beam);
	Bool_t intgt = tgtx>-35.&&tgtx<35.&&tgty>-35.&&tgty<35.;
	if(intgt&&B17){
	  hPbbeta->Fill(beta_beam);
	  hPbtgtxy->Fill(tgtx,tgty);
	  hPbtgtab->Fill(tgta,tgtb);
	}

    }//End of DSB trigger

  }//tree fill
  file->Write();
}//function

void SetBranchAddress(TChain* tree){
  //BigRIPS
  tree->SetBranchAddress("ftriggerbit[4]",&ftriggerbit);
  tree->SetBranchAddress("aoz_beam",&aoz_beam);
  tree->SetBranchAddress("z_beam",&z_beam);
  tree->SetBranchAddress("tgtx",&tgtx);
  tree->SetBranchAddress("tgty",&tgty);
  tree->SetBranchAddress("tgta",&tgta);
  tree->SetBranchAddress("tgtb",&tgtb);
  tree->SetBranchAddress("beta_beam",&beta_beam);

}
