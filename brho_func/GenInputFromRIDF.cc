#include "TBeamSimData.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TString.h"
#include "TNamed.h"

#include "TArtEventStore.hh"
#include "TArtEventInfo.hh"
#include "TArtUtil.hh"
#include "TArtStoreManager.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"
#include "TArtPlastic.hh"
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtDCTrack.hh"
#include "TArtIC.hh"
#include "TArtHODPla.hh"
#include "TArtHODPlaPara.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"
#include "TArtCalibDALI.hh"
#include "TArtDALINaI.hh"
#include "TArtCalibBPCHit.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtCalibFDC1Track.hh"
#include "TArtCalibFDC2Track.hh"
#include "TArtCalibDCHit.hh"
#include "TArtCalibDCTrack.hh"
#include "TArtCalibCoin.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibHODPla.hh"
#include "TArtCalibIC.hh"

#include <iostream>
#include <sstream>
#include <fstream>

void GenInputFromRIDF(Int_t num){

  const double AMU = 931.494043;//MeV
  const double c_light=299.792458;//mm/ns

  Int_t A, Z, nRun;
  Double_t M, fac, Zgate;
  if (runname=="17b_emp"){

  }else if (runname=="19b_emp"){
    A = 29;
    Z = 9;
    M=27049;//MeV
    nRun = 486;// 29F+Emp
    fac = 1;
    Zgate = 9;
    prm->LoadParameterList("db/List_29F_Emp_29F.csv");
  }else{
    cout<<"Unknown run name :"<<runname.Data()<<endl;
    return;
  }

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventStore *estore = new TArtEventStore;
  estore->Open(Form("./ridf/sdaq02/dayone%04d.ridf",num));
  //only use Empty target run?

  //Parameter files--------------------------------------------------
  TArtBigRIPSParameters *bigripspara = new TArtBigRIPSParameters;
  bigripspara->LoadParameter((char*)"db/BigRIPSPlastic.xml");
  bigripspara->LoadParameter((char*)"db/BigRIPSIC.xml");
  TArtSAMURAIParameters *sampara = new TArtSAMURAIParameters;
  sampara->LoadParameter((char*)"db/SAMURAIBPC.xml");
  sampara->LoadParameter((char*)"db/SAMURAIBDC1.xml");
  sampara->LoadParameter((char*)"db/SAMURAIBDC2.xml");
  sampara->LoadParameter((char*)"db/SAMURAIFDC1.xml");
  sampara->LoadParameter((char*)"db/SAMURAIFDC2.xml");
  sampara->LoadParameter((char*)"db/SAMURAIHOD.xml");
  sampara->LoadParameter((char*)"db/NEBULA.xml");

  //---------------------------------------------------------------------

  TFile* file = new TFile(Form("root/brho_func/input_run%04d.root",num),"recreate");
  TTree *tree = new TTree("tree_input","Input tree for trajectories");

  //Histograms----------------------------------------------------------------------
  TH1* hcoin   = new TH1I("hcoin","COIN",8,0.5,8.5);
  TH1* hcoin_g = new TH1I("hcoin_g","COIN DSB",8,0.5,8.5);

  TH1* haoqz   = new TH2D("haoqz","AoQ Z",400,2.0,3.5, 300,0,15);
  TH1* haoqz_g = new TH2D("haoqz_g","AoQ Z Gated",400,2.0,3.5, 300,0,15);

  TH1* hidz   = new TH2D("hidz","HOD ID Z",24,0.5,24.5, 200,0,15);
  TH1* hidz_g = new TH2D("hidz_g","HOD ID Z Gated",24,0.5,24.5, 200,0,15);
  //-----------------------------------------------------------------------------------

  //Tree Parameter and Branch---------------------------------------------------------

  //Simulation data of made beam
  gBeamSimDataArray = new TBeamSimDataArray();
  tree->Branch("TBeamSimData", gBeamSimDataArray);

  //QuietNaN for overflow & underflow
  Double_t nan = TMath::QuietNaN();

  //Event number & triggerbit                                                                                       
  Int_t fneve = -1;
  Int_t ftriggerbit[4] = {-1,-1,-1,-1};
  tree->Branch("fneve", &fneve,"fneve/I");
  tree->Branch("ftriggerbit[4]", &ftriggerbit,"ftriggerbit[4]/I");

  //f3,f7,f13 pla Q & T                                                                                             
  Double_t f3PlaTAve = nan; Double_t f3PlaQAve = nan; Double_t f3PlaTDiff = nan;
  Double_t f7PlaTAve = nan; Double_t f7PlaQAve = nan; Double_t f7PlaTDiff = nan;
  Double_t f13PlaTAve = nan; Double_t f13PlaQAve = nan; Double_t f13PlaTDiff = nan;
  Double_t fSBVQAve = nan;
  tree->Branch("f3PlaTAve", &f3PlaTAve,"f3PlaTAve/D");
  tree->Branch("f3PlaQAve", &f3PlaQAve,"f3PlaQAve/D");
  tree->Branch("f3PlaTDiff", &f3PlaTDiff,"f3PlaTDiff/D");
  tree->Branch("f7PlaTAve", &f7PlaTAve,"f7PlaTAve/D");
  tree->Branch("f7PlaQAve", &f7PlaQAve,"f7PlaQAve/D");
  tree->Branch("f7PlaTDiff", &f7PlaTDiff,"f7PlaTDiff/D");
  tree->Branch("f13PlaTAve", &f13PlaTAve,"f13PlaTAve/D");
  tree->Branch("f13PlaQAve", &f13PlaQAve,"f13PlaQAve/D");
  tree->Branch("f13PlaTDiff", &f13PlaTDiff,"f13PlaTDiff/D");
  tree->Branch("fSBVQAve", &fSBVQAve,"fSBVQAve/D");
  
  //F5X from BPC                                                                                                    
  Double_t f5x = nan;
  tree->Branch("f5x",&f5x,"f5x/D");
  
  //ICB                                                 
  Double_t fQICB = nan;
  tree->Branch("fQICB",&fQICB,"fQICB/D");

  //Plastic&IC Gating                                   
  Int_t Plagatebit = 0; Int_t ICgatebit = 0;
  tree->Branch("Plagatebit",&Plagatebit,"Plagatebit/I");
  tree->Branch("ICgatebit",&ICgatebit,"ICgatebit/I");

  //BigRIPS Beam PID                                    
  Double_t tof_f7_f13 = nan;   Double_t beta_beam_f7_f13 = nan;
  Double_t brho_f5 = nan; Double_t beta_beam_f5 = nan; Double_t E_beam_f5 = nan;
  Double_t aoz_beam = nan; Double_t z_beam = nan;
  Int_t B19beambit = 0; Int_t B17beambit = 0; Int_t C20beambit = 0;
  tree->Branch("tof_f7_f13",&tof_f7_f13,"tof_f7_f13/D");
  tree->Branch("beta_beam_f7_f13",&beta_beam_f7_f13,"beta_beam_f7_f13/D");
  tree->Branch("brho_f5",&brho_f5,"brho_f5/D");
  tree->Branch("beta_beam_f5",&beta_beam_f5,"beta_beam_f5/D");
  tree->Branch("E_beam_f5",&E_beam_f5,"E_beam_f5/D");
  tree->Branch("aoz_beam",&aoz_beam,"aoz_beam/D");
  tree->Branch("z_beam",&z_beam,"z_beam/D");
  tree->Branch("B19beambit",&B19beambit,"B19beambit/I");
  tree->Branch("B17beambit",&B17beambit,"B17beambit/I");
  tree->Branch("C20beambit",&C20beambit,"C20beambit/I");

  //Beam Profile at target                                          
  Double_t tof_f13_tgt = nan; Double_t tzero_tgt = nan;
  Double_t beta_beam_tgt = nan;  Double_t E_beam_tgt = nan;
  tree->Branch("tof_f13_tgt",&tof_f13_tgt,"tof_f13_tgt/D");
  tree->Branch("tzero_tgt",&tzero_tgt,"tzero_tgt/D");
  tree->Branch("beta_beam_tgt",&beta_beam_tgt,"beta_beam_tgt/D");
  tree->Branch("E_beam_tgt",&E_beam_tgt,"E_beam_tgt/D");

  //Target X & Y                                                    
  Double_t tgtx = nan; Double_t tgty = nan; Double_t tgta = nan; Double_t tgtb = nan;
  Double_t bdc1x = nan; Double_t bdc2x = nan; Double_t bdc1y = nan; Double_t bdc2y = nan;
  tree->Branch("tgtx",&tgtx,"tgtx/D"); tree->Branch("tgty",&tgty,"tgty/D");
  tree->Branch("tgta",&tgta,"tgta/D"); tree->Branch("tgtb",&tgtb,"tgtb/D");
  tree->Branch("bdc1x",&bdc1x,"bdc1x/D"); tree->Branch("bdc2x",&bdc2x,"bdc2x/D");
  tree->Branch("bdc1y",&bdc1y,"bdc1y/D"); tree->Branch("bdc2y",&bdc2y,"bdc2y/D");
  //Scat A&B                                                        
  Double_t scata = nan; Double_t scatb = nan;
  tree->Branch("scata",&scata,"scata/D"); tree->Branch("scatb",&scatb,"scatb/D");

  //SAMURAI Fragment PID                                            
  Double_t aoz_frag = nan; Double_t z_frag = nan;
  Double_t beta_frag = nan; Double_t tof_frag = nan;
  Double_t fl_frag_sim = nan; Double_t tof_frag_sim = nan;
  Double_t energy_frag_sim = nan; Double_t rigidity_frag_sim = nan;  Double_t momentum_frag_sim = nan;
  Int_t B19fragbit = 0; Int_t B17fragbit = 0; Int_t B15fragbit = 0; Int_t B14fragbit = 0; Int_t B13fragbit = 0;
  Int_t C20fragbit = 0; Int_t C19fragbit = 0;Int_t C18fragbit = 0;
  tree->Branch("aoz_frag",&aoz_frag,"aoz_frag/D");
  tree->Branch("z_frag",&z_frag,"z_frag/D");
  tree->Branch("beta_frag",&beta_frag,"beta_frag/D");
  tree->Branch("tof_frag",&tof_frag,"tof_frag/D");
  tree->Branch("fl_frag_sim",&fl_frag_sim,"fl_frag_sim/D");
  tree->Branch("tof_frag_sim",&tof_frag_sim,"tof_frag_sim/D");
  tree->Branch("energy_frag_sim",&energy_frag_sim,"energy_frag_sim/D");
  tree->Branch("rigidity_frag_sim",&rigidity_frag_sim,"rigidity_frag_sim/D");
  tree->Branch("momentum_frag_sim",&momentum_frag_sim,"momentum_frag_sim/D");
  tree->Branch("B19fragbit",&B19fragbit,"B19fragbit/I");
  tree->Branch("B17fragbit",&B17fragbit,"B17fragbit/I");
  tree->Branch("B15fragbit",&B15fragbit,"B15fragbit/I");
  tree->Branch("B14fragbit",&B14fragbit,"B14fragbit/I");
  tree->Branch("B13fragbit",&B13fragbit,"B13fragbit/I");
  tree->Branch("C20fragbit",&C20fragbit,"C20fragbit/I");
  tree->Branch("C19fragbit",&C19fragbit,"C19fragbit/I");
  tree->Branch("C18fragbit",&C18fragbit,"C18fragbit/I");

  Double_t beta_frag_brho = nan;
  tree->Branch("beta_frag_brho",&beta_frag_brho,"beta_frag_brho/D");

  //FDC1&2 X & Y                                                    
  Double_t fdc1x = nan; Double_t fdc1y = nan; Double_t fdc1a = nan; Double_t fdc1b = nan;
  Double_t fdc2x = nan; Double_t fdc2y = nan; Double_t fdc2a = nan; Double_t fdc2b = nan;
  tree->Branch("fdc1x",&fdc1x,"fdc1x/D"); tree->Branch("fdc1y",&fdc1y,"fdc1y/D");
  tree->Branch("fdc1a",&fdc1a,"fdc1a/D"); tree->Branch("fdc1b",&fdc1b,"fdc1b/D");
  tree->Branch("fdc2x",&fdc2x,"fdc2x/D"); tree->Branch("fdc2y",&fdc2y,"fdc2y/D");
  tree->Branch("fdc2a",&fdc2a,"fdc2a/D"); tree->Branch("fdc2b",&fdc2b,"fdc2b/D");

  //4-Momentum of beam from BDCs&FDC1                               
  Double_t Vb_px = nan; Double_t Vb_py = nan; Double_t Vb_pz = nan; Double_t Vb_E = nan;
  tree->Branch("Vb_px",&Vb_px,"Vb_px/D"); tree->Branch("Vb_py",&Vb_py,"Vb_py/D");
  tree->Branch("Vb_pz",&Vb_pz,"Vb_pz/D"); tree->Branch("Vb_E",&Vb_E,"Vb_E/D");

  //4-Momentum of fragment from BDCs&FDC1                           
  Double_t Vf_px = nan; Double_t Vf_py = nan; Double_t Vf_pz = nan; Double_t Vf_E = nan;
  tree->Branch("Vf_px",&Vf_px,"Vf_px/D"); tree->Branch("Vf_py",&Vf_py,"Vf_py/D");
  tree->Branch("Vf_pz",&Vf_pz,"Vf_pz/D"); tree->Branch("Vf_E",&Vf_E,"Vf_E/D");

  //HODFX from FDC2                                                 
  Double_t hodx = nan; Double_t hody = nan; Double_t windowx = nan; Double_t windowy = nan;
  tree->Branch("hodx",&hodx,"hodx/D");
  tree->Branch("hody",&hody,"hody/D");
  tree->Branch("windowx",&windowx,"windowx/D");
  tree->Branch("windowy",&windowy,"windowy/D");

  //HODF                                                            
  Double_t fHODTAve[16] = {nan}; Double_t fHODQAve[16] = {nan};
  for(Int_t i=0;i<16;i++){
    fHODTAve[i] = nan; fHODQAve[i] = nan;
  }
  Int_t fHODID = -1; Int_t fHODMulti = 0;
  Double_t z_frag_buffer = nan; Double_t tof_frag_buffer = nan;
  tree->Branch("fHODTAve[16]", fHODTAve,"fHODTAve[16]/D");
  tree->Branch("fHODQAve[16]", fHODQAve,"fHODQAve[16]/D");
  tree->Branch("fHODID",&fHODID,"fHODID/I");
  tree->Branch("fHODMulti",&fHODMulti,"fHODMulti/I");
  tree->Branch("z_frag_buffer",&z_frag_buffer,"z_frag_buffer/D");
  tree->Branch("tof_frag_buffer",&tof_frag_buffer,"tof_frag_buffer/D");
  //-----------------------------------------------------------------------------------------------

  //Calibrators-------------------------------------------------------------------------------------
  TArtCalibCoin *coin = new TArtCalibCoin();
  TClonesArray *triggerbit_array = (TClonesArray *)estore -> GetEventInfoArray();
  TArtEventInfo *triggerinfo = (TArtEventInfo *)triggerbit_array -> At(0);

  TArtCalibPlastic *calib_pla = new TArtCalibPlastic();
  TClonesArray * pla_array = (TClonesArray *)sman -> FindDataContainer("BigRIPSPlastic");

  TArtCalibBPCHit  *calib_bpc = new TArtCalibBPCHit();

  TArtCalibIC  *calib_ic  = new TArtCalibIC();
  TClonesArray *ic_array = (TClonesArray *)sman -> FindDataContainer("BigRIPSIC");

  TArtCalibBDC1Hit    *calib_bdc1ht = new TArtCalibBDC1Hit();
  TArtCalibBDC1Track * calib_bdc1tr = new TArtCalibBDC1Track();
  TArtCalibBDC2Hit    *calib_bdc2ht = new TArtCalibBDC2Hit();
  TArtCalibBDC2Track * calib_bdc2tr = new TArtCalibBDC2Track();
  TClonesArray *bdc1tr_array = (TClonesArray *)sman -> FindDataContainer("SAMURAIBDC1Track");
  TClonesArray *bdc2tr_array = (TClonesArray *)sman -> FindDataContainer("SAMURAIBDC2Track");
  //BDC1&2 STC calibration                                                                                          
  TFile *calibhistbdc1 = new TFile("db/dayone0431bdc1.root");
  TFile *calibhistbdc2 = new TFile("db/dayone0431bdc2.root");
  gROOT->cd();
  calib_bdc1tr -> SetTDCWindow(600,800);
  calib_bdc2tr -> SetTDCWindow(600,800);
  for(int i=0;i<8;i++){
    calib_bdc1tr -> SetTDCDistribution((TH1F*)calibhistbdc1 -> Get(Form("htdc%d",i)),i);
    calib_bdc2tr -> SetTDCDistribution((TH1F*)calibhistbdc2 -> Get(Form("htdc%d",i)),i);
  }

  TArtCalibFDC1Hit   * calib_fdc1ht   = new TArtCalibFDC1Hit();
  TArtCalibFDC1Track *calib_fdc1tr = new TArtCalibFDC1Track();
  TArtCalibFDC2Hit   * calib_fdc2ht   = new TArtCalibFDC2Hit();
  TArtCalibFDC2Track *calib_fdc2tr = new TArtCalibFDC2Track();
  TClonesArray * fdc1tr_array = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  TClonesArray * fdc1ht_array = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Hit");
  TClonesArray * fdc2tr_array = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");
  TClonesArray * fdc2ht_array = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Hit");
  //FDC1&2 STC calibration                                                                                          
  TFile *calibhistfdc1 = new TFile("db/dayone0431fdc1.root");
  TFile *calibhistfdc2 = new TFile("db/dayone0431fdc2.root");
  gROOT->cd();
  calib_fdc1tr -> SetTDCWindow(1300,1700);
  calib_fdc2tr -> SetTDCWindow(500,1600);
  for(int i=0;i<14;i++){
    calib_fdc1tr -> SetTDCDistribution((TH1F*)calibhistfdc1 -> Get(Form("htdc%d",i)),i);
    calib_fdc2tr -> SetTDCDistribution((TH1F*)calibhistfdc2 -> Get(Form("htdc%d",i)),i);
  }
  TArtCalibHODPla    *calib_hodpla    = new TArtCalibHODPla();
  //------------------------------------------------------------------------------------------

  //Start loof--------------------------------------------------------------------------------
  TNamed *header = new TNamed("header","");
  file->Add(header);

  stringstream sheader;
  sheader<<endl;
  sheader<<" A="<<A
	 <<" Z="<<Z
	 <<endl;

  int neve = 0;
  while( estore -> GetNextEvent()){
    //while( estore -> GetNextEvent() && neve < 100000){                                       
    if(neve%10000==0) cout << "neve: " << neve << endl;
    //estore -> GetNextEvent();//online                                                        
    //initialize------------------------------------------------------------------------------ 
    fneve = -1;
    ++neve;
    //Simdata---------------------------------------------------------------------------------
    gBeamSimDataArray->clear();
    //----------------------------------------------------------------
    // Coincidence for trigger
    coin->ClearData();
    coin->ReconstructData();
    for (int id=1;id<8;++id)  if (coin->IsChTrue(id)) hcoin->Fill(id);
    if (!coin->IsChTrue(1)) {estore->ClearData();continue;}//only DBS
    for (int id=1;id<8;++id)  if (coin->IsChTrue(id)) hcoin_g->Fill(id);

    //----------------------------------------------------------------
    // Plastics
    calib_pla -> ClearData();
    calib_pla -> ReconstructData();
    calib_bpc -> ClearData();
    calib_bpc -> ReconstructData();
    calib_ic -> ClearData();
    calib_ic -> ReconstructData();

    Plagatebit = 0; ICgatebit = 0;
    if(tgtCbit == 1){
      if(f3PlaTDiff >-0.3&&f3PlaTDiff <2. &&
         f7PlaTDiff > 3. &&f7PlaTDiff <5.7&&
         f13PlaTDiff>-1.5&&f13PlaTDiff<3.2) Plagatebit = 1;
      if(f13PlaTAve-f7PlaTAve-0.736754*(f7PlaTAve-f3PlaTAve)>47.5&&
         f13PlaTAve-f7PlaTAve-0.736754*(f7PlaTAve-f3PlaTAve)<50.5&&
         f3PlaQAve-6.789*fQICB   >120.&&f3PlaQAve-6.789*fQICB   <190.&&
         f7PlaQAve-9.0923*fQICB  >120.&&f7PlaQAve-9.0923*fQICB  <190.&&
         f13PlaQAve-8.62017*fQICB>140.&&f13PlaQAve-8.62017*fQICB<230.) ICgatebit = 1;
    }else if(tgtPbbit == 1){
      if(f3PlaTDiff >-0.2131&&f3PlaTDiff <1.4309&&
         f7PlaTDiff > 3.0987&&f7PlaTDiff <5.7843&&
         f13PlaTDiff>-1.7647&&f13PlaTDiff<3.3225) Plagatebit = 1;
      if(f13PlaTAve-f7PlaTAve-0.783051*(f7PlaTAve-f3PlaTAve)>49.6353&&
         f13PlaTAve-f7PlaTAve-0.783051*(f7PlaTAve-f3PlaTAve)<51.6447&&
         f3PlaQAve-6.54965*fQICB >130.16&&f3PlaQAve-6.54965*fQICB <173.24&&
         f7PlaQAve-8.56988*fQICB >134.94&&f7PlaQAve-8.56988*fQICB <182.06&&
         f13PlaQAve-8.29961*fQICB>144.56&&f13PlaQAve-8.29961*fQICB<223.04) ICgatebit = 1;
    }else if(tgtEmpbit == 1){
      Plagatebit = 1; //No Plagate in Emp tgt                                                             
      if(f13PlaTAve-f7PlaTAve-0.749895*(f7PlaTAve-f3PlaTAve)>48.3645&&
         f13PlaTAve-f7PlaTAve-0.749895*(f7PlaTAve-f3PlaTAve)<50.2755&&
         f3PlaQAve-6.4199*fQICB  >131.775&&f3PlaQAve-6.4199*fQICB  <173.025&&
         f7PlaQAve-8.31573*fQICB >137.09 &&f7PlaQAve-8.31573*fQICB < 183.11&&
         f13PlaQAve-8.33562*fQICB>146.74 &&f13PlaQAve-8.33562*fQICB<221.26) ICgatebit = 1;
    }


    //Reconstruct beam properties>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //initialize-----------------------------------------------------------------------------------
    tof_f7_f13 = nan;
    beta_beam_f7_f13 = nan;
    beta_beam_f5 = nan;
    beta_beam_tgt = nan;
    brho_f5 = nan;
    aoz_beam = nan;
    z_beam = nan;
    tzero_tgt = nan;
    B19beambit = 0; B17beambit = 0; C20beambit = 0;
    //---------------------------------------------------------------------------------------------
    tof_f7_f13 = f13PlaTAve - f7PlaTAve + tofoff_f13_f7;
    beta_beam_f7_f13 = dist_f7f13/(tof_f7_f13*clight);
    brho_f5 = (1.+f5x/f5dispersion)*f5_brho_center;
    beta_beam_f5 = 1.723/100000.*(tof_f7_f13*tof_f7_f13) - 0.009899*tof_f7_f13 + 1.902;//fit with 17B
    aoz_beam = (clight*brho_f5)/(m_u*beta_beam_f5*(1./(sqrt(1-beta_beam_f5*beta_beam_f5))));
    E_beam_f5 = m_u*(-1.+sqrt(1.+96.4853414192065202*pow(brho_f5/aoz_beam,2.)/m_u));//[/u]
    z_beam = Calib_zed_beam(fQICB,beta_beam_f7_f13);


    if(BeamParticleSelect(z_beam,aoz_beam,5.055,0.2124,3.817,0.010990)) B19beambit = 1;
    if(BeamParticleSelect(z_beam,aoz_beam,5.055,0.2124,3.421,0.006571)) B17beambit = 1;
    if(BeamParticleSelect(z_beam,aoz_beam,6.106,0.1721,3.353,0.008337)) C20beambit = 1;




    if ((Plagatebit==0)||(ICgatebit==0)){estore->ClearData();continue;}
    haoqz->Fill(aoz_beam,z_beam);
    if (B17beambit==0) {estore->ClearData();continue;}
    haoqz_g->Fill(AoQbeam,Zbeam);
 
   //-------------------------------------------------------------------
    // HOD
    calibhod->ClearData();
    calibhod->ReconstructData();
    TanaHODPla *pla_max = 0;
    Double_t Zmax=0;
    for (int ipla=0;ipla<hod_array->GetEntries();++ipla){
      TanaHODPla *pla = (TanaHODPla*)hod_array->At(ipla);
      Double_t Ztmp = pla->GetZ();
      if (pla->GetID()==11) pla->SetZ(Ztmp*0.97);
      if (Zmax < pla->GetZ() ) {
	Zmax = pla->GetZ();
	pla_max = pla;
      }
    }
    if (pla_max==0) {estore->ClearData();continue;}
    hidz->Fill(pla_max->GetID(), pla_max->GetZ());
    if (fabs(Zmax-Zgate)>0.5) {estore->ClearData();continue;}
    hidz_g->Fill(pla_max->GetID(), pla_max->GetZ());
  
    //-------------------------------------------------------------------
    bdc1x = bdc1->GetPosX() + prm->FindGeomPara("BDC1_Xoff"); //mm
    bdc1y = bdc1->GetPosY() + prm->FindGeomPara("BDC1_Yoff"); //mm
    bdc2x = bdc2->GetPosX() + prm->FindGeomPara("BDC2_Xoff"); //mm	 
    bdc2y = bdc2->GetPosY() + prm->FindGeomPara("BDC2_Yoff"); //mm	 
    tgtx = recobeam->GetBeamPosX();
    tgty = recobeam->GetBeamPosY();
    tgta = recobeam->GetBeamAngX();
    tgtb = recobeam->GetBeamAngY();
    fdc1x = fdc1->GetPosX() + prm->FindGeomPara("FDC1_Xoff"); //mm
    fdc1y = fdc1->GetPosY() + prm->FindGeomPara("FDC1_Yoff"); //mm
    fdc2x = fdc2->GetPosX() + prm->FindGeomPara("FDC2_Xoff"); //mm
    fdc2y = fdc2->GetPosY() + prm->FindGeomPara("FDC2_Yoff"); //mm
    fdc2a = fdc2->GetAngX() + prm->FindGeomPara("FDC2_DXDZoff"); //mm
    fdc2b = fdc2->GetAngY() + prm->FindGeomPara("FDC2_DYDZoff"); //mm
    Double_t dist_TGTFDC1 = prm->FindGeomPara("FDC1_Zoff")
      - prm->FindGeomPara("TGT_Zoff");
    fdc1a = (fdc1x-tgtx)/dist_TGTFDC1*1000.;//mrad
    fdc1b = (fdc1y-tgty)/dist_TGTFDC1*1000.;

    //-------------------------------------------------------------------

    Double_t Ptot = brho*Z*c_light;  
    Double_t Etot = sqrt(Ptot*Ptot + M*M);

    TVector3 tgt_pos(tgtx,tgty,tgtz);//target position
    Double_t dxdz = fdc1a*0.001;//mrad->rad
    Double_t dydz = fdc1b*0.001;
    Double_t vec_u = sqrt( 1.0 + pow(dxdz,2) + pow(dydz,2) );
    TLorentzVector P(dxdz/vec_u * Ptot, 
		     dydz/vec_u * Ptot, 
		     1./vec_u   * Ptot, 
		     Etot);
    TBeamSimData particle(Z,A,P,tgt_pos);
    gBeamSimDataArray->push_back(particle);

    tree->Fill();
    estore->ClearData();
  }

  header->SetTitle(sheader.str().c_str());

  file->Write();
}
