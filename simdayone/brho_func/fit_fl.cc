const Int_t Nvar=6;
//const Int_t neve_max=10;
const Int_t neve_max=-1;
//Int_t neve=100000;
Int_t neve=10000;
//____________________________________________________________________
void fit_fl()
{
  TString outfilename="root/brho_func/fit_fl_out.root";
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
  //Double_t brho;
  Double_t fl;
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  //tree->SetBranchAddress("brho",&brho);  
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

  Int_t Nana = tree->GetEntries();
  if (neve>0) Nana = neve;
  TArtCounter *counter = TArtCounter::SetCounter(Nana);
  for (int i=0;i<Nana;++i){
    counter->Draw();
    tree->GetEntry(i);

    if (!OK_FDC1 || !OK_FDC2 || !OK_HODF ) continue;

    Double_t x[Nvar]={fdc1x,fdc1a,fdc1y,fdc1b,fdc2x,fdc2a};
    fFit->AddRow(x,fl,0.1);
    fFit->AddTestRow(x,fl,0.1);
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
  Double_t fl;
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  tree->SetBranchAddress("brho",&brho);  
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
  //Double_t brho_fun;
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
  otree->Branch("fl",&fl,"fl/D");
  otree->Branch("fl_fun",&fl_fun,"fl_fun/D");
//  otree->Branch("fl",&fl,"fl/D");
  TH1* hdl = new TH1D("hdl","dl",200,-20,20);
  TH1* hx1dl = new TH2D("hx1dl","fdc1x dl",200,-200,200,  200,-20,20);
  TH1* ha1dl = new TH2D("ha1dl","fdc1a dl",200,-200,200,  200,-20,20);
  TH1* hx2dl = new TH2D("hx2dl","fdc2x dl",200,-1200,1200,200,-20,20);
  TH1* ha2dl = new TH2D("ha2dl","fdc2a dl",200,-400,400,  200,-20,20);
  TH1* hbdl = new TH2D("hbdl","Brho dl",200,4.5,8.5,      200,-20,20);
  TH1* hy1dl = new TH2D("hy1dl","fdc1y dl",200,-200,200,  200,-20,20);
  TH1* hb1dl = new TH2D("hb1dl","fdc1b dl",200,-200,200,  200,-20,20);
  TH1* hy2dl = new TH2D("hy2dl","fdc2y dl",200,-500,500,  200,-20,20);
  TH1* hb2dl = new TH2D("hb2dl","fdc2b dl",200,-200,200,  200,-20,20);


  Int_t Nana = tree->GetEntries();
  if (neve>0) Nana = neve;
  TArtCounter *counter = TArtCounter::SetCounter(Nana);
  for (int i=0;i<Nana;++i){
    counter->Draw();
    tree->GetEntry(i);

    //if (sqrt(fdc1x*fdc1x+fdc1y*fdc1y)>10) continue;
    //if (sqrt(fdc1a*fdc1a+fdc1b*fdc1b)>10) continue;
    //if (fabs(fdc1b)>10 || fabs(fdc1y)>10) continue;
    //if (fabs(fdc1x)>50 || fabs(fdc1a)>50) continue;
    if (!OK_FDC1 || !OK_FDC2 || !OK_HODF ) continue;

    Double_t x[Nvar]={fdc1x,fdc1a,fdc1y,fdc1b,fdc2x,fdc2a};
    fl_fun = MDF(x);

    Double_t dl = fl-fl_fun;
    hdl->Fill(dl);
    hx1dl->Fill(fdc1x,dl);
    ha1dl->Fill(fdc1a,dl);
    hx2dl->Fill(fdc2x,dl);
    ha2dl->Fill(fdc2a,dl);
    hbdl->Fill(brho,dl);

    hy1dl->Fill(fdc1y,dl);
    hb1dl->Fill(fdc1b,dl);
    hy2dl->Fill(fdc2y,dl);
    hb2dl->Fill(fdc2b,dl);

    otree->Fill();
  }
  hdl->Fit("gaus");
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
  fFit->MakeMethod("MDF");

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
