void chksim(Int_t nRun=466)
{
  TFile *file = new TFile(Form("root/brho_func/input_run%04d.root",nRun),"readonly");
  TTree* tree = (TTree*)file->Get("tree_input");
  tree->AddFriend("tree",Form("root/brho_func/analyzed_%04d.root",nRun));

  Double_t bdc1x,bdc1y;
  Double_t bdc2x,bdc2y;
  Double_t tgtx,tgty,tgta,tgtb;
  Double_t fdc1x,fdc1y;
  Double_t fdc1a,fdc1b;
  Double_t fdc2x,fdc2y;
  Double_t fdc2a,fdc2b;
  Double_t tof713;
  Double_t brho;
  tree->SetBranchAddress("tof713",&tof713);
  tree->SetBranchAddress("brho",&brho);
  tree->SetBranchAddress("bdc1x",&bdc1x);
  tree->SetBranchAddress("bdc1y",&bdc1y);
  tree->SetBranchAddress("bdc2x",&bdc2x);
  tree->SetBranchAddress("bdc2y",&bdc2y);
  tree->SetBranchAddress("tgtx",&tgtx);
  tree->SetBranchAddress("tgty",&tgty);
  tree->SetBranchAddress("tgta",&tgta);
  tree->SetBranchAddress("tgtb",&tgtb);
  tree->SetBranchAddress("fdc1x",&fdc1x);
  tree->SetBranchAddress("fdc1a",&fdc1a);
  tree->SetBranchAddress("fdc1y",&fdc1y);
  tree->SetBranchAddress("fdc1b",&fdc1b);
  tree->SetBranchAddress("fdc2x",&fdc2x);
  tree->SetBranchAddress("fdc2a",&fdc2a);
  tree->SetBranchAddress("fdc2y",&fdc2y);
  tree->SetBranchAddress("fdc2b",&fdc2b);
  Double_t fdc1xsim,fdc1ysim,fdc1asim,fdc1bsim;
  Double_t fdc2xsim,fdc2ysim,fdc2asim,fdc2bsim;
  tree->SetBranchAddress("fdc1xsim",&fdc1xsim);
  tree->SetBranchAddress("fdc1asim",&fdc1asim);
  tree->SetBranchAddress("fdc1ysim",&fdc1ysim);
  tree->SetBranchAddress("fdc1bsim",&fdc1bsim);
  tree->SetBranchAddress("fdc2xsim",&fdc2xsim);
  tree->SetBranchAddress("fdc2asim",&fdc2asim);
  tree->SetBranchAddress("fdc2ysim",&fdc2ysim);
  tree->SetBranchAddress("fdc2bsim",&fdc2bsim);

  TH1* hfdc2xy = new TH2D("hfdc2xy","fdc2 XY",200,-200,600, 200,-300,300);
  TH1* hfdc2xysim = new TH2D("hfdc2xysim","fdc2 XY sim",200,-200,600, 200,-300,300);

  TH1* hfdc2xx = new TH2D("hfdc2xx","fdc2 x xsim",200,-100,600,200,-100,600);
  TH1* hfdc2aa = new TH2D("hfdc2aa","fdc2 a asim",200, 50,400,200, 50,400);
  TH1* hfdc2yy = new TH2D("hfdc2yy","fdc2 y ysim",200,-200,200,200,-200,200);
  TH1* hfdc2bb = new TH2D("hfdc2bb","fdc2 b bsim",200,-100,100,200,-100,100);

  TH1* hfdc2dxda = new TH2D("hfdc2dxda","fdc2 dx da",200,-20,20, 200,-20,20);
  TH1* hfdc2dydb = new TH2D("hfdc2dydb","fdc2 dy db",200,-20,20, 200,-20,20);

  TH1* hfdc1dx = new TH1D("hfdc1dx","FDC1 dx(=meas-sim)",200,-40,40);
  TH1* hfdc1da = new TH1D("hfdc1da","FDC1 da(=meas-sim)",200,-40,40);
  TH1* hfdc1dy = new TH1D("hfdc1dy","FDC1 dy(=meas-sim)",200,-40,40);
  TH1* hfdc1db = new TH1D("hfdc1db","FDC1 db(=meas-sim)",200,-40,40);

  TH1* hfdc2dx = new TH1D("hfdc2dx","FDC2 dx(=meas-sim)",200,-40,40);
  TH1* hfdc2da = new TH1D("hfdc2da","FDC2 da(=meas-sim)",200,-40,40);
  TH1* hfdc2dy = new TH1D("hfdc2dy","FDC2 dy(=meas-sim)",200,-40,40);
  TH1* hfdc2db = new TH1D("hfdc2db","FDC2 db(=meas-sim)",200,-40,40);

  TH1* hbrhodx = new TH2D("hbrhodx","brho fdc2 dx",200,6.3,8, 200,-20,20);
  TH1* hbrhoda = new TH2D("hbrhoda","brho fdc2 da",200,6.3,8, 200,-20,20);
  TH1* hbrhody = new TH2D("hbrhody","brho fdc2 dy",200,6.3,8, 200,-20,20);
  TH1* hbrhodb = new TH2D("hbrhodb","brho fdc2 db",200,6.3,8, 200,-20,20);

  for (int ieve=0;ieve<tree->GetEntries();++ieve){
    tree->GetEntry(ieve);

    hfdc2xy->Fill(fdc2x,fdc2y);
    hfdc2xysim->Fill(fdc2xsim,fdc2ysim);

    hfdc2xx->Fill(fdc2x,fdc2xsim);
    hfdc2aa->Fill(fdc2a,fdc2asim);
    hfdc2yy->Fill(fdc2y,fdc2ysim);
    hfdc2bb->Fill(fdc2b,fdc2bsim);

    Double_t dx = fdc2x-fdc2xsim;
    Double_t da = fdc2a-fdc2asim;
    Double_t dy = fdc2y-fdc2ysim;
    Double_t db = fdc2b-fdc2bsim;

    hfdc2dxda->Fill(dx,da);
    hfdc2dydb->Fill(dy,db);

    hfdc1dx->Fill(fdc1x-fdc1xsim);
    hfdc1da->Fill(fdc1a-fdc1asim);
    hfdc1dy->Fill(fdc1y-fdc1ysim);
    hfdc1db->Fill(fdc1b-fdc1bsim);

    hfdc2dx->Fill(dx);
    hfdc2da->Fill(da);
    hfdc2dy->Fill(dy);
    hfdc2db->Fill(db);

    hbrhodx->Fill(brho,dx);
    hbrhoda->Fill(brho,da);
    hbrhody->Fill(brho,dy);
    hbrhodb->Fill(brho,db);
  }

}
