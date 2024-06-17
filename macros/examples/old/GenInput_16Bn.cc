// HOW TO USE
// root [0] .L macros/examples/GenerateInputTree_PhaseSpaceDecay.cc+
// root [1] GenerateInputTree_PhaseSpaceDecay()

#include "TBeamSimData.hh"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TGenPhaseSpace.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <iostream>

Double_t Erel_in_max = 10.;
Int_t simnum = 10000;

void GenInput_16Bn(){

  //Parameteres------------------------------------------------------------------------------------------------

  const Int_t Abeam = 16;
  const Int_t Afrag = 15;
  const Int_t Zfrag = 5;
  const Int_t Ndecay = Abeam - Afrag + 1;

  const Int_t    Z[Ndecay]={Zfrag,0,0};
  const Int_t    A[Ndecay]={Afrag,1,1};

  const Double_t m_u = 931.4940954;//[MeV/c^2]atomic mass unit  
  const Double_t m_b19 = 19.*m_u + 59.8;
  const Double_t m_b17 = 17.*m_u + 43.72;
  const Double_t m_b15 = 15.*m_u + 28.957;
  const Double_t m_b14 = 14.*m_u + 23.664;
  const Double_t m_b13 = 13.*m_u + 16.561;

  const Double_t m_n = 939.565413; //[MeV]
  const Double_t clight = 299.792458; //[mm/nsec]
  const Double_t Masses[Ndecay] = {m_b15, m_n, m_n};

  // Init Phase Space Decay-------------------------------------------------------------------------------------

  TFile *file = new TFile("./root/Gen/16BnInp.root","recreate");
  TTree *tree = new TTree("tree","Input tree for simtrace");
  gBeamSimDataArray = new TBeamSimDataArray();
  tree->Branch("TBeamSimData", gBeamSimDataArray);

  TFile *fileparam = new TFile("./root/Gen/16BInpParam.root","recreate");
  
  TTree *treeinp = new TTree("tree","Input tree for simtrace");
  Double_t nan = TMath::QuietNaN();
  Double_t scat_ang = nan;
  Double_t scat_deg = nan;
  Double_t scat_gen = nan;
  Double_t Erel_in = nan;
  Double_t randX = nan; Double_t  randY = nan;
  Double_t randA = nan; Double_t  randB = nan;
  Double_t randbeta = nan; 
  Double_t Bgen_px = nan; Double_t Bgen_py = nan; Double_t Bgen_pz = nan; Double_t Bgen_E = nan;
  Double_t Fgen_px = nan; Double_t Fgen_py = nan; Double_t Fgen_pz = nan; Double_t Fgen_E = nan;
  Double_t N1gen_px = nan; Double_t N1gen_py = nan; Double_t N1gen_pz = nan; Double_t N1gen_E = nan;
  //Double_t N2gen_px = nan; Double_t N2gen_py = nan; Double_t N2gen_pz = nan; Double_t N2gen_E = nan;
  Double_t Erel_gen = nan;
  treeinp->Branch("scat_ang", &scat_ang, "scat_ang/D");
  treeinp->Branch("scat_deg", &scat_deg, "scat_deg/D");
  treeinp->Branch("scat_gen", &scat_gen, "scat_gen/D");
  treeinp->Branch("Erel_in", &Erel_in, "Erel_in/D");
  treeinp->Branch("randX", &randX, "randX/D");
  treeinp->Branch("randY", &randY, "randY/D");
  treeinp->Branch("randA", &randA, "randA/D");
  treeinp->Branch("randB", &randB, "randB/D");
  treeinp->Branch("randbeta", &randbeta, "randbeta/D");
  treeinp->Branch("Bgen_px", &Bgen_px, "Bgen_px/D");
  treeinp->Branch("Bgen_py", &Bgen_py, "Bgen_py/D");
  treeinp->Branch("Bgen_pz", &Bgen_pz, "Bgen_pz/D");
  treeinp->Branch("Bgen_E", &Bgen_E, "Bgen_E/D");
  treeinp->Branch("Fgen_px", &Fgen_px, "Fgen_px/D");
  treeinp->Branch("Fgen_py", &Fgen_py, "Fgen_py/D");
  treeinp->Branch("Fgen_pz", &Fgen_pz, "Fgen_pz/D");
  treeinp->Branch("Fgen_E", &Fgen_E, "Fgen_E/D");
  treeinp->Branch("N1gen_px", &N1gen_px, "N1gen_px/D");
  treeinp->Branch("N1gen_py", &N1gen_py, "N1gen_py/D");
  treeinp->Branch("N1gen_pz", &N1gen_pz, "N1gen_pz/D");
  treeinp->Branch("N1gen_E", &N1gen_E, "N1gen_E/D");
  //treeinp->Branch("N2gen_px", &N2gen_px, "N2gen_px/D");
  //treeinp->Branch("N2gen_py", &N2gen_py, "N2gen_py/D");
  //treeinp->Branch("N2gen_pz", &N2gen_pz, "N2gen_pz/D");
  //treeinp->Branch("N2gen_E", &N2gen_E, "N2gen_E/D");
  treeinp->Branch("Erel_gen", &Erel_gen, "Erel_gen/D");
 
  // Get Experimental Data----------------------------------------------------------------------------------
  //TFile *exp = new TFile("./macros/ReadTarget.root");
  //TH2* htgtxy = (TH2*)exp->Get("hPbtgtxy");
  //TH2* htgtab = (TH2*)exp->Get("hPbtgtab");
  //TH1* hbeta =  (TH1*)exp->Get("hPbbeta");
  //--------------------------------------------------------------------------------------------------------

  // For Check----------------------------------------------------------------------------------------------
  //TH1* htgtxyg = new TH2D("htgtxyg","Target XY Ramdom",100,-50,50,100,-50,50);
  //TH1* htgtabg = new TH2D("htgtabg","Target AB Random",100,-0.05,0.05,100,-0.05,0.05);
  //TH1* hbetag = new TH1D("hbetag","17B Beam Beta",100,0.61,0.65);

  TH1* herel_in = new TH1D("herel_in","Erel Input",200,0,10);
  TH1* hscat_in_d = new TH1D("hscat_in_d","Scattering Angle Input [degree]",200,0,2);
  TH1* hscat_in_r = new TH1D("hscat_in_r","Scattering Angle Input [rad]",200,0,0.035);
  
  TH1* herel_gen = new TH1D("herel_gen","Erel generated",200,0,10);
  TH1* hxypos = new TH2D("hxypos","XY pos (mm)",100,-50,50, 100,-50,50);
  TH1* hzxpos = new TH2D("hzxpos","ZX pos (mm)",100,-50-3872.23,50-3872.23, 100,-50,50);
  TH1* hxyang = new TH2D("hxyang","XY ang (mrad)",100,-50,50, 100,-50,50);

  TH2* h1 = new  TH2D("h1","Erel_in : Scat_in",200,0,10,200,0,0.03);
  TH2* h2 = new  TH2D("h2","Erel_in : Erel_gen",200,0,10,200,0,10);
  TH2* h3 = new  TH2D("h3","sact_in : scat_gen",200,0,0.03,200,0,0.03);

  
  TRandom3 rand;
  
  for (int i=0;i<simnum;++i){
    if(!(i%1000))std::cout << "\r event "<< i << std::flush;
    
    gBeamSimDataArray->clear();

    //Erel generation
    Erel_in = gRandom->Uniform(0,Erel_in_max);
    herel_in->Fill(Erel_in);
    //scat_deg = gRandom->Uniform(0,2);//Degree
    //scat_ang = scat_deg/(180./TMath::Pi());//rad
     
    scat_ang = 0.001*gRandom->Uniform(0,5);

    hscat_in_d->Fill(scat_deg);
    hscat_in_r->Fill(scat_ang);

    
    Double_t Ein = 270.;//[MeV/u] //m_b16/sqrt(1-beta*beta) - m_b17;
    Double_t Mbeam = Erel_in;

    for (Int_t i=0;i<Ndecay;++i) Mbeam+=Masses[i];

    Double_t Tbeam = Ein;
    Double_t Ebeam = Mbeam + Tbeam;
    Double_t Pbeam_abs = sqrt(Tbeam*(Tbeam+2.0*Mbeam));
    cout << Pbeam_abs << " " << Ebeam << endl;
    
    TGenPhaseSpace Decay;

    TLorentzVector Pbeam(0, 0, Pbeam_abs, Ebeam);

    // position spread: tgtx, tgty for XY, +-1.5mm for Z
    //htgtxy->GetRandom2(randX, randY);

    //htgtxyg->Fill(randX, randY);

    //printf("Random Number %d: X = %.2f, Y = %.2f\n", i+1, randX, randY);
    //printf("Random Number %d: A = %.5f, B = %.5f\n", i+1, randA, randB);
    Double_t Zpos_target = -3872.23;
    TVector3 pos_vec(randX, randY, gRandom->Uniform(-1.5,1.5)+Zpos_target); 

    //Scattering angle
    Double_t phi_ang = gRandom->Uniform(0 , 2*TMath::Pi());

    Pbeam.RotateY(scat_ang);
    Pbeam.RotateZ(phi_ang);
  
    // angular spread xy: tgta, tgtb [rad]
    //    htgtab->GetRandom2(randA, randB);
    //htgtabg->Fill(randA, randB);
    Double_t xang = gRandom->Gaus(0,0,03);
    Double_t yang = gRandom->Gaus(0,0.03);
    
    Pbeam.RotateY(xang);
    Pbeam.RotateX(-yang);

    //Input for the scattering angle analysis
    Bgen_px = nan; Bgen_py = nan; Bgen_pz = nan; Bgen_E = nan;
    TLorentzVector VBeam(0,0,Pbeam_abs,Ebeam);
    VBeam.RotateY(xang);
    VBeam.RotateX(-yang);
    Bgen_px = VBeam.Px(); Bgen_py = VBeam.Py(); Bgen_pz = VBeam.Pz(); Bgen_E = VBeam.E();
    //////////////////////////////////
 
    //Set phase space decay
    Decay.SetDecay(Pbeam,Ndecay,Masses);

    // Generate decayed events
    double weight = Decay.Generate();
    while (gRandom->Uniform()>weight) weight = Decay.Generate();

    TLorentzVector P[Ndecay];
    for (int i=0;i<Ndecay;++i){
      P[i] = *(Decay.GetDecay(i));
      TBeamSimData particle(Z[i],A[i],P[i],pos_vec);
      if (Z[i]==0) particle.fParticleName="neutron";
      particle.fPrimaryParticleID = i;
      gBeamSimDataArray->push_back(particle);
    }
    
    // checking input
    TLorentzVector Ptot(0,0,0,0);
    Double_t Mtot=0;
    for (Int_t i=0;i<Ndecay;i++) {
      Ptot += P[i];
      Mtot += P[i].M();
    }

    //Input for the scattering angle analysis
    Fgen_px = nan; Fgen_py = nan; Fgen_pz = nan; Fgen_E = nan;
    N1gen_px = nan; N1gen_py = nan; N1gen_pz = nan; N1gen_E = nan;
    //N2gen_px = nan; N2gen_py = nan; N2gen_pz = nan; N2gen_E = nan;
    Erel_gen = nan;

    Fgen_px = P[0].Px(); Fgen_py = P[0].Py(); Fgen_pz = P[0].Pz(); Fgen_E = P[0].E();
    N1gen_px = P[1].Px(); N1gen_py = P[1].Py(); N1gen_pz = P[1].Pz(); N1gen_E = P[1].E();
    //N2gen_px = P[2].Px(); N2gen_py = P[2].Py(); N2gen_pz = P[2].Pz(); N2gen_E = P[2].E();

    TVector3 beam1(Bgen_px,Bgen_py,Bgen_pz);
    TVector3 beam2(Ptot.Px(),Ptot.Py(),Ptot.Pz());
    //cout << scat_ang << " " << beam1.Angle(beam2) << endl;

    scat_gen = beam1.Angle(beam2);

    Erel_gen = Ptot.M() - Mtot;
    herel_gen->Fill(Erel_gen);
    hxypos->Fill(pos_vec.x(), pos_vec.y());
    hzxpos->Fill(pos_vec.z(), pos_vec.x());
    hxyang->Fill(Ptot.Px()/Ptot.Pz()*1000., Ptot.Py()/Ptot.Pz()*1000.);

    h1->Fill(Erel_in,scat_ang);
    h2->Fill(Erel_in,Erel_gen);
    h3->Fill(scat_ang,scat_gen);


    // Fill tree
    tree->Fill();
    treeinp->Fill();

  }
  file->Write();
  file->Close();
  fileparam->Write();
  fileparam->Close();
}
