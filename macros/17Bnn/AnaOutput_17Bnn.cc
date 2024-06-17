/*
  Simple analysis example
  root[0] .L macros/examples/analysis_example.cc+g
  root[1] analysis_example()
*/
#include <iostream>
#include <TString.h>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TClonesArray.h"
#include "TChain.h"
#include "TMath.h"

#include "TBeamSimData.hh"
#include "TDetectorSimParameter.hh"
#include "TSimParameter.hh"
#include "TSimData.hh"
#include "TRunSimParameter.hh"
#include "TFragSimParameter.hh"

#include "TArtNEBULAPla.hh"
#include "TArtCalibNEBULA.hh"

void AnaOutput_17Bnn(Int_t irun, TString tag){
//void AnaOutput_17Bnn(){

  TFile *file = new TFile(Form("./root/Sim/%s/Sim17Bnn%02d_%s0000.root",tag.Data(),irun,tag.Data()),"readonly");
  //TFile *file = new TFile("./root/Sim/Sim17Bnn0000.root","readonly");
  
  TRunSimParameter *RunPrm = 0;
  file->GetObject("RunParameter",RunPrm);
  TFragSimParameter *FragPrm = 0;
  file->GetObject("FragParameter",FragPrm);

  RunPrm->Print();
  TTree *tree = 0;
  file->GetObject(RunPrm->fTreeName.Data(),tree);

  TClonesArray *NEBULAPlaArray = new TClonesArray("TArtNEBULAPla",144);
  TClonesArray *gFragSimDataArray = new TClonesArray("TSimData",256);

  tree->SetBranchAddress("beam",&gBeamSimDataArray);
  tree->SetBranchAddress("FragSimData",&gFragSimDataArray);
  tree->SetBranchAddress("NEBULAPla",&NEBULAPlaArray);
  
  Bool_t ok_fdc1 = 0; 
  Bool_t ok_window = 0; 
  Bool_t ok_fdc2 = 0; 
  Bool_t ok_hod = 0;
  tree->SetBranchAddress("ok_fdc1",&ok_fdc1);
  tree->SetBranchAddress("ok_window",&ok_window);
  tree->SetBranchAddress("ok_fdc2",&ok_fdc2);
  tree->SetBranchAddress("ok_hod",&ok_hod);

  TChain* paramtree = new TChain("tree","ALL tree clone");
  paramtree -> Add(Form("./root/Gen/%s/17BnnInpParam%02d_%s.root",tag.Data(),irun,tag.Data()));
  //paramtree -> Add("./root/Gen/17BnnInpParam.root");

  const Double_t nan = TMath::QuietNaN();
  Double_t scat_ang = nan;
  Double_t scat_gen_cm[3] = {nan};
  Double_t Erel_in = nan;
  Double_t Bgen_px = nan; Double_t Bgen_py = nan; Double_t Bgen_pz = nan; Double_t Bgen_E = nan;
  Double_t Fgen_px = nan; Double_t Fgen_py = nan; Double_t Fgen_pz = nan; Double_t Fgen_E = nan;
  Double_t N1gen_px = nan; Double_t N1gen_py = nan; Double_t N1gen_pz = nan; Double_t N1gen_E = nan;
  Double_t N2gen_px = nan; Double_t N2gen_py = nan; Double_t N2gen_pz = nan; Double_t N2gen_E = nan;
  
  Double_t Erel_gen = nan;

  paramtree->SetBranchAddress("scat_ang",&scat_ang);
  paramtree->SetBranchAddress("Erel_in",&Erel_in);
  paramtree->SetBranchAddress("scat_gen_cm[3]",scat_gen_cm);

  paramtree->SetBranchAddress("Bgen_px", &Bgen_px);
  paramtree->SetBranchAddress("Bgen_py", &Bgen_py);
  paramtree->SetBranchAddress("Bgen_pz", &Bgen_pz);
  paramtree->SetBranchAddress("Bgen_E", &Bgen_E);
  paramtree->SetBranchAddress("Fgen_px", &Fgen_px);
  paramtree->SetBranchAddress("Fgen_py", &Fgen_py);
  paramtree->SetBranchAddress("Fgen_pz", &Fgen_pz);
  paramtree->SetBranchAddress("Fgen_E", &Fgen_E);
  paramtree->SetBranchAddress("N1gen_px", &N1gen_px);
  paramtree->SetBranchAddress("N1gen_py", &N1gen_py);
  paramtree->SetBranchAddress("N1gen_pz", &N1gen_pz);
  paramtree->SetBranchAddress("N1gen_E", &N1gen_E);
  paramtree->SetBranchAddress("N2gen_px", &N2gen_px);
  paramtree->SetBranchAddress("N2gen_py", &N2gen_py);
  paramtree->SetBranchAddress("N2gen_pz", &N2gen_pz);
  paramtree->SetBranchAddress("N2gen_E", &N2gen_E);
  paramtree->SetBranchAddress("Erel_gen", &Erel_gen);

  TFile* outfile = new TFile(Form("./root/Ana/%s/AnaSim17Bnn%02d_%s.root",tag.Data(),irun,tag.Data()),"recreate");
  //TFile* outfile = new TFile("./root/Ana/AnaSim17Bnn_test.root","recreate");
  TTree* outtree = new TTree("tree","ALL Event Tree");
  
  const Double_t clight = 299.792458; //[mm/nsec]
  
  Double_t tgtx = nan; Double_t tgty = nan; Double_t tgtz = nan; 
  Double_t neutron0x = nan; Double_t neutron0y = nan; Double_t neutron0z = nan; 
  outtree->Branch("tgtx",&tgtx,"tgtx/D");
  outtree->Branch("tgty",&tgty,"tgty/D");
  outtree->Branch("tgtz",&tgtz,"tgtz/D");
  outtree->Branch("neutron0x",&neutron0x,"neutron0x/D");
  outtree->Branch("neutron0y",&neutron0y,"neutron0y/D");
  outtree->Branch("neutron0z",&neutron0z,"neutron0z/D");
 
  outtree->Branch("scat_ang", &scat_ang, "scat_ang/D");
  outtree->Branch("scat_gen_cm[3]",&scat_gen_cm,"scat_gen_cm[3]/D");
  outtree->Branch("Erel_in", &Erel_in, "Erel_in/D");

  outtree->Branch("Bgen_px", &Bgen_px, "Bgen_px/D");
  outtree->Branch("Bgen_py", &Bgen_py, "Bgen_py/D");
  outtree->Branch("Bgen_pz", &Bgen_pz, "Bgen_pz/D");
  outtree->Branch("Bgen_E", &Bgen_E, "Bgen_E/D");
  outtree->Branch("Fgen_px", &Fgen_px, "Fgen_px/D");
  outtree->Branch("Fgen_py", &Fgen_py, "Fgen_py/D");
  outtree->Branch("Fgen_pz", &Fgen_pz, "Fgen_pz/D");
  outtree->Branch("Fgen_E", &Fgen_E, "Fgen_E/D");
  outtree->Branch("N1gen_px", &N1gen_px, "N1gen_px/D");
  outtree->Branch("N1gen_py", &N1gen_py, "N1gen_py/D");
  outtree->Branch("N1gen_pz", &N1gen_pz, "N1gen_pz/D");
  outtree->Branch("N1gen_E", &N1gen_E, "N1gen_E/D");
  outtree->Branch("N2gen_px", &N2gen_px, "N2gen_px/D");
  outtree->Branch("N2gen_py", &N2gen_py, "N2gen_py/D");
  outtree->Branch("N2gen_pz", &N2gen_pz, "N2gen_pz/D");
  outtree->Branch("N2gen_E", &N2gen_E, "N2gen_E/D");
  outtree->Branch("Erel_gen", &Erel_gen, "Erel_gen/D");

  outtree->Branch("ok_fdc1",&ok_fdc1,"ok_fdc1/O");
  outtree->Branch("ok_window",&ok_window,"ok_window/O");
  outtree->Branch("ok_fdc2",&ok_fdc2,"ok_fdc2/O");
  outtree->Branch("ok_hod",&ok_hod,"ok_hod/O");

  //NEBULA
  Double_t fNEBQAve[144] = {nan};
  Double_t fNEBTOF[144]  = {nan};
  Double_t fNEBLgt[144]  = {nan};
  Double_t fNEBBeta[144] = {nan};
  Int_t fNEBID[144]    = {-1};
  Int_t fNEBLayer[144] = {-1};
  Double_t fNEBX[144] = {nan};
  Double_t fNEBY[144] = {nan}; 
  Double_t fNEBZ[144] = {nan};
  Int_t fNEBV1Multi = -1;
  Int_t fNEBV2Multi = -1;

  for(Int_t i=0;i<144;i++){
    fNEBQAve[i]  = nan;  
    fNEBTOF[i]   = nan; 
    fNEBLgt[i]   = nan; 
    fNEBBeta[i]  = nan; 
    fNEBID[i]    = -1; 
    fNEBLayer[i] = -1;
  }

  outtree->Branch("fNEBQAve[144]", &fNEBQAve,"fNEBQAve[144]/D"); 
  outtree->Branch("fNEBTOF[144]", &fNEBTOF,"fNEBTOF[144]/D");
  outtree->Branch("fNEBLgt[144]", &fNEBLgt,"fNEBLgt[144]/D"); 
  outtree->Branch("fNEBBeta[144]", &fNEBBeta,"fNEBBeta[144]/D");
  outtree->Branch("fNEBID[144]", &fNEBID,"fNEBID[144]/I"); 
  outtree->Branch("fNEBLayer[144]", &fNEBLayer,"fNEBLayer[144]/I");
  outtree->Branch("fNEBX[144]", &fNEBX,"fNEBX[144]/D"); 
  outtree->Branch("fNEBY[144]", &fNEBY,"fNEBY[144]/D"); 
  outtree->Branch("fNEBZ[144]", &fNEBZ,"fNEBZ[144]/D");
  outtree->Branch("fNEBV1Multi", &fNEBV1Multi,"fNEBV1Multi/I");
  outtree->Branch("fNEBV2Multi", &fNEBV2Multi,"fNEBV2Multi/I");

  //Histogram----------------------------------------------------------------------------------------
  TH2* hbeamposxy = new TH2D("hbeamposxy","Beam Pos XY (mm)",200,-50,50,200,-50,50);
  TH2* hbeamposxy_gated = new TH2D("hbeamposxy_gated","Beam Pos XY (mm)",200,-50,50,200,-50,50);
  TH2* hneu0posxy = new TH2D("hneu0posxy","Neutron 0 Pos XY (mm)",200,-50,50,200,-50,50);
  TH1* hmulti = new TH1I("hmulti","NEBULA Multiplicity",10,-0.5,9.5);
  TH2* hnebulatq = new TH2D("hnebulatq","NEBULA TOF(ns) Q(MeVee)",200,0,200,200,0,200);
  TH2* hnebulaxy = new TH2D("hnebulaxy","NEBULA Pos XY (mm)",200,-2000,2000,200,-2000,2000);
  TH2* hnebulazx = new TH2D("hnebulazx","NEBULA Pos ZX (mm)",200,6000,10000,200,-2000,2000);
  TH2* hnebulatq1 = new TH2D("hnebulatq1","NEBULA TOF(ns) Q(MeVee)",200,0,200,200,0,200);
  TH2* hnebulaxy1 = new TH2D("hnebulaxy1","NEBULA Pos XY (mm)",200,-2000,2000,200,-2000,2000);
  TH2* hnebulazx1 = new TH2D("hnebulazx1","NEBULA Pos ZX (mm)",200,6000,10000,200,-2000,2000);

  //Start fill tree----------------------------------------------------------------------------------
  Int_t neve = tree->GetEntries();
  for (Int_t ieve=0;ieve<neve;++ieve){
    if (ieve%1000==0){
      cout<<"\r events: "<<ieve<<" / "<<neve
          <<" ("<<100.*ieve/neve<<"%)"
          <<flush;
    }
    gBeamSimDataArray->clear();
    gFragSimDataArray->Clear();
    NEBULAPlaArray->Delete();

    tree->GetEntry(ieve);
    paramtree->GetEntry(ieve);
    //-----------------------------------------------------------------------------------------------

    // Get beam data---------------------------------------------------------------------------------
    tgtx = nan; tgty = nan; tgtz = nan;
    neutron0x = nan; neutron0y = nan; neutron0z = nan;

    Int_t nbeam = gBeamSimDataArray->size();//Number of decay
    //cout << "nbeam = " << nbeam << endl;
    for (Int_t ibeam=0;ibeam<nbeam;++ibeam){
      TBeamSimData beam = (*gBeamSimDataArray)[ibeam];
      
      //cout << "beam.fZ = " << beam.fZ << endl;
      if(beam.fZ==5){
	tgtx = beam.fPosition.fX;
	tgty = beam.fPosition.fY;
	tgtz = beam.fPosition.fZ;
        hbeamposxy->Fill(tgtx, tgty);
	if(ok_fdc1==1&&ok_window==1&&ok_fdc2==1&&ok_hod==1){
	  hbeamposxy_gated->Fill(tgtx, tgty);
	}
      }
      else if(beam.fZ==0){//neutron
	neutron0x = beam.fPosition.fX;
	neutron0y = beam.fPosition.fY;
	neutron0z = beam.fPosition.fZ;      
        hneu0posxy->Fill(neutron0x, neutron0y);
      }  
    }
    //----------------------------------------------------------------------------
    
    // Get fragment data----------------------------------------------------------
    Int_t nfrag = gFragSimDataArray->GetEntries();
    //cout << "nfrag = " << nfrag << endl;
    for(Int_t ifrag=0;ifrag<nfrag;++ifrag){
      TSimData *frag = (TSimData*)gFragSimDataArray->At(ifrag);
      //cout << "frag->fDetectorName = " << frag->fDetectorName << endl;
    }
    //cout << "ok_fdc1 = " << ok_fdc1 <<endl;
    //cout << "ok_window = " << ok_window <<endl;
    //cout << "ok_fdc2 = " << ok_fdc2 <<endl;
    //cout << "ok_hod = " << ok_hod <<endl;
    
    
    // Get NEBULA data------------------------------------------------------------
    
    fNEBV1Multi = 0;
    fNEBV2Multi = 0;
    for(Int_t i=0;i<144;i++){
        fNEBQAve[i] = nan;  
	fNEBTOF[i] = nan; 
	fNEBLgt[i] = nan;
	fNEBBeta[i] = nan;
        fNEBX[i] = nan; 
	fNEBY[i] = nan; 
	fNEBZ[i] = nan;
        fNEBID[i] = -1;
	fNEBLayer[i] = -1;
      } 
    
    Int_t npla = NEBULAPlaArray->GetEntries();
    // cout << " npla = " << npla << endl;

    //      return;
    
    hmulti->Fill(npla);
    for(Int_t ipla=0;ipla<npla;++ipla){
      TArtNEBULAPla *pla = (TArtNEBULAPla*)NEBULAPlaArray->At(ipla);
      hnebulatq->Fill(pla->GetTAveSlw(),pla->GetQAveCal());
      hnebulaxy->Fill(pla->GetPos(0),pla->GetPos(1));
      hnebulazx->Fill(pla->GetPos(2),pla->GetPos(0));
      
      Int_t ID = pla -> GetID();
      Int_t Layer = pla -> GetLayer();//1 or 2
      Int_t SubLayer = pla -> GetSubLayer();//0(VETO) or 1(NEUT1) or 2(NEUT2)
      
      fNEBQAve[ID-1] = pla -> GetQAveCal();
      fNEBTOF[ID-1] = pla -> GetTAveSlw();
      fNEBX[ID-1] = pla -> GetPos(0); 
      fNEBY[ID-1] = pla -> GetPos(1); 
      fNEBZ[ID-1] = pla -> GetPos(2) - neutron0z;//[mm]
      fNEBLgt[ID-1] = sqrt(pow(fNEBX[ID-1]-neutron0x, 2.) + pow(fNEBY[ID-1]-neutron0y, 2.) + pow(fNEBZ[ID-1], 2.));//[mm]
      fNEBBeta[ID-1] = fNEBLgt[ID-1]/(fNEBTOF[ID-1]*clight);
      fNEBID[ID-1] = ID-1;
      
      hnebulatq1->Fill(fNEBTOF[ID-1],fNEBQAve[ID-1]);
      hnebulaxy1->Fill(fNEBX[ID-1],fNEBY[ID-1]);
      hnebulazx1->Fill(fNEBZ[ID-1],fNEBX[ID-1]);

      if(Layer==1&&SubLayer==0&&fNEBQAve[ID-1]>0) fNEBV1Multi = 1;
      if(Layer==1&&SubLayer==0&&fNEBTOF[ID-1]>0) fNEBV1Multi++;//Why
      if(Layer==2&&SubLayer==0&&fNEBQAve[ID-1]>0) fNEBV2Multi = 1;
      
      if(SubLayer>0){
	if(Layer==1&&SubLayer==1&&fNEBQAve[ID-1]>0) fNEBLayer[ID-1] = 1;
	if(Layer==1&&SubLayer==2&&fNEBQAve[ID-1]>0) fNEBLayer[ID-1] = 2;
	if(Layer==2&&SubLayer==1&&fNEBQAve[ID-1]>0) fNEBLayer[ID-1] = 3;
	if(Layer==2&&SubLayer==2&&fNEBQAve[ID-1]>0) fNEBLayer[ID-1] = 4;
      }
      
    }
    outtree -> Fill();
  }

  
  outfile -> Write();
  outfile -> Close();
  return;
}
