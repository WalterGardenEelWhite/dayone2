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

#include <iostream>
#include <sstream>
#include <fstream>

Int_t simgen = 10000000;

void GenInput()
{
  const double m_u = 931.494043;//MeV
  const double c_light=299.792458;//mm/ns
  
  //Double_t tgtz = -3872.23;//[mm]
  Double_t tgtz = -3871.54; 
  Double_t fdc1z = tgtz+983.38+336./2.;//[mm]

  //17B
  Double_t A = 17;
  Double_t Z = 5;
  Double_t M = 17.*m_u+43.72;

  //-----------------------------------------
  TFile *file = new TFile("root/brho_func/input_17B.root","recreate");
  TTree *tree = new TTree("tree_input","Input tree for trajectories");
  gBeamSimDataArray = new TBeamSimDataArray();
  tree->Branch("TBeamSimData", gBeamSimDataArray);
  Double_t tgtx,tgty,tgta,tgtb;
  Double_t fdc1x,fdc1y;
  Double_t fdc1a,fdc1b;
  Double_t brho;
  tree->Branch("brho",&brho,"brho/D");
  tree->Branch("tgtx",&tgtx,"tgtx/D");
  tree->Branch("tgty",&tgty,"tgty/D");
  tree->Branch("tgta",&tgta,"tgta/D");
  tree->Branch("tgtb",&tgtb,"tgtb/D");
  tree->Branch("fdc1x",&fdc1x,"fdc1x/D");
  tree->Branch("fdc1a",&fdc1a,"fdc1a/D");
  tree->Branch("fdc1y",&fdc1y,"fdc1y/D");
  tree->Branch("fdc1b",&fdc1b,"fdc1b/D");

  TNamed *header = new TNamed("header","");
  file->Add(header);

  stringstream sheader;
  sheader<<endl;
  sheader<<" A="<<A
	 <<" Z="<<Z
	 <<endl;
  
  Double_t xmin=-40,xmax=40;
  Double_t amin=-50,amax=50;
  Double_t ymin=-40,ymax=40;
  Double_t bmin=-50,bmax=50;
  Double_t brhomin=6.5,brhomax=8.5;


  for (int ieve=0;ieve<simgen;++ieve){
    tgtx = gRandom->Uniform(xmin,xmax);
    tgta = gRandom->Uniform(amin,amax);
    tgty = gRandom->Uniform(ymin,ymax);
    tgtb = gRandom->Uniform(bmin,bmax);

    brho = gRandom->Uniform(brhomin,brhomax);
    
    fdc1a = tgta;
    fdc1b = tgtb;
    fdc1x = tgtx+(fdc1z-tgtz)*tan(fdc1a*0.001);
    fdc1y = tgty+(fdc1z-tgtz)*tan(fdc1b*0.001);


    gBeamSimDataArray->clear();

    Double_t Ptot = brho*Z*c_light;
    Double_t Etot = sqrt(Ptot*Ptot + M*M);

    TVector3 tgt_pos(tgtx,tgty,tgtz);
    Double_t dxdz = tgta*0.001;//mrad->rad
    Double_t dydz = tgtb*0.001;
    Double_t vec_u = sqrt( 1.0 + pow(dxdz,2) + pow(dydz,2) );
    TLorentzVector P(dxdz/vec_u * Ptot, 
		     dydz/vec_u * Ptot, 
		     1./vec_u   * Ptot, 
		     Etot);
    TBeamSimData particle(Z,A,P,tgt_pos);

    gBeamSimDataArray->push_back(particle);

    tree->Fill();
  }

  header->SetTitle(sheader.str().c_str());

  file->Write();
}
