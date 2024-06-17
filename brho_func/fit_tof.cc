const Int_t Nvar=6;
//const Int_t neve_max=10;
const Int_t neve_max=-1;
Int_t neve=100000;
//____________________________________________________________________
void fit_tof()
{
  TString outfilename="root/brho_func/fit_tof_out.root";
  TFile *file = new TFile(outfilename.Data(),"recreate");
  // Global data parameters 
  // make fit object and set parameters on it. 
  TMultiDimFit *fFit = new TMultiDimFit(Nvar, TMultiDimFit::kMonomials,"v");
  //TMultiDimFit *fFit = new TMultiDimFit(Nvar, TMultiDimFit::kChebyshev,"v");

  TString filename = "root/brho_func/analyzed_29f_0000.root";
  SetData(fFit,filename);
  FitData(fFit);
  CheckFunc(filename,file);
  file->Write();

  delete fFit;
  std::cout<<"\a"<<std::endl;
}
//____________________________________________________________________
void SetData(TMultiDimFit *fFit, TString filename)
{
  std::cout<<"start SetData ...."<<std::endl;

  ifstream fin;
  fin.open(filename.Data());
  if (!fin.good()) {
    cerr<<"Cannot open file: "<<filanema.Data()<<endl;
    return;
  }
  TFile file(filename.Data(),"readonly");
  TTree *tree = (TTree*)file.Get("tree");
  Double_t brho;
  Double_t tof;
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  tree->SetBranchAddress("brho",&brho);  
  tree->SetBranchAddress("tof",&tof);  
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

  Int_t Nana = tree->GetEntries();
  if (neve>0) Nana = neve;
  TArtCounter *counter = TArtCounter::SetCounter(Nana);
  for (int i=0;i<Nana;++i){
    counter->Draw();
    tree->GetEntry(i);

    if (sqrt(fdc1a*fdc1a+fdc1b*fdc1b)>20) continue;
    if (brho<7.4 || brho>7.8) continue;
    if (!OK_FDC1 || !OK_FDC2 || !OK_HODF ) continue;

    Double_t x[Nvar]={fdc1x,fdc1a,fdc1y,fdc1b,fdc2x,fdc2a};
    fFit->AddRow(x,tof,0.1);
    fFit->AddTestRow(x,tof,0.1);
  }
  return;
}
//____________________________________________________________________
void CheckFunc(TString filename, TFile *ofile)
{
  std::cout<<"start CheckFunc ...."<<endl;
  gROOT->ProcessLine(".L MDF.C");

  TFile file(filename.Data(),"readonly");
  TTree *tree = (TTree*)file.Get("tree");
  Double_t brho;
  Double_t tof;
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  tree->SetBranchAddress("brho",&brho);  
  tree->SetBranchAddress("tof",&tof);  
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
  //Double_t brho_fun;
  Double_t tof_fun;
  otree->Branch("fdc1x",&fdc1x,"fdc1x/D");
  otree->Branch("fdc1a",&fdc1a,"fdc1a/D");
  otree->Branch("fdc1y",&fdc1y,"fdc1y/D");
  otree->Branch("fdc1b",&fdc1b,"fdc1b/D");
  otree->Branch("fdc2x",&fdc2x,"fdc2x/D");
  otree->Branch("fdc2a",&fdc2a,"fdc2a/D");
  otree->Branch("fdc2y",&fdc2y,"fdc2y/D");
  otree->Branch("fdc2b",&fdc2b,"fdc2b/D");
  otree->Branch("brho",&brho,"brho/D");
  otree->Branch("tof",&tof,"tof/D");
  otree->Branch("tof_fun",&tof_fun,"tof_fun/D");
//  otree->Branch("fl",&fl,"fl/D");
  TH1* hdt = new TH1D("hdt","dt",200,-1,1);
  TH1* hx1dt = new TH2D("hx1dt","fdc1x dt",200,-200,200,  200,-1,1);
  TH1* ha1dt = new TH2D("ha1dt","fdc1a dt",200,-200,200,  200,-1,1);
  TH1* hx2dt = new TH2D("hx2dt","fdc2x dt",200,-1200,1200,200,-1,1);
  TH1* ha2dt = new TH2D("ha2dt","fdc2a dt",200,-400,400,  200,-1,1);
  TH1* hbdt = new TH2D("hbdt","Brho dt",200,4.5,8.5,      200,-1,1);
  TH1* hy1dt = new TH2D("hy1dt","fdc1y dt",200,-200,200,  200,-1,1);
  TH1* hb1dt = new TH2D("hb1dt","fdc1b dt",200,-200,200,  200,-1,1);
  TH1* hy2dt = new TH2D("hy2dt","fdc2y dt",200,-500,500,  200,-1,1);
  TH1* hb2dt = new TH2D("hb2dt","fdc2b dt",200,-200,200,  200,-1,1);


  Int_t Nana = tree->GetEntries();
  if (neve>0) Nana = neve;
  TArtCounter *counter = TArtCounter::SetCounter(Nana);
  for (int i=0;i<Nana;++i){
    counter->Draw();
    tree->GetEntry(i);

    //if (sqrt(fdc1x*fdc1x+fdc1y*fdc1y)>10) continue;
    if (sqrt(fdc1a*fdc1a+fdc1b*fdc1b)>20) continue;
    if (brho<7.4 || brho>7.8) continue;
    //if (fabs(fdc1b)>10 || fabs(fdc1y)>10) continue;
    //if (fabs(fdc1x)>50 || fabs(fdc1a)>50) continue;
    if (!OK_FDC1 || !OK_FDC2 || !OK_HODF ) continue;

    Double_t x[Nvar]={fdc1x,fdc1a,fdc1y,fdc1b,fdc2x,fdc2a};
    tof_fun = MDF(x);

    Double_t dt = tof-tof_fun;
    hdt->Fill(dt);
    hx1dt->Fill(fdc1x,dt);
    ha1dt->Fill(fdc1a,dt);
    hx2dt->Fill(fdc2x,dt);
    ha2dt->Fill(fdc2a,dt);
    hbdt->Fill(brho,dt);

    hy1dt->Fill(fdc1y,dt);
    hb1dt->Fill(fdc1b,dt);
    hy2dt->Fill(fdc2y,dt);
    hb2dt->Fill(fdc2b,dt);

    otree->Fill();
  }
  hdt->Fit("gaus");
}
//____________________________________________________________________
Int_t FitData(TMultiDimFit *fFit)
{
  //Int_t mPowers[Nvar]   = {6,6,6,6};
  Int_t mPowers[Nvar]   = {6,6,4,4,6,6};
  fFit->SetMaxPowers(mPowers);
  //fFit->SetMinRelativeError(1);
  fFit->SetMinRelativeError(0.000000001);
  
  fFit->SetMaxFunctions(100000);
  fFit->SetMaxStudy(10000000);
  fFit->SetMaxTerms(1000);
  fFit->SetPowerLimit(1);
  fFit->SetMinAngle(10);
  fFit->SetMaxAngle(10);
  //
  //fFit->SetMinRelativeError(.005);
  // SumSquare 4.488e+06
  // SumSqRes ~ 0.1715 for 100000 events for C setting

  // Print out the start parameters
  std::cout<<"... print out the start parameters"<<std::endl;
  fFit->Print("fcvp");


  // Print out the statistics
  std::cout<<"... print out the statistics"<<std::endl;
  fFit->Print("s");

  // Book histograms 
  fFit->MakeHistograms();

  // Find the parameterization 
  std::cout<<"... start FindParameterization"<<std::endl;
  fFit->FindParameterization();

  // Print coefficents 
  fFit->Print("rc");

  // Test the parameterizatio and coefficents using the test sample. 
  std::cout<<"... start fit"<<std::endl;
  //fFit->Fit("M");// probably better error estimation
  fFit->Fit();

  // Print result 
  fFit->Print("fc");

  // Write code to file 
  std::cout<<"... start MakeCode"<<std::endl;
  fFit->MakeCode();
  fFit->MakeMethod("TArtMDF_s021_TOF29F");

  // Write histograms to disk, and close file 
  //fOutput->Write();
  //fOutput->Close();
  
//  // Compare results with reference run
//  Int_t compare = CompareResults(fFit);
//  if (!compare) {
//     printf("\nmultidimfit ..............................................  OK\n");
//  } else {
//     printf("\nmultidimfit ..............................................  fails case %d\n",compare);
//  }

  // We're done 
//  return compare;
  return 0;
}
