void chksim_func()
{
  TFile *file = new TFile("root/brho_func/analyzed_29f_0000.root","readonly");
  TTree* tree = (TTree*)file->Get("tree");
  tree->AddFriend("tree_input","root/brho_func/input_A29Z9.root");

  Double_t fdc1x,fdc1y;
  Double_t fdc1a,fdc1b;
  Double_t fdc2x,fdc2y;
  Double_t fdc2a,fdc2b;
  Double_t brho;
  Double_t tof;
  Double_t brho_func;
  Bool_t OK_FDC2;
  tree->SetBranchAddress("brho",&brho);
  tree->SetBranchAddress("tof",&tof);
  tree->SetBranchAddress("fdc1x",&fdc1x);
  tree->SetBranchAddress("fdc1a",&fdc1a);
  tree->SetBranchAddress("fdc1y",&fdc1y);
  tree->SetBranchAddress("fdc1b",&fdc1b);
  tree->SetBranchAddress("fdc2x",&fdc2x);
  tree->SetBranchAddress("fdc2a",&fdc2a);
  tree->SetBranchAddress("fdc2y",&fdc2y);
  tree->SetBranchAddress("fdc2b",&fdc2b);
  tree->SetBranchAddress("OK_FDC2",&OK_FDC2);

  TH1* hfdc1xy = new TH2D("hfdc1xy","fdc1 XY",200,-200,200, 200,-200,200);
  TH1* hfdc1xa = new TH2D("hfdc1xa","fdc1 XA",200,-200,200, 200,-100,100);
  TH1* hfdc1yb = new TH2D("hfdc1yb","fdc1 YB",200,-200,200, 200,-100,100);

  TH1* hfdc2xy = new TH2D("hfdc2xy","fdc2 XY",200,-2000,2000, 200,-1000,1000);
  TH1* hbdb = new TH2D("hbdb","brho dbrho/brho (%)",200,4,9, 200,-1,1);

  TH1* hfdc2xb = new TH2D("hfdc2xb","fdc2x brho (Tm)",200,-1500,1500,200,4,9);
  TH1* hfdc2xfl = new TH2D("hfdc2xfl","fdc2x FL",200,-1500,1500, 200,8500,10000);

  Double_t v[6];
  for (int ieve=0;ieve<tree->GetEntries();++ieve){
    tree->GetEntry(ieve);

    hfdc1xy->Fill(fdc1x,fdc1y);
    hfdc1xa->Fill(fdc1x,fdc1a);
    hfdc1yb->Fill(fdc1y,fdc1b);
    hfdc2xy->Fill(fdc2x,fdc2y);
    v[0] = fdc1x;
    v[1] = fdc1a;
    v[2] = fdc1y;
    v[3] = fdc1b;
    v[4] = fdc2x;
    v[5] = fdc2a;
    brho_func = TArtMDF_s021_Brho::MDF(v);
    Double_t fl_func = TArtMDF_s021_FL::MDF(v);
    Double_t db = brho-brho_func;

    if (OK_FDC2) {
      hbdb->Fill(brho,db/brho*100.);
      hfdc2xb->Fill(fdc2x,brho);
      hfdc2xfl->Fill(fdc2x,fl_func);
    }
  }

}
