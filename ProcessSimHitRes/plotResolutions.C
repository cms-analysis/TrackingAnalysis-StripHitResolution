#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "ZZStyle.C"
#include "TFile.h"
#include "TColor.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPad.h"
#include "TMath.h"
#include "TSystem.h"
#include <libgen.h>

using namespace std;

class plotResolutions{

 public: 
  plotResolutions();

};

plotResolutions::plotResolutions(){

  TStyle * style = getStyle("ZZ");
  //style->SetMarkerSize(0.8);
  style->cd();
  style->SetNdivisions(508, "X");
  style->SetNdivisions(508, "Y");
  style->SetMarkerSize(0.8);

  TLegend* legend = new TLegend( 0.55, 0.70, 0.9, 0.92);
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.030);
  
  TCanvas *c1 = new TCanvas("c1","c1",800,800);  
  c1->cd();
  c1->SetTicks(1,1);
  

  // 2016 data and MC 

  //TFile *f = TFile::Open("HitResolution_compOutputs_SingleMuon_Run2016B-v2_IB2_BOTH_1_4.root");
  //TFile *f = TFile::Open("HitResolution_compOutputs_SingleMuon_IB2_BOTH_1_4_.root");
  TFile *fdata = TFile::Open("HitResolution_compOutputs_SingleMuon_Run2016B-v2_OB1_BOTH_1_4.root");
  TFile *fmc = TFile::Open("HitResolution_compOutputs_TTbar_OB1_BOTH_1_4.root");
  

  TGraphErrors * OB1_BOTH_1_4_pairResData = (TGraphErrors*)fdata->Get("OB1_1_4_pairRes");
  TH2F *hframe=NULL;
  hframe= new TH2F("hframe","hframe",250,0.,0.45,500,0.1,0.35);
  hframe->GetXaxis()->SetLabelOffset(0.007);
  hframe->GetXaxis()->SetTitleOffset(0.9);
  hframe->GetYaxis()->SetLabelOffset(0.007);  
  hframe->SetXTitle("Track Width");
  hframe->SetYTitle("Resolution");  
  hframe->Draw();
  
  OB1_BOTH_1_4_pairResData->SetMarkerStyle(20);
  OB1_BOTH_1_4_pairResData->SetMarkerColor(4);
  OB1_BOTH_1_4_pairResData->Draw("EPsame");
  legend->AddEntry(OB1_BOTH_1_4_pairResData,"Pair -  2016D Data","P");
  
  TGraphErrors * OB1_BOTH_1_4_pairResMC = (TGraphErrors*)fmc->Get("OB1_1_4_pairRes");
  TGraphErrors * OB1_BOTH_1_4_trueResMC = (TGraphErrors*)fmc->Get("OB1_1_4_trueRes");

  OB1_BOTH_1_4_pairResMC->SetMarkerStyle(21);
  OB1_BOTH_1_4_pairResMC->SetMarkerColor(2);
  OB1_BOTH_1_4_pairResMC->Draw("EPsame");
  if (OB1_BOTH_1_4_pairResMC->Integral()>0. )  legend->AddEntry(OB1_BOTH_1_4_pairResMC,"Pair -  t#bar{t} (Summer16)","P");
  
  OB1_BOTH_1_4_trueResMC->SetMarkerStyle(22);
  OB1_BOTH_1_4_trueResMC->SetMarkerColor(5);
  OB1_BOTH_1_4_trueResMC->Draw("EPsame");
  if (OB1_BOTH_1_4_trueResMC->Integral()>0. ) legend->AddEntry(OB1_BOTH_1_4_trueResMC,"True -  t#bar{t} (Summer16)","P");
  

  // 2015 data and MC 
  
  //TFile *f = TFile::Open("HitResolution_compOutputs_SingleMuon_Run2016B-v2_OB1_BOTH_1_4.root");
  //TFile *f = TFile::Open("HitResolution_compOutputs_SingleMuon_OB1_BOTH_1_4_.root");
  TFile *fdata2015 = TFile::Open("/lustre/home/nicola/slc6/Tracker/Nickmccoll-stripresparam-19fa6ddc24ea/CMSSW_7_6_2/src/ProcessSimHitRes/HitResolution_compOutputs_SingleMuon_Run2015D_OB1_BOTH_1_4.root ");
  TFile *fmcFall15 = TFile::Open("/lustre/home/nicola/slc6/Tracker/Nickmccoll-stripresparam-19fa6ddc24ea/CMSSW_7_6_2/src/ProcessSimHitRes/HitResolution_compOutputs_TTbar_OB1_BOTH_1_4.root");
  

  TGraphErrors * OB1_BOTH_1_4_pairResData2015 = (TGraphErrors*)fdata2015->Get("OB1_1_4_pairRes");
 
  
  OB1_BOTH_1_4_pairResData2015->SetMarkerStyle(23);
  OB1_BOTH_1_4_pairResData2015->SetMarkerColor(6);
  OB1_BOTH_1_4_pairResData2015->Draw("EPsame");
  legend->AddEntry(OB1_BOTH_1_4_pairResData2015,"Pair -  2015D Data","P");
  
  TGraphErrors * OB1_BOTH_1_4_pairResMCFall15 = (TGraphErrors*)fmcFall15->Get("OB1_1_4_pairRes");
  TGraphErrors * OB1_BOTH_1_4_trueResMCFal115 = (TGraphErrors*)fmcFall15->Get("OB1_1_4_trueRes");

  OB1_BOTH_1_4_pairResMCFall15->SetMarkerStyle(24);
  OB1_BOTH_1_4_pairResMCFall15->SetMarkerColor(7);
  OB1_BOTH_1_4_pairResMCFall15->Draw("EPsame");
  if (OB1_BOTH_1_4_pairResMCFall15->Integral()>0. )  legend->AddEntry(OB1_BOTH_1_4_pairResMCFall15,"Pair -  t#bar{t} (Fall15)","P");
  
  OB1_BOTH_1_4_trueResMCFal115->SetMarkerStyle(25);
  OB1_BOTH_1_4_trueResMCFal115->SetMarkerColor(8);
  OB1_BOTH_1_4_trueResMCFal115->Draw("EPsame");
  if (OB1_BOTH_1_4_trueResMCFal115->Integral()>0. ) legend->AddEntry(OB1_BOTH_1_4_trueResMCFal115,"True -  t#bar{t} (Fall15)","P");
  


  legend->Draw("same");
  
  c1->Update();
  c1->SaveAs("plots/OB1_BOTH_1_4_pairRes.png");
  
}
