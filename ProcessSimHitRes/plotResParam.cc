/**
  @file         check_top_ewk_scale_factors.cc
  @author       Sue Ann Koay (sakoay@cern.ch)
 */


#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>

#include <TFile.h>
#include <TSystem.h>
#include <TObjArray.h>
#include <TVector2.h>
#include <TMath.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TRandom3.h>

#if !defined(__CINT__) && !defined(MAKECINT)

#include "Utilities/Parang/interface/Prescriptions.h"
#include "Utilities/Parang/interface/Persistables.h"
#include "Utilities/Parang/interface/Pack.h"
#include "Utilities/Parang/interface/Pint.h"
#include "Utilities/Parang/interface/Plotter.h"
#include "Utilities/Parang/interface/Polybook.h"


using namespace std;


void plot(){
   const int pt =100;
   TString dir = "DQMData/Run 1/Tracking/Run summary/Track/general_AssociatorByHitsRecoDenom";
//   TString histos[] ={"chi2_prob","ptres_vs_eta_Sigma","h_ptpulleta_Sigma","dzres_vs_eta_Sigma","h_dzpulleta_Sigma","dxyres_vs_eta_Sigma","h_dxypulleta_Sigma",""};
//   TString xNames[] ={"Track Chi2 Probability","#eta","#eta",""};
//   TString yNames[] ={"# of Events","Track pT Resolution","Track pT Pull","DZRes","DZPull","DXYRes","DXYPull"};

   TString histos[] ={"chi2_prob","ptres_vs_eta_Sigma","h_ptpulleta_Sigma",""};
   TString xNames[] ={"Track Chi2 Probability","#eta","#eta",""};
   TString yNames[] ={"# of Events","Track pT Resolution","Track pT Pull",""};


//   TString histos[] ={"ptres_vs_eta_Sigma",""};
//   TString xNames[] ={"#eta","#eta",""};
//   TString yNames[] ={"Track pT Resolution","Track pT Pull",""};


//   TString inputs[] = {"std.root","orig_change.root","newFunParamIt2.root","bePar.root","oldbePar.root",""};
//   TString names[] = {"Standard","Only Default","New Param.","B/E Param",":(",""};

//   TString inputs[] = {"std.root","newFunParamIt2.root","bePar.root","new_bePar.root",""};
//   TString names[] = {"Standard","New Param.","B/E Param","hope",""};

//   TString inputs[] = {"std.root","new_bePar.root","sdPar.root","geom.root","","temp.root",""};
//   TString names[] = {"Standard","BEPar","SD","geom","temp"};

//   TString inputs[] = {"std.root","newFunParamIt2.root","sdPar.root","","geom.root","","temp.root",""};
//   TString names[] = {"Standard","V1","V2","geom","temp"};


//   TString inputs[] = {"std.root","sdPar.root","geom.root",""};
//   TString names[] = {"Standard","V2","Geom"};

   TString inputs[] = {"../../j_9_19_RecHitRes/100k/std.root","newParam.root",""};
   TString names[] = {"standard","new",""};

//   TString inputs[] = {"std.root","newFunParamIt2.root","new_bePar.root",""};
//   TString names[] = {"Standard","T1","BEPar"};

   vector<TFile*> files;
   for(unsigned int iI = 0; inputs[iI][0]; ++iI)
      files.push_back(new TFile(inputs[iI]));

   TObjArray * o = new TObjArray;
   TObjArray * o2 = new TObjArray;

   for(unsigned int iV = 0; histos[iV][0]; ++iV){
      Plot * p = new Plot();
      for(unsigned int iI = 0; inputs[iI][0]; ++iI){
         files[iI]->cd(dir);
         TH1 * hist  = 0;
         gDirectory->GetObject(histos[iV], hist);
         // files[iI]->GetObject(histos[iV], hist);
         if(hist == 0){
            cout << endl << inputs[iI] <<" "<< histos[iV] <<endl;
            continue;
         }

         hist->SetTitle(" ");
         hist->GetXaxis()->SetTitle(xNames[iV]);
         hist->GetYaxis()->SetTitle(yNames[iV]);
         p->add(hist,names[iI]);
      }

      o->Add(p);
      if(p->getNumberOfHistograms() < 2) continue;
//      p->normalize();
      p = p->makeRatiogram(0u,"");
//      p->SetTitle("hi");
      o2->Add(p);


   }
   Pint::drawAll(o,"");
   Pint::drawAll(o2,"");


}

