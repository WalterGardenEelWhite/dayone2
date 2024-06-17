void sample(){
  TH2* hran = new TH2D("hran","generated",100,-100,100,100,-100,100);
  TDirectory *pwd = gDirectory;

  TFile *file = new TFile("root/Gen/AllInpParam.root");
  TH2* h = (TH2*)file->Get("hxypos");

  Double_t x,y;

  for (int i=0;i<1000;i++){
    h->GetRandom2(x,y);
    hran->Fill(x,y);
  }
  //cout<<x<<" "<<y<<endl;

  pwd->cd();
}
