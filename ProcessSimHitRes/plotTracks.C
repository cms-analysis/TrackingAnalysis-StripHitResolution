{
  //#include "ZZStyle.C"
  //.L ZZStyle.C


// TStyle *style = getStyle("ZZ");
// style->cd();
// style->SetNdivisions(508, "X");
// style->SetNdivisions(508, "Y");
// style->SetMarkerSize(0.8);

TCanvas *c1 = new TCanvas("c1","c1",800,800);  
c1->cd();
c1->SetTicks(1,1);
c1->SetLogy(1);


TFile *_file0 = TFile::Open("HitRes_TTbar.root");
TDirectory* dir = _file0->GetDirectory("analysis");
TTree *trackstree;
dir->GetObject("tracks", trackstree);

// pT 
hframe= new TH2F("hframe","hframe",250,0.,1000.,500,0.1,50000000.);

hframe->GetXaxis()->SetLabelOffset(0.007);
hframe->GetXaxis()->SetTitleOffset(0.9);
hframe->GetYaxis()->SetLabelOffset(0.007);

hframe->SetXTitle("p_{T}");
hframe->SetYTitle("Events/4 GeV");

hframe->Draw();
trackstree->Draw("momentum","","same");
  
c1->Update();


// eta

//TCanvas *c2 = new TCanvas("c2","c2",800,800);  
//c2->cd();
//c2->SetTicks(1,1);
//c2->SetLogy(1);


hframe1= new TH2F("hframe1","hframe1",200,-10.,10.,500,0.1,50000000.);

hframe1->GetXaxis()->SetLabelOffset(0.007);
hframe1->GetXaxis()->SetTitleOffset(0.9);
hframe1->GetYaxis()->SetLabelOffset(0.007);

hframe1->SetXTitle("#eta");
hframe1->SetYTitle("Events/bin=0.1");

hframe1->Draw();
trackstree->Draw("eta","","same");

c1->Update();

// chi2

//TCanvas *c3 = new TCanvas("c3","c3",800,800);  
//c3->cd();
//c3->SetTicks(1,1);
//c3->SetLogy(1);

hframe3= new TH2F("hframe3","hframe3",100,0.,1.,500,100000,10000000.);

hframe3->GetXaxis()->SetLabelOffset(0.007);
hframe3->GetXaxis()->SetTitleOffset(0.9);
hframe3->GetYaxis()->SetLabelOffset(0.007);

hframe3->SetXTitle("#chi^{2}");
hframe3->SetYTitle("Events/bin=0.1");

hframe3->Draw();
trackstree->Draw("trackChi2","","same");

c1->Update();

}