//void plot(){
//   const int pt =100;
//   TString dir = "DQMData/Run 1/Tracking/Run summary/Track/general_AssociatorByHitsRecoDenom";
//   TString histos[] ={"ptpull_vs_eta","","ptres_vs_eta_Sigma","h_ptpulleta_Sigma","h_ptpulleta_Mean",""};
//   TString xNames[] ={"Track Chi2 Probability","#eta","#eta",""};
//   TString yNames[] ={"# of Events","Track Resolution","Track Pull","mean"};
//
////   TString inputs[] = {"std.root","orig_change.root","newFunParamIt2.root","bePar.root","oldbePar.root",""};
////   TString names[] = {"Standard","Only Default","New Param.","B/E Param",":(",""};
//
//   TString inputs[] = {"std.root","newFunParamIt2.root","bePar.root","oldbePar.root",""};
//   TString names[] = {"Standard","New Param.","B/E Param",":(",""};
//
//   vector<TFile*> files;
//   for(unsigned int iI = 0; inputs[iI][0]; ++iI)
//      files.push_back(new TFile(inputs[iI]));
//
//   TObjArray * o = new TObjArray;
//   TObjArray * o2 = new TObjArray;
//
//   for(unsigned int iV = 0; histos[iV][0]; ++iV){
//      Plot * p = new Plot();
//      for(unsigned int iI = 0; inputs[iI][0]; ++iI){
//         files[iI]->cd(dir);
//         TH2 * hist2  = 0;
//         gDirectory->GetObject(histos[iV], hist2);
//         // files[iI]->GetObject(histos[iV], hist);
//         if(hist2 == 0){
//            cout << endl << inputs[iI] <<" "<< histos[iV] <<endl;
//            continue;
//         }
//
//         TH1 * oH = new TH1F(TString::Format("%u_%u",iI,iV),TString::Format("%u_%u",iI,iV),hist2->GetNbinsX(),hist2->GetXaxis()->GetXmin(),hist2->GetXaxis()->GetXmax());
//
//         for(unsigned int iB = 0; iB<=hist2->GetNbinsY(); ++iB){
//           TH1 * hist = hist2->ProjectionY(TString::Format("%u_%u_%u",iI,iV,iB),iB,iB);
//           oH->SetBinContent(iB,hist->GetRMS());
//         }
//
//
//
//         oH->SetTitle(" ");
//         oH->GetXaxis()->SetTitle(xNames[iV]);
//         oH->GetYaxis()->SetTitle(yNames[iV]);
//         p->add(oH,names[iI]);
//         new TCanvas();
//         oH->Draw();
//      }
//
//      p->toUnderOverflow();
//      o->Add(p);
//      if(p->getNumberOfHistograms() < 2) continue;
//      p = p->makeRatiogram(0u,"");
//      o2->Add(p);
//
//
//   }
//   Pint::drawAll(o,"","",":r");
//   Pint::drawAll(o2,"");
//
//
//}
//

void plotMean(){
   const int pt =100;
   TString dir = "DQMData/Run 1/Tracking/Run summary/Track/general_AssociatorByHitsRecoDenom";
   TString histos[] ={"chi2_vs_eta","chi2_vs_nhits","dzres_vs_eta",""};
   TString xNames[] ={"Track Chi2 Probability","#eta","#eta",""};
   TString yNames[] ={"Mean","Track Resolution","Track Pull","mean"};

//   TString inputs[] = {"std.root","orig_change.root","newFunParamIt2.root","bePar.root","oldbePar.root",""};
//   TString names[] = {"Standard","Only Default","New Param.","B/E Param",":(",""};

   TString inputs[] = {"std.root","new_bePar.root","geom.root","sdPar.root",""};
   TString names[] = {"Standard","BEPar","geom","sd"};
//   TString inputs[] = {"std.root","newFunParamIt2.root","new_bePar.root",""};
//   TString names[] = {"Standard","T1","BEPar"};

   vector<TFile*> files;
   for(unsigned int iI = 0; inputs[iI][0]; ++iI)
      files.push_back(new TFile(inputs[iI]));

   TObjArray * o = new TObjArray;
   TObjArray * o2 = new TObjArray;

   for(unsigned int iV = 0; histos[iV][0]; ++iV){
      Plot * p = new Plot();
      for(unsigned int iI = 0; inputs[iI][0]; ++iI){
         files[iI]->cd(dir);
         TH2 * hist2  = 0;
         gDirectory->GetObject(histos[iV], hist2);
         // files[iI]->GetObject(histos[iV], hist);
         if(hist2 == 0){
            cout << endl << inputs[iI] <<" "<< histos[iV] <<endl;
            continue;
         }

         hist2->RebinX(4);

         TH1 * oH = new TH1F(TString::Format("%u_%u",iI,iV),TString::Format("%u_%u",iI,iV),hist2->GetNbinsX(),hist2->GetXaxis()->GetXmin(),hist2->GetXaxis()->GetXmax());
//         TGraphAsymmErrors * oH = new TGraphAsymmErrors(hist2->GetNbinsY());

         for(unsigned int iB = 0; iB<=hist2->GetNbinsX(); ++iB){
           TH1 * hist = hist2->ProjectionY(TString::Format("%u_%u_%u",iI,iV,iB),iB,iB);
//           double x[] = {.25,.5,.75};
//           double y[] = {0,0,0};

           double x[] = {.95,.5,.75};
           double y[] = {0,0,0};

           hist->GetQuantiles(3,y,x);


//           double mean = hist->GetMean();

           oH->SetBinContent(iB,hist->GetMean());
           oH->SetBinError(iB,hist->GetMeanError());
//           oH->SetBinContent(iB,y[0]);
//           oH->SetBinError(iB,0);
         }



         oH->SetTitle(" ");
         oH->GetXaxis()->SetTitle(xNames[iV]);
         oH->GetYaxis()->SetTitle(yNames[iV]);
         p->add(oH,names[iI]);
//         new TCanvas();
//         oH->Draw();
      }

      p->toUnderOverflow();
      o->Add(p);
      if(p->getNumberOfHistograms() < 2) continue;
      p = p->makeRatiogram(0u,"");
      o2->Add(p);


   }
   Pint::drawAll(o,"","",":r");
   Pint::drawAll(o2,"");


}


#endif //!CINT

//_____________________________________________________________________________
void plotResParam         ()
{
   plot();
//   plotMean();
}
