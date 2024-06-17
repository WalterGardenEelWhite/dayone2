#ifndef __CINT__OA
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

void AnaNEBTree(Int_t irun, TString tag){
//void AnaNEBTree(){

  TChain* oritree = new TChain("tree","ALL tree clone");
  oritree -> Add(Form("./root/Ana/%s/AnaSim17Bnn%02d_%s.root",tag.Data(),irun,tag.Data()));
  //oritree -> Add("./root/Ana/holysample.root");
  oritree -> SetName("oritree");

  const Double_t nan = TMath::QuietNaN();
  Double_t        tgtx;
  Double_t        tgty;
  Double_t        tgtz;
  Double_t        scat_ang;
  Double_t        scat_gen_cm[3];
  Double_t        Erel_in;
  Bool_t          ok_fdc1;
  Bool_t          ok_window;
  Bool_t          ok_fdc2;
  Bool_t          ok_hod;
  Double_t        fNEBQAve[144];
  Double_t        fNEBTOF[144];
  Double_t        fNEBLgt[144];
  Double_t        fNEBBeta[144];
  Int_t           fNEBID[144];
  Int_t           fNEBLayer[144];
  Double_t        fNEBX[144];
  Double_t        fNEBY[144];
  Double_t        fNEBZ[144];
  Int_t           fNEBV1Multi;
  Int_t           fNEBV2Multi;
  //Double_t        Vb_px,Vb_py,Vb_pz,Vb_E,Vf_px,Vf_py,Vf_pz,Vf_E;

  oritree->SetBranchAddress("tgtx", &tgtx);
  oritree->SetBranchAddress("tgty", &tgty);
  oritree->SetBranchAddress("tgtz", &tgtz);
  oritree->SetBranchAddress("scat_ang", &scat_ang);
  oritree->SetBranchAddress("Erel_in", &Erel_in);
  oritree->SetBranchAddress("scat_gen_cm[3]",scat_gen_cm);
  //oritree->SetBranchAddress("Vb_px", &Vb_px);
  //oritree->SetBranchAddress("Vb_py", &Vb_py);
  //oritree->SetBranchAddress("Vb_pz", &Vb_pz);
  //oritree->SetBranchAddress("Vb_E", &Vb_E);
  //oritree->SetBranchAddress("Vf_px", &Vf_px);
  //oritree->SetBranchAddress("Vf_py", &Vf_py);
  //oritree->SetBranchAddress("Vf_pz", &Vf_pz);
  //oritree->SetBranchAddress("Vf_E", &Vf_E);

  oritree->SetBranchAddress("ok_fdc1", &ok_fdc1);
  oritree->SetBranchAddress("ok_window", &ok_window);
  oritree->SetBranchAddress("ok_fdc2", &ok_fdc2);
  oritree->SetBranchAddress("ok_hod", &ok_hod);
  oritree->SetBranchAddress("fNEBQAve[144]", fNEBQAve);
  oritree->SetBranchAddress("fNEBTOF[144]", fNEBTOF);
  oritree->SetBranchAddress("fNEBLgt[144]", fNEBLgt);
  oritree->SetBranchAddress("fNEBBeta[144]", fNEBBeta);
  oritree->SetBranchAddress("fNEBID[144]", fNEBID);
  oritree->SetBranchAddress("fNEBLayer[144]", fNEBLayer);
  oritree->SetBranchAddress("fNEBX[144]", fNEBX);
  oritree->SetBranchAddress("fNEBY[144]", fNEBY);
  oritree->SetBranchAddress("fNEBZ[144]", fNEBZ);
  oritree->SetBranchAddress("fNEBV1Multi", &fNEBV1Multi);
  oritree->SetBranchAddress("fNEBV2Multi", &fNEBV2Multi);

  Double_t Bgen_px = nan; Double_t Bgen_py = nan; Double_t Bgen_pz = nan; Double_t Bgen_E = nan;
  Double_t Fgen_px = nan; Double_t Fgen_py = nan; Double_t Fgen_pz = nan; Double_t Fgen_E = nan;
  Double_t N1gen_px = nan; Double_t N1gen_py = nan; Double_t N1gen_pz = nan; Double_t N1gen_E = nan;
  Double_t N2gen_px = nan; Double_t N2gen_py = nan; Double_t N2gen_pz = nan; Double_t N2gen_E = nan;
  Double_t Erel_gen = nan;

  oritree->SetBranchAddress("Bgen_px", &Bgen_px);
  oritree->SetBranchAddress("Bgen_py", &Bgen_py);
  oritree->SetBranchAddress("Bgen_pz", &Bgen_pz);
  oritree->SetBranchAddress("Bgen_E", &Bgen_E);
  oritree->SetBranchAddress("Fgen_px", &Fgen_px);
  oritree->SetBranchAddress("Fgen_py", &Fgen_py);
  oritree->SetBranchAddress("Fgen_pz", &Fgen_pz);
  oritree->SetBranchAddress("Fgen_E", &Fgen_E);
  oritree->SetBranchAddress("N1gen_px", &N1gen_px);
  oritree->SetBranchAddress("N1gen_py", &N1gen_py);
  oritree->SetBranchAddress("N1gen_pz", &N1gen_pz);
  oritree->SetBranchAddress("N1gen_E", &N1gen_E);
  oritree->SetBranchAddress("N2gen_px", &N2gen_px);
  oritree->SetBranchAddress("N2gen_py", &N2gen_py);
  oritree->SetBranchAddress("N2gen_pz", &N2gen_pz);
  oritree->SetBranchAddress("N2gen_E", &N2gen_E);
  oritree->SetBranchAddress("Erel_gen", &Erel_gen);

  //OUTPUT TTree file-----------------------------------------------------------------------------------------------
  TFile *outfile = new TFile(Form("./root/NEB/%s/AnaNeb17Bnn%02d_%s.root",tag.Data(),irun,tag.Data()),"recreate");
  //TFile *outfile = new TFile("./root/NEB/AnaNeb17Bnn.root","recreate");
  TTree *tree = new TTree("tree","NEB analized tree");

  tree->Branch("tgtx",&tgtx,"tgtx/D");
  tree->Branch("tgty",&tgty,"tgty/D");
  tree->Branch("tgtz",&tgtz,"tgtz/D");
  tree->Branch("scat_ang",&scat_ang,"scat_ang/D");
  tree->Branch("Erel_in",&Erel_in,"Erel_in/D");
  tree->Branch("scat_gen_cm[3]",&scat_gen_cm,"scat_gen_cm[3]/D");
  //tree->Branch("Vb_px",&Vb_px,"Vb_px/D");
  //tree->Branch("Vb_py",&Vb_py,"Vb_py/D");
  //tree->Branch("Vb_pz",&Vb_pz,"Vb_pz/D");
  //tree->Branch("Vb_E",&Vb_E,"Vb_E/D");
  //tree->Branch("Vf_px",&Vf_px,"Vf_px/D");
  //tree->Branch("Vf_py",&Vf_py,"Vf_py/D");
  //tree->Branch("Vf_pz",&Vf_pz,"Vf_pz/D");
  //tree->Branch("Vf_E", &Vf_E,"Vf_E/D");
  tree->Branch("ok_fdc1",&ok_fdc1,"ok_fdc1/O");
  tree->Branch("ok_window",&ok_window,"ok_window/O");
  tree->Branch("ok_fdc2",&ok_fdc2,"ok_fdc2/O");
  tree->Branch("ok_hod",&ok_hod,"ok_hod/O");
  tree->Branch("fNEBQAve[144]", &fNEBQAve,"fNEBQAve[144]/D");
  tree->Branch("fNEBTOF[144]", &fNEBTOF,"fNEBTOF[144]/D");
  tree->Branch("fNEBLgt[144]", &fNEBLgt,"fNEBLgt[144]/D");
  tree->Branch("fNEBBeta[144]", &fNEBBeta,"fNEBBeta[144]/D");
  tree->Branch("fNEBID[144]", &fNEBID,"fNEBID[144]/I");
  tree->Branch("fNEBLayer[144]", &fNEBLayer,"fNEBLayer[144]/I");
  tree->Branch("fNEBX[144]", &fNEBX,"fNEBX[144]/D");
  tree->Branch("fNEBY[144]", &fNEBY,"fNEBY[144]/D");
  tree->Branch("fNEBZ[144]", &fNEBZ,"fNEBZ[144]/D");
  tree->Branch("fNEBV1Multi", &fNEBV1Multi,"fNEBV1Multi/I");
  tree->Branch("fNEBV2Multi", &fNEBV2Multi,"fNEBV2Multi/I");
  tree->Branch("Bgen_px", &Bgen_px, "Bgen_px/D");
  tree->Branch("Bgen_py", &Bgen_py, "Bgen_py/D");
  tree->Branch("Bgen_pz", &Bgen_pz, "Bgen_pz/D");
  tree->Branch("Bgen_E", &Bgen_E, "Bgen_E/D");
  tree->Branch("Fgen_px", &Fgen_px, "Fgen_px/D");
  tree->Branch("Fgen_py", &Fgen_py, "Fgen_py/D");
  tree->Branch("Fgen_pz", &Fgen_pz, "Fgen_pz/D");
  tree->Branch("Fgen_E", &Fgen_E, "Fgen_E/D");
  tree->Branch("N1gen_px", &N1gen_px, "N1gen_px/D");
  tree->Branch("N1gen_py", &N1gen_py, "N1gen_py/D");
  tree->Branch("N1gen_pz", &N1gen_pz, "N1gen_pz/D");
  tree->Branch("N1gen_E", &N1gen_E, "N1gen_E/D");
  tree->Branch("N2gen_px", &N2gen_px, "N2gen_px/D");
  tree->Branch("N2gen_py", &N2gen_py, "N2gen_py/D");
  tree->Branch("N2gen_pz", &N2gen_pz, "N2gen_pz/D");
  tree->Branch("N2gen_E", &N2gen_E, "N2gen_E/D");
  tree->Branch("Erel_gen", &Erel_gen, "Erel_gen/D");

  Double_t fNEBdr[50] = {nan};
  Double_t fNEBdrcut[50] = {nan};
  Double_t fNEBdt[50] = {nan};
  Double_t fNEBdtcut[50] = {nan};

  Double_t fW1beta[50] = {nan};
  Double_t fW1dbeta[50] = {nan};
  Double_t fW1betacut[50] = {nan};
  Double_t fW1dbetacut[50] = {nan};
  Double_t fW1Q1[50] = {nan};
  Double_t fW1Q2[50] = {nan};

  Double_t fW2beta[50] = {nan};
  Double_t fW2dbeta[50] = {nan};
  Double_t fW2betacut[50] = {nan};
  Double_t fW2dbetacut[50] = {nan};
  Double_t fW2Q1[50] = {nan};
  Double_t fW2Q2[50] = {nan};

  for (Int_t i=0;i<50;++i){
    fNEBdr[i] = nan;
    fNEBdrcut[i] = nan;
    fNEBdt[i] = nan;
    fNEBdtcut[i] = nan;

    fW1beta[i] = nan;
    fW1dbeta[i] = nan;
    fW1betacut[i] = nan;
    fW1dbetacut[i] = nan;
    fW1Q2[i] = nan;
    fW1Q1[i] = nan;

    fW2beta[i] = nan;
    fW2dbeta[i] = nan;
    fW2betacut[i] = nan;
    fW2dbetacut[i] = nan;
    fW2Q1[i] = nan;
    fW2Q2[i] = nan;
  }
  tree->Branch("fNEBdr[50]", &fNEBdr,"fNEBdr[50]/D");
  tree->Branch("fNEBdrcut[50]", &fNEBdrcut,"fNEBdrcut[50]/D");
  tree->Branch("fNEBdt[50]", &fNEBdt,"fNEBdt[50]/D");
  tree->Branch("fNEBdtcut[50]", &fNEBdtcut,"fNEBdtcut[50]/D");

  tree->Branch("fW1beta[50]", &fW1beta,"fW1beta[50]/D");
  tree->Branch("fW1dbeta[50]", &fW1dbeta,"fW1dbeta[50]/D");
  tree->Branch("fW1betacut[50]", &fW1betacut,"fW1betacut[50]/D");
  tree->Branch("fW1dbetacut[50]", &fW1dbetacut,"fW1dbetacut[50]/D");
  tree->Branch("fW1Q1[50]", &fW1Q1,"fW1Q1[50]/D");
  tree->Branch("fW1Q2[50]", &fW1Q2,"fW1Q2[50]/D");

  tree->Branch("fW2beta[50]", &fW2beta,"fW2beta[50]/D");
  tree->Branch("fW2dbeta[50]", &fW2dbeta,"fW2dbeta[50]/D");
  tree->Branch("fW2betacut[50]", &fW2betacut,"fW2betacut[50]/D");
  tree->Branch("fW2dbetacut[50]", &fW2dbetacut,"fW2dbetacut[50]/D");
  tree->Branch("fW2Q1[50]", &fW2Q1,"fW2Q1[50]/D");
  tree->Branch("fW2Q2[50]", &fW2Q2,"fW2Q2[50]/D");



  Double_t        fNEBQAveCut[144] = {nan};
  Double_t        fNEBTOFCut[144] = {nan};
  Double_t        fNEBLgtCut[144] = {nan};
  Double_t        fNEBBetaCut[144] = {nan};
  Int_t           fNEBIDCut[144] = {nan};
  Int_t           fNEBLayerCut[144] = {nan};
  Double_t        fNEBXCut[144] = {nan};
  Double_t        fNEBYCut[144] = {nan};
  Double_t        fNEBZCut[144] = {nan};
  Int_t fNEBWallbit = -1;
  Int_t fNEBINVmulti = -1;
  for(Int_t i=0;i<144;++i){
    fNEBQAveCut[i] = nan;
    fNEBTOFCut[i] = nan;
    fNEBLgtCut[i] = nan;
    fNEBBetaCut[i] = nan;
    fNEBIDCut[i] = nan;
    fNEBLayerCut[i] = nan;
    fNEBXCut[i] = nan;
    fNEBYCut[i] = nan;
    fNEBZCut[i] = nan;
  }
  tree->Branch("fNEBQAveCut[144]", &fNEBQAveCut,"fNEBQAveCut[144]/D");
  tree->Branch("fNEBTOFCut[144]", &fNEBTOFCut,"fNEBTOFCut[144]/D");
  tree->Branch("fNEBLgtCut[144]", &fNEBLgtCut,"fNEBLgtCut[144]/D");
  tree->Branch("fNEBBetaCut[144]", &fNEBBetaCut,"fNEBBetaCut[144]/D");
  tree->Branch("fNEBIDCut[144]", &fNEBIDCut,"fNEBIDCut[144]/I");
  tree->Branch("fNEBLayerCut[144]", &fNEBLayerCut,"fNEBLayerCut[144]/I");
  tree->Branch("fNEBXCut[144]", &fNEBXCut,"fNEBXCut[144]/D");
  tree->Branch("fNEBYCut[144]", &fNEBYCut,"fNEBYCut[144]/D");
  tree->Branch("fNEBZCut[144]", &fNEBZCut,"fNEBZCut[144]/D");
  tree->Branch("fNEBWallbit", &fNEBWallbit,"fNEBWallbit/I");
  tree->Branch("fNEBINVmulti", &fNEBINVmulti,"fNEBINVmulti/I");

  Double_t Mass_inv_15Bnn = nan;
  Double_t Mass_inv_15Bn = nan;
  Double_t E_rel_15Bnn = nan;
  Double_t E_rel_15Bn = nan; 
  Double_t E_rel_15Bnn_in = nan;
  Double_t E_rel_15Bn_in = nan;
  tree->Branch("Mass_inv_15Bnn", &Mass_inv_15Bnn,"Mass_inv_15Bnn/D");
  tree->Branch("Mass_inv_15Bn", &Mass_inv_15Bn,"Mass_inv_15Bn/D");
  tree->Branch("E_rel_15Bnn", &E_rel_15Bnn,"E_rel_15Bnn/D");
  tree->Branch("E_rel_15Bn", &E_rel_15Bn,"E_rel_15Bn/D");
  tree->Branch("E_rel_15Bnn_in", &E_rel_15Bnn_in,"E_rel_15Bnn_in/D");
  tree->Branch("E_rel_15Bn_in", &E_rel_15Bn_in,"E_rel_15Bn_in/D");

  Double_t Scat_17B_15Bnn = nan;
  Double_t Scat_17B_15Bn = nan;
  Double_t Scat_17B_15Bnn_in = nan;
  Double_t Scat_17B_15Bn_in = nan;
  Double_t Scat_17B_15Bnn_cm_c = nan;
  Double_t Scat_17B_15Bnn_cm_emp = nan;
  Double_t Scat_17B_15Bnn_cm_pb = nan;
  Double_t Scat_17B_15Bn_cm_c = nan;
  Double_t Scat_17B_15Bn_cm_emp = nan;
  Double_t Scat_17B_15Bn_cm_pb = nan;
  Double_t Theta_n1_n2 = nan;
  Double_t Theta_n1 = nan;
  Int_t DID = -1;
  Double_t DTOF = nan;
  tree->Branch("Scat_17B_15Bnn", &Scat_17B_15Bnn,"Scat_17B_15Bnn/D");
  tree->Branch("Scat_17B_15Bn", &Scat_17B_15Bn,"Scat_17B_15Bn/D");
  tree->Branch("Scat_17B_15Bnn_in", &Scat_17B_15Bnn_in,"Scat_17B_15Bnn_in/D");
  tree->Branch("Scat_17B_15Bn_in", &Scat_17B_15Bn_in,"Scat_17B_15Bn_in/D");
  tree->Branch("Scat_17B_15Bnn_cm_c", &Scat_17B_15Bnn_cm_c,"Scat_17B_15Bnn_cm_c/D");
  tree->Branch("Scat_17B_15Bnn_cm_emp", &Scat_17B_15Bnn_cm_emp,"Scat_17B_15Bnn_cm_emp/D");
  tree->Branch("Scat_17B_15Bnn_cm_pb", &Scat_17B_15Bnn_cm_pb,"Scat_17B_15Bnn_cm_pb/D");
  tree->Branch("Scat_17B_15Bn_cm_c", &Scat_17B_15Bn_cm_c,"Scat_17B_15Bn_cm_c/D");
  tree->Branch("Scat_17B_15Bn_cm_emp", &Scat_17B_15Bn_cm_emp,"Scat_17B_15Bn_cm_emp/D");
  tree->Branch("Scat_17B_15Bn_cm_pb", &Scat_17B_15Bn_cm_pb,"Scat_17B_15Bn_cm_pb/D");
  tree->Branch("Theta_n1_n2", &Theta_n1_n2,"Theta_n1_n2/D");
  tree->Branch("Theta_n1", &Theta_n1,"Theta_n1/D");
  tree->Branch("DID", &DID,"DID/I");
  tree->Branch("DTOF", &DTOF,"DTOF/D");

  Double_t test = nan;
  tree->Branch("test", &test,"test/D");

  //Physics parameters-----------------------------------------------------------------------------
  const Double_t clight =  299.792458; //[mm/ns]
  const Double_t m_n =  939.565413;//[MeV/c2]
  const Double_t m_c22 = 20543.416;//[MeV/c2]
  const Double_t m_c20 = 18667.386;//[MeV/c2]

  Double_t m_u = 931.4940954;//[MeV/c^2]atomic mass unit   
  Double_t m_b19=19.*m_u+58.777;
  Double_t m_b17=17.*m_u+43.770816;
  Double_t m_b15=15.*m_u+28.957988;
  
  //Event Loop=====================================================================================
  Int_t numB17beam = 0;
  Int_t numB17frag = 0;
  Int_t numB15frag = 0;

  Int_t neve = oritree -> GetEntries();
  cout << "all events:" << oritree->GetEntries() << endl;

  for (Int_t ieve=0;ieve<neve;++ieve){
    if(ieve%1000==0){
      cout<<"\revent:  "<<ieve<<"/"<<neve
	  <<" ("<<100*ieve/neve<<"%)"
	  <<flush;
    }
    oritree -> GetEntry(ieve);

    if(ok_fdc1==0) continue;
    if(ok_fdc2==0) continue;
    if(ok_hod==0) continue;
    if(ok_window==0) continue;

    //Analysis Gate Condition======================================================================
    for(Int_t i=0;i<144;++i){
      fNEBQAveCut[i] = nan;
      fNEBTOFCut[i] = nan;
      fNEBLgtCut[i] = nan;
      fNEBBetaCut[i] = nan;
      fNEBIDCut[i] = nan;
      fNEBLayerCut[i] = nan;
      fNEBXCut[i] = nan;
      fNEBYCut[i] = nan;
      fNEBZCut[i] = nan;
    }
    //Start NEBULA analysis------------------------------------------------------------------------
    if(fNEBV1Multi==1) continue;//||fNEBV2Multi==1) continue;//Remove 1st and 2nd VETO hit
    for(Int_t i=0;i<144;++i){
      fNEBQAveCut[i] = fNEBQAve[i];
      fNEBTOFCut[i] = fNEBTOF[i];
      fNEBLgtCut[i] = fNEBLgt[i];
      fNEBBetaCut[i] = fNEBBeta[i];
      fNEBIDCut[i] = fNEBID[i];
      fNEBLayerCut[i] = fNEBLayer[i];
      fNEBXCut[i] = fNEBX[i];
      fNEBYCut[i] = fNEBY[i];
      fNEBZCut[i] = fNEBZ[i];
    }
    
    //QAve & TOF Min gate--------------------------------------------------------------------------
    Int_t QAveTOFcutbit[144] = {0};
    for (Int_t i=0;i<144;++i){
      QAveTOFcutbit[i] = 0;
    }
    for(Int_t i=0;i<120;i++){//Remove NEUT <6 MeVee hits
      //if(fNEBQAve[i]<6.)QAveTOFcutbit[i] = 1;
       if(fNEBQAve[i]<6. || fNEBQAve[i]>200.)QAveTOFcutbit[i] = 1;
    }
    for(Int_t i=0;i<60;i++){//Remove 1st NEUT wall TOF<40nsec hits
      if(fNEBTOF[i]<40.)QAveTOFcutbit[i] = 1;
    }
    for(Int_t i=60;i<120;i++){//Remove 2nd NEUT wall TOF<42nsec hits
      if(fNEBTOF[i]<42.)QAveTOFcutbit[i] = 1;
    }
    for(Int_t i=132;i<144;i++){//Remove 2nd VETO wall TOF<42nsec hits
      if(fNEBTOF[i]<42.)QAveTOFcutbit[i] = 1;
    }
    
    for (Int_t i=0;i<144;++i){
      if(QAveTOFcutbit[i] == 1){
        fNEBQAveCut[i]=nan; fNEBTOFCut[i]=nan; fNEBLgtCut[i] = nan; fNEBBetaCut[i] = nan; fNEBIDCut[i] = -1; fNEBLayerCut[i] = -1; fNEBXCut[i] = nan; fNEBYCut[i] = nan; fNEBZCut[i] = nan;
      }
    }

    //Remove 2nd NEUT wall hits after 2nd VETO hit-------------------------------------------------
    Int_t VETOcutbit[144] = {0};
    for (Int_t i=0;i<144;++i){
      VETOcutbit[i] = 0;
    }
    for(Int_t i=132;i<144;i++){
      if(fNEBTOFCut[i]>0){
        VETOcutbit[i] = 1;
    	for(Int_t j=60;j<120;j++){//2nd NEUT dr(xy)<50cm, 1ns<dt(xy)<5ns
          if(fNEBTOFCut[j]>0 && sqrt(pow(fNEBXCut[i]-fNEBXCut[j],2.)+pow(fNEBYCut[i]-fNEBYCut[j],2.))<500.) VETOcutbit[j] = 1;
    	  if((fNEBTOFCut[j]-fNEBTOFCut[i])<5.&&(fNEBTOFCut[j]-fNEBTOFCut[i])>1.) VETOcutbit[j] = 1;
        }
      }
    }
    
    for (Int_t i=0;i<120;++i){
      if(VETOcutbit[i] == 1){
        fNEBQAveCut[i]=nan; fNEBTOFCut[i]=nan; fNEBLgtCut[i] = nan; fNEBBetaCut[i] = nan; fNEBIDCut[i] = -1; fNEBLayerCut[i] = -1; fNEBXCut[i] = nan; fNEBYCut[i] = nan; fNEBZCut[i] = nan;
      }
    }
    //Remove2nd VETO Signals
    for (Int_t i=132;i<144;++i){
      fNEBQAveCut[i]=nan; fNEBTOFCut[i]=nan; fNEBLgtCut[i] = nan; fNEBBetaCut[i] = nan; fNEBIDCut[i] = -1; fNEBLayerCut[i] = -1; fNEBXCut[i] = nan; fNEBYCut[i] = nan; fNEBZCut[i] = nan;
    }
    
    //---------------------------------------------------------------------------------------------
    //Sort & Crosstalk analysis-17B->n+n+15B-------------------------------------------------------
    //drdt cut within the same wall----------------------------------------------------------------
    Int_t DrDtcutbit[144] = {0};
    for (Int_t i=0;i<144;++i){
      DrDtcutbit[i] = 0;
    }
    for(Int_t i=0;i<50;i++){
      fNEBdr[i] = nan; fNEBdt[i] = nan; fNEBdrcut[i] = nan; fNEBdtcut[i] = nan;
    }

    Int_t NEBMultiCut = 144;
    for(Int_t i=0;i<144;i++){//Set TOF = 10000 for sorting
      if(!(fNEBTOFCut[i]>0)){
        fNEBTOFCut[i]=10000;
        NEBMultiCut = NEBMultiCut - 1;
      }
    }

    int *index; index = new int[144]; TMath::Sort(144, fNEBTOFCut, index, false);

    for(Int_t i=0;i<144;i++){//Set TOF = nan for analyzing
      if(!(fNEBTOFCut[i]<10000)){
        fNEBTOFCut[i] = nan;
        //if(NEBMultiCut>1)cout << ieve << " SORT " << NEBMultiCut << " " << index[i] << " " << fNEBTOF[index[i]] << endl;
      }
    }

    for(Int_t i=0;i<NEBMultiCut-1;i++){
      for(Int_t j=i+1;j<NEBMultiCut;j++){
	if(fNEBLayerCut[index[i]]>0&&fNEBLayerCut[index[j]]>0){//no gamma ray
        if((fNEBLayerCut[index[i]]<3 && fNEBLayerCut[index[j]]<3) || (fNEBLayerCut[index[i]]>2 && fNEBLayerCut[index[j]]>2)){//only same wall
          fNEBdr[NEBMultiCut] = sqrt(pow(fNEBXCut[index[j]] - fNEBXCut[index[i]], 2.)+pow(fNEBYCut[index[j]] - fNEBYCut[index[i]], 2.)+pow(fNEBZCut[index[j]] - fNEBZCut[index[i]],2.));
          fNEBdt[NEBMultiCut] = fNEBTOFCut[index[j]] - fNEBTOFCut[index[i]];
          fNEBdrcut[NEBMultiCut] =  fNEBdr[NEBMultiCut];
          fNEBdtcut[NEBMultiCut] =  fNEBdt[NEBMultiCut];
	  //if((pow((fNEBdr[NEBMultiCut]-100.1)/3./63.34,2) + pow((fNEBdt[NEBMultiCut]-0.4847)/3./0.3552,2))<1.){//15B+2n 230110
          //if((pow((fNEBdr[NEBMultiCut]-95.35)/5./64.92,2) + pow((fNEBdt[NEBMultiCut]-0.4881)/5./0.3537,2))<1.){//15B+2n 231003
	  if((pow((fNEBdr[NEBMultiCut]-98.34)/(5*71.07),2) + pow((fNEBdt[NEBMultiCut]-0.6549)/(5*0.4043),2))<1.){//2311108        
	  fNEBdrcut[NEBMultiCut] = nan;
	  fNEBdtcut[NEBMultiCut] = nan;
	  DrDtcutbit[index[j]] = 1; 
	  }
        }
	}
      }
    }
    for (Int_t i=0;i<144;++i){
      if(DrDtcutbit[i] == 1){
        fNEBQAveCut[i]=nan; fNEBTOFCut[i]=nan; fNEBLgtCut[i] = nan; fNEBBetaCut[i] = nan; fNEBIDCut[i] = -1; fNEBLayerCut[i] = -1; fNEBXCut[i] = nan; fNEBYCut[i] = nan; fNEBZCut[i] = nan;
      }
    }

    delete [] index;
    //---------------------------------------------------------------------------------------------

    //Sort & Crosstalk analysis-17B->n+n+15B-------------------------------------------------------------------------------------------------------------------------------------------------
    //Cross Talk cut  same wall & diff wall--------------------------------------------------------------------------------------------------------------------------------------------------
    for(Int_t i=0;i<50;i++){
      fW1beta[i] = nan; fW1dbeta[i] = nan; fW1betacut[i] = nan; fW1dbetacut[i] = nan; fW1Q1[i] = nan; fW1Q2[i] = nan;
      fW2beta[i] = nan; fW2dbeta[i] = nan; fW2betacut[i] = nan; fW2dbetacut[i] = nan; fW2Q1[i] = nan; fW2Q2[i] = nan;
    }

    Int_t SWallcutbit[144] = {0};
    for (Int_t i=0;i<144;++i){
      SWallcutbit[i] = 0;
    }

    Double_t W1NEBTOF[120] = {nan};
    for(Int_t i=0;i<120;++i){
      W1NEBTOF[i] = fNEBTOFCut[i];
    }

    Int_t W1NEBCut = 120; 
    for(Int_t i=0;i<120;i++){//Set TOF = 10000 for sorting
      if(!(W1NEBTOF[i]>0)){
        W1NEBTOF[i]=10000;
        W1NEBCut = W1NEBCut - 1;
      }
    }
    int *w1index; w1index = new int[120]; TMath::Sort(120, W1NEBTOF, w1index, false);

    for(Int_t i=0;i<120;i++){//Set TOF = nan for analyzing
      if(!(W1NEBTOF[i]<10000)) W1NEBTOF[i] = nan;
    }

    for(Int_t i=0;i<W1NEBCut-1;i++){
      for(Int_t j=i+1;j<W1NEBCut;j++){

        Double_t fWbeta = fNEBLgtCut[w1index[i]]/(fNEBTOFCut[w1index[i]]*clight); 
        Double_t fWdbeta = sqrt(pow(fNEBXCut[w1index[j]]-fNEBXCut[w1index[i]],2) + pow(fNEBYCut[w1index[j]]-fNEBYCut[w1index[i]],2) + pow(fNEBZCut[w1index[j]]-fNEBZCut[w1index[i]],2)) / ((fNEBTOFCut[w1index[j]] - fNEBTOFCut[w1index[i]])*clight);
        Double_t fWQ1 = fNEBQAveCut[w1index[i]];
        Double_t fWQ2 = fNEBQAveCut[w1index[j]];

        if(fNEBZCut[w1index[j]]-fNEBZCut[w1index[i]]<0) fWdbeta = -1. * fWdbeta;

        //-----------------------------------------------------------------------------------------------------------------------------------------------
        if(fNEBLayerCut[w1index[i]]>0&&fNEBLayerCut[w1index[j]]>0){//no gamma
	if((fNEBLayerCut[w1index[i]] < 3 && fNEBLayerCut[w1index[j]] < 3) || ( fNEBLayerCut[w1index[i]] > 2 && fNEBLayerCut[w1index[j]] > 2)){//SAME WALL

          fW1beta[W1NEBCut] = fWbeta;
          fW1dbeta[W1NEBCut] = fWdbeta;
          fW1Q1[W1NEBCut] = fWQ1;
          fW1Q2[W1NEBCut] = fWQ2;

          fW1betacut[W1NEBCut] =  fWbeta;
          fW1dbetacut[W1NEBCut] = fWdbeta;

	  if((fWbeta/fWdbeta)>1. || (fWbeta/fWdbeta)<-1.5){
	    SWallcutbit[w1index[j]] = 1;
            fW1betacut[W1NEBCut] = nan;
            fW1dbetacut[W1NEBCut] = nan; 
	  }

	  if(fabs(fabs(1./fWdbeta)-1.)<3.*0.1 && fWQ2<20){
	    SWallcutbit[w1index[j]] = 1;
	    fW1betacut[W1NEBCut] = nan;
	    fW1dbetacut[W1NEBCut] = nan;
	  }
        }  
        //-----------------------------------------------------------------------------------------------------------------------------------------------
        else if((fNEBLayerCut[w1index[i]] < 3 && fNEBLayerCut[w1index[j]] > 2) || ( fNEBLayerCut[w1index[i]] > 2 && fNEBLayerCut[w1index[j]] < 3)){//DIFF WALL

          fW2beta[W1NEBCut] = fWbeta;
          fW2dbeta[W1NEBCut] = fWdbeta;
          fW2Q1[W1NEBCut] = fWQ1;
          fW2Q2[W1NEBCut] = fWQ2;

          fW2betacut[W1NEBCut] =  fWbeta;
          fW2dbetacut[W1NEBCut] = fWdbeta;

          if(fWbeta/fWdbeta>1. || fWbeta/fWdbeta<-1.5){
            SWallcutbit[w1index[j]] = 1;
            fW2betacut[W1NEBCut] = nan;
            fW2dbetacut[W1NEBCut] = nan;
          }
	  if(fabs(fabs(fWdbeta)-1.)<3.*0.1 && fWQ2<20){
	    SWallcutbit[w1index[j]] = 1;
	    fW2betacut[W1NEBCut] = nan;
	    fW2dbetacut[W1NEBCut] = nan;
	  }      
        }
	}
      }
    }
    delete [] w1index;
    
    for (Int_t i=0;i<144;++i){
      if(SWallcutbit[i] == 1){
	fNEBQAveCut[i]=nan; fNEBTOFCut[i]=nan; fNEBLgtCut[i] = nan; fNEBBetaCut[i] = nan; fNEBIDCut[i] = -1; fNEBLayerCut[i] = -1; fNEBXCut[i] = nan; fNEBYCut[i] = nan; fNEBZCut[i] = nan;
      }
    }
      //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    
      //Generate relative energy---------------------------------------------------------------------------------------------------------------------------------------------------------------
    
    Double_t fNEBTOFINV[120] = {0};
    Mass_inv_15Bnn = nan;
    Mass_inv_15Bn = nan;
    E_rel_15Bnn = nan;
    E_rel_15Bn = nan;
    E_rel_15Bnn_in = nan;
    E_rel_15Bn_in = nan;
    
    Scat_17B_15Bnn = nan;
    Scat_17B_15Bn = nan;
    Scat_17B_15Bnn_in = nan;
    Scat_17B_15Bnn_cm_c = nan;
    Scat_17B_15Bnn_cm_emp = nan;
    Scat_17B_15Bnn_cm_pb = nan;
    Scat_17B_15Bn_in = nan;
    Scat_17B_15Bn_cm_c = nan;
    Scat_17B_15Bn_cm_emp = nan;
    Scat_17B_15Bn_cm_pb = nan;
    Theta_n1_n2 = nan;
    Theta_n1 = nan;
    DID = -1;
    DTOF = nan;
    
      fNEBWallbit = -1;
      
      fNEBINVmulti = 120;
      for(Int_t i=0;i<120;i++){//Set TOF = 10000 for sorting
        if(fNEBTOFCut[i]>0){
          fNEBTOFINV[i]=fNEBTOFCut[i];
        }
        if(!(fNEBTOFCut[i]>0)){
          fNEBTOFINV[i]=10000;
          fNEBINVmulti = fNEBINVmulti - 1;
        }
      }

      int *invindex; invindex = new int[120]; TMath::Sort(120, fNEBTOFINV, invindex, false);

      if(fNEBLayerCut[invindex[0]]>0 && fNEBLayerCut[invindex[1]]>0 && fNEBINVmulti > 1){

        if(fNEBLayerCut[invindex[0]]<3 && fNEBLayerCut[invindex[1]]<3) fNEBWallbit = 1;//samewall1
        if(fNEBLayerCut[invindex[0]]>2 && fNEBLayerCut[invindex[1]]>2) fNEBWallbit = 2;//samewall2
        if((fNEBLayerCut[invindex[0]]<3 && fNEBLayerCut[invindex[1]]>2) || (fNEBLayerCut[invindex[0]]>2 && fNEBLayerCut[invindex[1]]<3)) fNEBWallbit = 3;//diffwall


        TVector3 vec_tmp1(fNEBXCut[invindex[0]] - tgtx, fNEBYCut[invindex[0]] - tgty, fNEBZCut[invindex[0]]);
        vec_tmp1 *= 1./(fNEBTOFCut[invindex[0]]*clight);
        Double_t beta1 = vec_tmp1.Mag();
        Double_t E1 = m_n/sqrt(1-beta1*beta1);
        vec_tmp1 *= E1;
        TLorentzVector vec1(vec_tmp1,E1);

        TVector3 vec_tmp2(fNEBXCut[invindex[1]] - tgtx, fNEBYCut[invindex[1]] - tgty, fNEBZCut[invindex[1]]);
        vec_tmp2 *= 1./(fNEBTOFCut[invindex[1]]*clight);
        Double_t beta2 = vec_tmp2.Mag();
        Double_t E2 = m_n/sqrt(1-beta2*beta2);
        vec_tmp2 *= E2;
        TLorentzVector vec2(vec_tmp2,E2);

        TLorentzVector vecf(Fgen_px,Fgen_py,Fgen_pz,Fgen_E);

        Mass_inv_15Bnn = (vec1+vec2+vecf).Mag(); 
        E_rel_15Bnn = Mass_inv_15Bnn  - m_b15 - m_n - m_n;
      	E_rel_15Bnn_in = Erel_in; 

        //if(fNEBWallbit==1)cout << ieve << " " << fNEBWallbit << " " << E_rel_15Bnn << endl;

        TVector3 vec_beam(Bgen_px,Bgen_py,Bgen_pz); 
      	TVector3 vec_frag(Fgen_px,Fgen_py,Fgen_pz); 
        test =  vec_frag.Angle(vec_beam);
        TVector3 vec_fragtot;
        TLorentzVector vec4_fragtot = vec1 + vec2 + vecf;
 
        vec_fragtot = vec_frag + vec_tmp1 + vec_tmp2;
        Scat_17B_15Bnn = vec_fragtot.Angle(vec_beam);
        Scat_17B_15Bnn_in = scat_ang;
      	Scat_17B_15Bnn_cm_c = scat_gen_cm[0];
      	Scat_17B_15Bnn_cm_emp = scat_gen_cm[1];
      	Scat_17B_15Bnn_cm_pb = scat_gen_cm[2];
        Theta_n1_n2 = vec_tmp1.Angle(vec_tmp2);
        DID = abs(invindex[1] - invindex[0]);
        DTOF = abs(fNEBTOFCut[invindex[1]] - fNEBTOFCut[invindex[0]]);

      }
      
      if(fNEBLayerCut[invindex[0]]>0 && fNEBINVmulti > 0){

        TVector3 vec_tmp(fNEBXCut[invindex[0]] - tgtx, fNEBYCut[invindex[0]] - tgty, fNEBZCut[invindex[0]]);
        vec_tmp *= 1./(fNEBTOFCut[invindex[0]]*clight);
        Double_t beta = vec_tmp.Mag();
        Double_t E = m_n/sqrt(1-beta*beta);
        vec_tmp *= E;
        TLorentzVector vec(vec_tmp,E);

        TLorentzVector vecf(Fgen_px,Fgen_py,Fgen_pz,Fgen_E);

        Mass_inv_15Bn = (vec+vecf).M();
        E_rel_15Bn = Mass_inv_15Bn  - m_b15 - m_n;
        E_rel_15Bn_in = Erel_in;//uniformly generated
 
        TVector3 vec_beam(Bgen_px,Bgen_py,Bgen_px); 
        TVector3 vec_frag(Fgen_px,Fgen_py,Fgen_px); 
        TVector3 vec_fragtot; 

        vec_fragtot = vec_frag + vec_tmp;
        Scat_17B_15Bn = vec_fragtot.Angle(vec_beam);
        Scat_17B_15Bn_in = scat_ang;//uniformly generated
	      Scat_17B_15Bn_cm_c = scat_gen_cm[0];
	      Scat_17B_15Bn_cm_emp = scat_gen_cm[1];
	      Scat_17B_15Bn_cm_pb = scat_gen_cm[2];
        Theta_n1 = vec_beam.Angle(vec_tmp);

      }    
      tree->Fill();
    }
    outfile->Write();
  }
