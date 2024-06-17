const Int_t Nvar=6;
//const Int_t neve_max=10;
const Int_t neve_max=-1;
//Int_t neve=100000;
Int_t neve=1000;
//____________________________________________________________________
void fit_brho()
{
  TString outfilename="root/brho_func/fit_brho_out.root";
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
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  tree->SetBranchAddress("brho",&brho);  
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

    //if (fabs(brho-7)>0.5) continue;
    //if (sqrt(fdc1x*fdc1x+fdc1y*fdc1y)>10) continue;
    //if (sqrt(fdc1a*fdc1a+fdc1b*fdc1b)>10) continue;
    //if (fabs(fdc1b)>10 || fabs(fdc1y)>10) continue;
    //if (fabs(fdc1x)>50 || fabs(fdc1a)>50) continue;

    if (!OK_FDC1 || !OK_FDC2 || !OK_HODF ) continue;

    //Double_t x[Nvar]={fdc1x,fdc1a,fdc2x,fdc2a};
    Double_t x[Nvar]={fdc1x,fdc1a,fdc1y,fdc1b,fdc2x,fdc2a};
    fFit->AddRow(x,brho,0.1);
    fFit->AddTestRow(x,brho,0.1);
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
  Double_t fdc1x,fdc1y,fdc1a,fdc1b;
  Double_t fdc2x,fdc2y,fdc2a,fdc2b;
  Bool_t OK_FDC1;
  Bool_t OK_FDC2;
  Bool_t OK_HODF;
  tree->SetBranchAddress("brho",&brho);  
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
//  otree->Branch("tof",&tof,"tof/D");
//  otree->Branch("fl",&fl,"fl/D");
  TH1* hdb = new TH1D("hdb","fdBrho/Brho (%)",200,-5,5);

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

    //if (fabs(brho-7)>0.5) continue;
    //if (sqrt(fdc1x*fdc1x+fdc1y*fdc1y)>10) continue;
    //if (sqrt(fdc1a*fdc1a+fdc1b*fdc1b)>10) continue;
    //if (fabs(fdc1b)>10 || fabs(fdc1y)>10) continue;
    //if (fabs(fdc1x)>50 || fabs(fdc1a)>50) continue;
    if (!OK_FDC1 || !OK_FDC2 || !OK_HODF ) continue;

    //Double_t x[Nvar]={fdc1x,fdc1a,fdc2x,fdc2a};
    Double_t x[Nvar]={fdc1x,fdc1a,fdc1y,fdc1b,fdc2x,fdc2a};
    brho_fun = MDF(x);

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
  fFit->MakeMethod("TArtMDF_s021_Brho");

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
