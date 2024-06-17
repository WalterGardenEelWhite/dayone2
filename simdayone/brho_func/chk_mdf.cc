const Int_t Nvar=6;
//const Int_t neve_max=10;
const Int_t neve_max=-1;
Int_t neve=100000;
//____________________________________________________________________
void chk_mdf()
{
  TString outfilename="root/brho_func/chk_mdf.root";
  TFile *ofile = new TFile(outfilename.Data(),"recreate");
  TString infilename = "root/brho_func/analyzed_29f_0000.root";
  CheckFunc(infilename,ofile);
  ofile->Write();
  std::cout<<"\a"<<std::endl;
}
//____________________________________________________________________
void CheckFunc(TString filename, TFile *ofile)
{
  std::cout<<"start CheckFunc ...."<<endl;
  TFile file(filename.Data(),"readonly");
  TTree *tree = (TTree*)file.Get("tree");
  Double_t brho,tof,fl;
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  tree->SetBranchAddress("brho",&brho);  
  tree->SetBranchAddress("tof",&tof);  
  tree->SetBranchAddress("fl",&fl);  
  tree->SetBranchAddress("OK_FDC1",&OK_FDC1);
  tree->SetBranchAddress("fdc1x",&fdc1x);
  tree->SetBranchAddress("fdc1a",&fdc1a);
  tree->SetBranchAddress("fdc1y",&fdc1y);
  tree->SetBranchAddress("fdc1b",&fdc1b);
  tree->SetBranchAddress("OK_FDC2",&OK_FDC2);
  tree->SetBranchAddress("fdc2x",&fdc2x);
  tree->SetBranchAddress("fdc2a",&fdc2a);
  tree->SetBranchAddress("fdc2y",&fdc2y);
  tree->SetBranchAddress("fdc2b",&fdc2b);
  tree->SetBranchAddress("OK_HODF",&OK_HODF);

  ofile->cd();
  TTree* otree = new TTree("tree","fit result");
  Double_t brho_fun;
  Double_t tof_fun;
  Double_t fl_fun;
  otree->Branch("fdc1x",&fdc1x,"fdc1x/D");
  otree->Branch("fdc1a",&fdc1a,"fdc1a/D");
  otree->Branch("fdc1y",&fdc1y,"fdc1y/D");
  otree->Branch("fdc1b",&fdc1b,"fdc1b/D");
  otree->Branch("fdc2x",&fdc2x,"fdc2x/D");
  otree->Branch("fdc2a",&fdc2a,"fdc2a/D");
  otree->Branch("fdc2y",&fdc2y,"fdc2y/D");
  otree->Branch("fdc2b",&fdc2b,"fdc2b/D");
  otree->Branch("brho",&brho,"brho/D");
  otree->Branch("brho_fun",&brho_fun,"brho_fun/D");
  otree->Branch("tof",&tof,"tof/D");
  otree->Branch("tof_fun",&tof_fun,"tof_fun/D");
  otree->Branch("fl",&fl,"fl/D");
  otree->Branch("fl_fun",&fl_fun,"fl_fun/D");
  TH1* hdb = new TH1D("hdb","dBrho/Brho (%)",200,-5,5);

  TH1* hx1db = new TH2D("hx1db","fdc1x dBrho/Brho (%)",200,-200,200,  200,-2,2);
  TH1* ha1db = new TH2D("ha1db","fdc1a dBrho/Brho (%)",200,-200,200,  200,-2,2);
  TH1* hx2db = new TH2D("hx2db","fdc2x dBrho/Brho (%)",200,-1200,1200,200,-2,2);
  TH1* ha2db = new TH2D("ha2db","fdc2a dBrho/Brho (%)",200,-400,400,  200,-2,2);
  TH1* hbdb = new TH2D("hbdb","Brho dBrho/Brho (%)",200,4.5,8.5,      200,-2,2);
  TH1* hy1db = new TH2D("hy1db","fdc1y dBrho/Brho (%)",200,-200,200,  200,-2,2);
  TH1* hb1db = new TH2D("hb1db","fdc1b dBrho/Brho (%)",200,-200,200,  200,-2,2);
  TH1* hy2db = new TH2D("hy2db","fdc2y dBrho/Brho (%)",200,-500,500,  200,-2,2);
  TH1* hb2db = new TH2D("hb2db","fdc2b dBrho/Brho (%)",200,-200,200,  200,-2,2);


  Int_t Nana = tree->GetEntries();
  if (neve>0) Nana = neve;
  TArtCounter *counter = TArtCounter::SetCounter(Nana);
  for (int i=0;i<Nana;++i){
    counter->Draw();
    tree->GetEntry(i);

    if (!OK_FDC1 || !OK_FDC2 || !OK_HODF ) continue;

    Double_t x[Nvar]={fdc1x,fdc1a,fdc1y,fdc1b,fdc2x,fdc2a};
    brho_fun = TArtMDF_s021_Brho::MDF(x);
//    tof_fun = TArtMDF_s021_TOF29F::MDF(x);
    tof_fun = 0;
    fl_fun = TArtMDF_s021_FL::MDF(x);

    Double_t db = (brho-brho_fun)/brho*100.;//%
    hdb->Fill(db);
    hx1db->Fill(fdc1x,db);
    ha1db->Fill(fdc1a,db);
    hx2db->Fill(fdc2x,db);
    ha2db->Fill(fdc2a,db);
    hbdb->Fill(brho,db);

    hy1db->Fill(fdc1y,db);
    hb1db->Fill(fdc1b,db);
    hy2db->Fill(fdc2y,db);
    hb2db->Fill(fdc2b,db);

    otree->Fill();
  }
  hdb->Fit("gaus");
}
