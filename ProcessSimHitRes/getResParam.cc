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

enum Locations {ALL,ENDCAP,BARREL,TID,TEC};
TRandom * r = new TRandom3(12345);

//__________Processing functions___________________________________________________________________

void fillCenters(const TH2* hist, const RebinToNewBins * bins, double centers[]){
   for(unsigned int iB = 0; iB < bins->numBins; ++iB){
      int firstBin = hist->GetYaxis()->FindFixBin(bins->binEdges[iB]);
      int lastBin = hist->GetYaxis()->FindFixBin(bins->binEdges[iB + 1]) -1;
      if(hist->GetYaxis()->GetXmin() == bins->binEdges[iB] ) firstBin = 0;
      if(hist->GetYaxis()->GetXmax() == bins->binEdges[iB] ) lastBin = hist->GetNbinsY() + 1;
      
      double mean = 0;
      double total = 0;
      
      for(unsigned int iB2 = firstBin; iB2 <= lastBin; ++iB2){
         double x = hist->GetYaxis()->GetBinCenter(iB2);
         double w = hist->Integral(0,-1,iB2,iB2);
         
         if(iB2 == 0) x = hist->GetYaxis()->GetXmin();
         if(iB2 == hist->GetNbinsY() + 1) x = hist->GetYaxis()->GetXmax();
         

         mean += x*w;
         total += w;  
      }
      mean /= total;  
      centers[iB] = mean;
      
      // cout << iB << " "<< firstBin << " - "<< lastBin <<" :: "<<  hist->GetYaxis()->GetBinLowEdge(firstBin) <<" - "<< mean <<" - "<< hist->GetYaxis()->GetBinLowEdge(lastBin) + hist->GetYaxis()->GetBinWidth(lastBin) <<endl;      
   }
}


void getWidthByQuant(const TH1D* hist, TString name, double& width, double& error, const double& maxDev = -1){


   const double xMin = hist->GetXaxis()->GetXmin();
   const double xMax = hist->GetXaxis()->GetXmax();
   const int   nBins = hist->GetXaxis()->GetNbins();

   const double rangeMin = maxDev > 0 ? -1*maxDev : xMin ;
   const double rangeMax = maxDev > 0 ?    maxDev : xMax ;

   const int firstBin = maxDev > 0 ? hist->FindFixBin(rangeMin) : 1 ;
   const int lastBin  = maxDev > 0 ? hist->FindFixBin(rangeMax) : nBins ;


   const int nEntries = hist->Integral(firstBin,lastBin);
   const double underNEntries =  maxDev > 0 ? hist->Integral(1,firstBin -1) :0;

   const int numIt = 20;

   TH1D * histInt = (TH1D*)(Properties::makeIntegral(hist,false,false,0));
   
   width = (Properties::findAbscissa(histInt->GetArray(),histInt->GetXaxis(),  .841*double(nEntries)  + underNEntries) - Properties::findAbscissa(histInt->GetArray(),histInt->GetXaxis(),  .159*double(nEntries)  + underNEntries ) ) /2;


   error = .001;
   return;


   float content[numIt+1];
   float widths[numIt];
//
   for(unsigned int i2 =0; i2 <= numIt; ++i2)
      content[i2] = histInt->GetBinContent(i2);
//   int firstBin = 1;
//   int lastBin  = nBins;

   for(unsigned int iI = 0; iI < numIt; ++iI){
      TString name2 = name  + TString::Format("_%u",iI);
      TH1D * hF = new TH1D(name2,name2,nBins,xMin,xMax);

      for(unsigned int iR = 0; iR < nEntries; ++iR){
         double cumulative = r->Uniform(0,nEntries);
         Int_t     bin  =  std::upper_bound(content+firstBin, content+lastBin,cumulative) - content;

         if(content[bin] == 0){
            iR--;
            continue;
         }

         hF->Fill(hF->GetXaxis()->GetBinCenter(bin));
      }

      hF = (TH1D*)(Properties::makeIntegral(hF,false,false,0));

    widths[iI] = (Properties::findAbscissa(hF->GetArray(),hF->GetXaxis(),  .841*hist->Integral() ) - Properties::findAbscissa(hF->GetArray(),hF->GetXaxis(),  .159*hist->Integral() ) ) /2;
   }

   error = 0;
   double avgError = 0;

   for(unsigned int i =0 ; i < numIt; ++i){
      avgError += widths[i];
   }
   avgError /= numIt;

   for(unsigned int i =0 ; i < numIt; ++i){
      error += (widths[i] - avgError)*(widths[i] - avgError);
   }
   error /=numIt;

   error = TMath::Sqrt(error);
}

void getWidthByRMS(const TH1* hist, double& width, double& error, double maxDev = -1){
   double mu = 0; 
   double total = 0;
   
   for(unsigned int iB = 0; iB <= hist->GetNbinsX() +1; ++iB){
      double x = hist->GetBinCenter(iB);
      double w = hist->GetBinContent(iB);
      
      if(maxDev > 0)
        if(TMath::Abs(x) >= maxDev) continue;

      if(iB == 0)
         x = hist->GetXaxis()->GetXmin();
      
      if(iB == hist->GetNbinsX() +1)
         x = hist->GetXaxis()->GetXmax();
      
      mu += x*w;
      total += w;
   }
   
   mu /= total;
   
   double sigma = 0;
   double avg4th = 0;
   double avg2nd = 0;
   double sigmaSigma = 0;
   
   for(unsigned int iB = 0; iB <= hist->GetNbinsX() +1; ++iB){
      double x = hist->GetBinCenter(iB);
      double w = hist->GetBinContent(iB);
      
      if(maxDev > 0)
        if(TMath::Abs(x) >= maxDev) continue;

      if(iB == 0)
         x = hist->GetXaxis()->GetXmin();
      
      if(iB == hist->GetNbinsX() +1)
         x = hist->GetXaxis()->GetXmax();
      
      sigma += w*(x*x - mu*mu);
      avg4th += w*(x - mu)*(x - mu)*(x - mu)*(x - mu);
      avg2nd += w*(x - mu)*(x - mu);
   }
   
   avg4th /= total;
   avg2nd /= total;
   
   sigma /= total;
   sigma = TMath::Sqrt(sigma);
   
   sigmaSigma = (avg4th - avg2nd*avg2nd)/(4*total*sigma*sigma);
   sigmaSigma = TMath::Sqrt(sigmaSigma);
   
   width = sigma;
   error = sigmaSigma;
}

TGraphErrors * calculate(TH2F * origHisto, const RebinToNewBins * bins, TString name = "input", bool useQuant = false, double maxDev = -1){
   TObjArray * o = new TObjArray;
   o->Add(origHisto);
   
   double centers[bins->numBins];
   fillCenters(origHisto,bins,centers);
   
   TH2F * inputHisto = (TH2F*)origHisto->Clone();
   Procedures::rebin(inputHisto,*bins);
   Procedures::toUnderOverflow(inputHisto);

   TGraphErrors * outputGraph = new TGraphErrors();
   
   for(unsigned int iW = 1; iW <= bins->numBins; ++iW){
     double width = 0;
     double error = 0;
      TString sliceName = name; sliceName += TString::Format("_px1_%u",iW);
      TH1D * input1D = inputHisto->ProjectionX(sliceName,iW,iW);
      TString sliceTitle = TString::Format("%.2f - %.2f",inputHisto->GetYaxis()->GetBinLowEdge(iW), inputHisto->GetYaxis()->GetBinLowEdge(iW)+ inputHisto->GetYaxis()->GetBinWidth(iW) );
      input1D->SetTitle(sliceTitle);
      o->Add(input1D);

      cout << endl << input1D->Integral(input1D->FindFixBin(-.1582),input1D->FindFixBin(.1582))/input1D->Integral() << " " << input1D->Integral(input1D->FindFixBin(-.1725),input1D->FindFixBin(.1725))/input1D->Integral() <<endl;

      if(input1D->GetEntries() >= 10 ){
      
      if(useQuant)
         getWidthByQuant(input1D,name + TString::Format("%u",iW),width,error,maxDev);
      else
         getWidthByRMS(input1D,width,error,maxDev);
      }
      

      outputGraph->SetPoint(iW -1,centers[iW - 1 ],width);
      outputGraph->SetPointError(iW -1,0,error);
   }
   


   Pint::drawAll(o,name,"",":i");
   
   return outputGraph;
}


//__________Drawing and fitting___________________________________________________________________

TF1 * fitAndDrawGraph(const TGraphErrors * origGraph, TString name, int fitType = 0){
   TGraphErrors * graph = (TGraphErrors*)origGraph->Clone();
   graph->GetXaxis()->SetTitle("Track Width");
   graph->GetYaxis()->SetTitle("Resolution");
   new TCanvas();
   graph->SetTitle(name);
   graph->SetMarkerStyle(20);
   graph->Draw("AP");
   
   double out = 0;

   if(fitType < 0) return 0;
   
   TF1 * f1 = 0;
   if(fitType == 0){
//      f1 = new TF1(name + "_fit","[0] + [1]*exp(-1*[2]*x)*(x+[3]*x*x)",0,5.5);
      f1 = new TF1(name + "_fit","[0] + [1]*exp(-1*[2]*(x+[4]))*((x+[4])+[3]*(x+[4])*(x+[4]))",0,6);
//      f1 = new TF1(name + "_fit","[0] + [1]*exp(-1*[2]*x)*(x+[3]*x*x)",0,6);

//      f1->SetParameters(.2675,-.3,1,2,0);
      f1->SetParameters(.197,1.572,2.343,-1.138,.2312);
//      f1->FixParameter(0,.165);
   }
   
   if(fitType == 1){
      f1 = new TF1(name + "_fit","[0] + [1]*x +[2]*x*x",0,15);
   }
   
   if(fitType == 2){
      f1 = new TF1(name + "_fit","[0] + [1]*exp(-1*x*[2])",0,5.5);
   }
   
   if(fitType == 3){
     f1 = new TF1(name + "_fit","[0] + [1]*exp(-1*[2]*(x+[4]))*((x+[4])+[3]*(x+[4])*(x+[4]))",0,6);
//     f1->FixParameter(1,1.807);
//     f1->FixParameter(2,3.141);
//     f1->FixParameter(3,-1.672);
//     f1->FixParameter(4,.128);

     f1->FixParameter(1,1.178);
     f1->FixParameter(2,2.287);
     f1->FixParameter(3,-0.671);
     f1->FixParameter(4,.4739);
      
   }
   if(fitType == 4){
     f1 =new TF1(name + "_fit","[0] + [1]*x*exp(-1*x*[2])",0,5.5);

     //Barrel
//     f1->SetParameters(.201,.1094,3.438);
     f1->FixParameter(1,-1.41663);
     f1->FixParameter(2,3.37073);

//     //Endcap
//     f1->SetParameters(.05425,.4826,1.249);
//     f1->FixParameter(1,.4826);
//     f1->FixParameter(2,1.249);

   }
   if(fitType ==5)
     f1 =new TF1(name + "_fit","[0]",0,5.5);



   graph->Fit(f1,"QS");
   return f1;
}

//__________Helper functions for starting___________________________________________________________________

TH2F * applyFilter(TFile * f, TString dir, TString prefix, double min, double max, int minW, int maxW){
  TH2F * outHist = 0;
   f->cd(dir);
   // 
   for(unsigned int iI = minW; iI <= maxW; ++iI){
      TH2F * h; gDirectory->GetObject(TString::Format(prefix,iI) ,h);
      
      if(outHist == 0){ outHist = (TH2F*)h->Clone(); outHist->Reset(); }
      
      for(unsigned int iY = 0; iY <= h->GetNbinsY() + 1; ++iY){
         if(iI - h->GetYaxis()->GetBinCenter(iY) > max ) continue;
         if(iI - h->GetYaxis()->GetBinCenter(iY) <= min ) continue;
         for(unsigned int iX = 0; iX <= h->GetNbinsX() + 1; ++iX){
            for(unsigned int iC = 0; iC <h->GetBinContent(iX,iY); ++iC)
               outHist->Fill( h->GetXaxis()->GetBinCenter(iX), h->GetYaxis()->GetBinCenter(iY));
         }  
   }
}
   return outHist;  
}


TH2F * getOne(TFile * f, TString dir, TString prefix){
   f->cd(dir);
   TH2F * h; gDirectory->GetObject(prefix ,h);
   return h;  
}


//_________________Starting Functions____________________________________________________________

void compareSubD(TFile * file, int minW, int maxW, bool useQuant =false, TString name = ""){
   // double bins[] = {0,.25,.5,.75,1,1.25,2,5.5};
//   double bins[] = {-.025,.225,.525,.775,1.025,1.275,2.025,5.975};
//   double bins[] = {-.025,.025,.075,.125,.225,.525,.775,1.025,1.275,2.025,2.525,5.975};
//   double bins[] = {-.025,.025,.075,.125,.225,.475,.525,.575,.625,.675,.725,.775,.825,1.025,1.275,2.025,2.525,5.975}; //1 17 bins
  double bins[] = {-.025,.075,.125,.225,.475,.575,5.975}; //6 bins
   RebinToNewBins b(Axis::Y,6,bins);

   Color_t colors[] = {kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};

   TMultiGraph * mg = new TMultiGraph("mg","mg");

   // TH2F * h1 = applyFilter(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL","Res_MF_rphi_v_TrackWidth",-99999,99999,1,4);
   // TGraph * graph1 = calculate(h1,&b,TString::Format("%u",4),useQuant);
   // graph1->SetTitle(TString::Format("Cluster Width <= 4"));
   // graph1->SetMarkerStyle(20);
   // graph1->SetMarkerColor(colors[0]);
   // mg->Add(graph1);


   TString loc[] ={"","_TIB","_TOB","_TEC","_TID",""};

   for(unsigned int iL = 0; loc[iL][0] || !iL; ++iL){
     TString histName = "Res_MF_rphi_v_TrackWidth_wclus%u";
     histName += loc[iL];
     TH2F * h = applyFilter(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL",histName,-99999,99999,minW,maxW);
     TGraph * graph = calculate(h,&b,name + loc[iL],useQuant, -9);
     TString gah = loc[iL];
     if(iL == 0) gah = "All";
     gah.ReplaceAll("_","");
     graph->SetTitle(name + gah);
     graph->SetMarkerStyle(20);
     graph->SetMarkerColor(colors[iL]);
     mg->Add(graph);
   }



   TCanvas * c1 = new TCanvas();
   mg->SetTitle(name);
   mg->Draw("AP");
   c1->BuildLegend();
   mg->GetXaxis()->SetTitle("Track Width");
   mg->GetYaxis()->SetTitle("Resolution");
   Pint::refresh();

}

void fitSubD(TFile * file, int minW, int maxW, bool useQuant =false, TString name = ""){
   // double bins[] = {0,.25,.5,.75,1,1.25,2,5.5};
//   double bins[] = {-.025,.225,.525,.775,1.025,1.275,2.025,5.975};
//   double bins[] = {-.025,.025,.075,.125,.225,.525,.775,1.025,1.275,2.025,2.525,5.975};
//   double bins[] = {-.025,.025,.075,.125,.225,.475,.525,.575,.625,.675,.725,.775,.825,1.025,1.275,2.025,2.525,5.975}; //1 17 bins
  double bins[] = {-.025,.075,.125,.225,.475,.575,5.975}; //6 bins
   RebinToNewBins b(Axis::Y,6,bins);

   Color_t colors[] = {kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};

   TMultiGraph * mg = new TMultiGraph("mg","mg");

   // TH2F * h1 = applyFilter(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL","Res_MF_rphi_v_TrackWidth",-99999,99999,1,4);
   // TGraph * graph1 = calculate(h1,&b,TString::Format("%u",4),useQuant);
   // graph1->SetTitle(TString::Format("Cluster Width <= 4"));
   // graph1->SetMarkerStyle(20);
   // graph1->SetMarkerColor(colors[0]);
   // mg->Add(graph1);


   TString loc[] ={"","_barrel","_endcap","_TIB","_TOB","_TEC","_TID",""};


   for(unsigned int iL = 0; loc[iL][0] || !iL; ++iL){
     TString histName = "Res_MF_rphi_v_TrackWidth_wclus%u";
     histName += loc[iL];
     TH2F * h = applyFilter(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL",histName,-99999,99999,minW,maxW);
     TGraphErrors * graph = calculate(h,&b,name + loc[iL],useQuant, -9);
     TF1* fit = fitAndDrawGraph(graph,name + loc[iL],5);
     cout << iL <<" "<< fit->GetParameter(0) <<endl;


     graph->SetTitle(name + loc[iL]);
     graph->SetMarkerStyle(20);
     graph->SetMarkerColor(colors[iL]);
     mg->Add(graph);
   }



   TCanvas * c1 = new TCanvas();
   mg->SetTitle(name);
   mg->Draw("AP");
   c1->BuildLegend();
   mg->GetXaxis()->SetTitle("Track Width");
   mg->GetYaxis()->SetTitle("Resolution");
   Pint::refresh();

}


void studyClusterWidths(TFile * file, Locations loc, bool useQuant =false){
   // double bins[] = {0,.25,.5,.75,1,1.25,2,5.5};
//   double bins[] = {-.025,.225,.525,.775,1.025,1.275,2.025,5.975};
//   double bins[] = {-.025,.025,.075,.125,.225,.525,.775,1.025,1.275,2.025,2.525,5.975};
//   double bins[] = {-.025,.025,.075,.125,.225,.475,.525,.575,.625,.675,.725,.775,.825,1.025,1.275,2.025,2.525,5.975}; //1 17 bins
   double bins[] = {-.025,.075,.125,.225,.475,.575,5.975}; //6 bins
   RebinToNewBins b(Axis::Y,6,bins);

   Color_t colors[] = {kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};
   
   TMultiGraph * mg = new TMultiGraph("mg","mg");
   
   // TH2F * h1 = applyFilter(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL","Res_MF_rphi_v_TrackWidth",-99999,99999,1,4);
   // TGraph * graph1 = calculate(h1,&b,TString::Format("%u",4),useQuant);
   // graph1->SetTitle(TString::Format("Cluster Width <= 4"));
   // graph1->SetMarkerStyle(20);
   // graph1->SetMarkerColor(colors[0]);
   // mg->Add(graph1);
   
   
   for(unsigned int iW = 1  ; iW <= 5; ++iW){
      TString histName = TString::Format("Res_MF_rphi_v_TrackWidth_wclus%u",iW);
      if(loc == BARREL) histName += "_barrel";
      if(loc == ENDCAP) histName += "_endcap";
      TH2F * h = getOne(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL",histName);
      TGraph * graph = calculate(h,&b,TString::Format("%u",iW),useQuant ,iW > 2 ? -1 : 5);
      graph->SetTitle(TString::Format("Cluster Width = %u",iW));
      graph->SetMarkerStyle(20);
//       graph->SetMarkerColor(colors[iW - 5]);
      graph->SetMarkerColor(colors[iW - 1]);

      mg->Add(graph);
      

   }
   TCanvas * c1 = new TCanvas();
   mg->SetTitle(" ");
   mg->Draw("AP");
   c1->BuildLegend();
   mg->GetXaxis()->SetTitle("Track Width");
   mg->GetYaxis()->SetTitle("Resolution");
   Pint::refresh();
   
}

void studyHighClusterWidths(TFile * file,Locations loc, bool useQuant =false){
   double bins[] = {0,.25,.5,.75,5.5};
   RebinToNewBins b(Axis::Y,4,bins);

   Color_t colors[] = {kBlack,kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};
   
   double uniOff = 1;
   
   TMultiGraph * mg = new TMultiGraph("mg","mg");
   
   TGraphErrors * offsetGraph = new TGraphErrors();
   
   TMultiGraph * simpleOffsetCorrGraph  = new TMultiGraph("mg","mg");

   TMultiGraph  * compOffsetCorrGraph  = new TMultiGraph("mg","mg");
   vector<double> compE(0);
   vector<double> compX(0);
   vector<double> compY(0);
   vector<double> compN(0);         
   // TF1 *       offsetFunct = new TF1("OffsetFunction","[0] + [1]*x",0,10);
   // offsetFunct->SetParameters(-1.839,.4494);
   TF1 *       offsetFunct =new TF1("OffsetFunction","[0] + [1]*exp(-1*x*[2])",0,15);
   offsetFunct->SetParameters(1.926,-12.87,.4282);

   int cCounter = 0;
   for(unsigned int iW = 1; iW <= 9; ++iW){
      TH2F * h = applyFilter(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL","Res_MF_rphi_v_TrackWidth",4.75,99999,iW,iW);
      if(h->GetEntries() == 0) continue;
      TGraph * graph = calculate(h,&b,TString::Format("%u",iW),useQuant);
      graph->SetTitle(TString::Format("Cluster Width = %u",iW));
      graph->SetMarkerStyle(20);
      graph->SetMarkerColor(colors[cCounter]);cCounter++;
         
      double xo, sinpleOffset;      
      if(graph->GetPoint(0,xo,sinpleOffset) < 0)continue;      
      static unsigned int iP = 0;
      offsetGraph->SetPoint(iP,iW,sinpleOffset);
      offsetGraph->SetPointError(iP,0,graph->GetErrorY(0));
      ++iP;
      
      TGraphErrors * simpleCorrGraph = (TGraphErrors*)graph->Clone();
      for(unsigned int iP = 0; iP < graph->GetN(); ++iP){
         double x,y;
         if(graph->GetPoint(iP,x,y) >= 0)
            simpleCorrGraph->SetPoint(iP, x, y - sinpleOffset + uniOff );
      }
      
      TGraphErrors * compCorrGraph = (TGraphErrors*)graph->Clone();
      for(unsigned int iP = 0; iP < graph->GetN(); ++iP){
         double x,y,e;
         if(graph->GetPoint(iP,x,y) < 0) continue;
         e = compCorrGraph->GetErrorY(iP);
         
         y = y - offsetFunct->Eval(iW);
         
         compCorrGraph->SetPoint(iP, x, y + uniOff);
                   
         while(iP  >= compN.size()){
            compE.push_back(0);
            compX.push_back(0);
            compY.push_back(0);
            compN.push_back(0);
         }
         compE[iP] += (1/(e*e))*e;   
         compX[iP] += (1/(e*e))*x;
         compY[iP] += (1/(e*e))*(y + uniOff);
         compN[iP] += (1/(e*e));
      }
      
      compOffsetCorrGraph->Add(compCorrGraph);
      simpleOffsetCorrGraph->Add(simpleCorrGraph);
      mg->Add(graph);
      
   }
   TCanvas * c1 = new TCanvas();
   mg->Draw("AP");
   mg->SetTitle("Standard");
   c1->BuildLegend();
   mg->GetXaxis()->SetTitle("Track Width");
   mg->GetYaxis()->SetTitle("Resolution");  
   
   TCanvas * c2 = new TCanvas();
   simpleOffsetCorrGraph->Draw("AP");
   simpleOffsetCorrGraph->SetTitle("Simple Offset");
   simpleOffsetCorrGraph->GetXaxis()->SetTitle("Track Width");
   simpleOffsetCorrGraph->GetYaxis()->SetTitle("Resolution");
   c2->BuildLegend();   
   
   fitAndDrawGraph(offsetGraph,"Offsets",3);
   
   TCanvas * c3 = new TCanvas();
   compOffsetCorrGraph->Draw("AP");
   compOffsetCorrGraph->SetTitle("Comp Offset");
   compOffsetCorrGraph->GetXaxis()->SetTitle("Track Width");
   compOffsetCorrGraph->GetYaxis()->SetTitle("Resolution");
   c3->BuildLegend();   
   
   TGraphErrors * compCombOffsetCorrGraph  = new TGraphErrors();
   for(unsigned int iP = 0; iP < compN.size(); ++iP){
      cout << endl << compE[iP] <<" "<<compX[iP]  <<" "<< compY[iP]<<" "<<compN[iP] <<" "<< endl;
      compE[iP] = compE[iP] / double(compN[iP]);
      compX[iP] = compX[iP] / double(compN[iP]);
      compY[iP] = compY[iP] / double(compN[iP]);
      compCombOffsetCorrGraph->SetPoint(iP, compX[iP], compY[iP]);
      compCombOffsetCorrGraph->SetPointError(iP,0,compE[iP]);
   }
    fitAndDrawGraph(compCombOffsetCorrGraph,"Comb. Offsest",2);
    Pint::refresh();
   
   
}


void studyBarrelHighClusterWidths(TFile * file,Locations loc, bool useQuant =false){

   TString histName = "Res_MF_rphi_v_TrackWidth_wclus%u";
   if(loc == BARREL) histName += "_barrel";
   if(loc == ENDCAP) histName += "_endcap";
   Color_t colors[] = {kBlack,kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};

   //First fit the base function
//   double bins5[] = {-.025,.025,.075,.125,.225,.325,.375,.425,.475,.525,.575,.625,.675,.725,.775,.825,.875,5.975}; //17 used for the low guys
   double bins5[] = {-.025,.075,.125,.225,.475,.525,.575,5.975}; //7 bins



   RebinToNewBins b5(Axis::Y,7,bins5);

   TH2F * baseH = getOne(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL",TString::Format(histName,6));
   TGraphErrors * baseGraph = calculate(baseH,&b5,"base",useQuant);
   fitAndDrawGraph(baseGraph,"base",3);


   //Now for the offsets!
   int cCounter = 0;
   TGraphErrors * offsetGraph = new TGraphErrors();
   vector<double> points;
   int count = 0;
   for(unsigned int iW = 6; iW <= 9; ++iW){
     TH2F * h = getOne(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL",TString::Format(histName,iW));
      if(h->GetEntries() == 0) continue;
      TGraphErrors * graph = calculate(h,&b5,TString::Format("%u",iW),useQuant);
      graph->SetTitle(TString::Format("Cluster Width = %u",iW));
      TF1* fit = fitAndDrawGraph(graph,TString::Format("%u",iW),3);
      offsetGraph->SetPoint(count,iW,fit->GetParameter(0));count++;
      cout << iW <<" "<< fit->GetParameter(0) <<" "<< fit->GetParameter(1) <<" "<< fit->GetParameter(2) << endl;
   }
   fitAndDrawGraph(offsetGraph,"Offsest",1);





}

void studyBarrelLowClusterWidths(TFile * file,Locations loc, bool useQuant =false){

   TString histName = "Res_MF_rphi_v_TrackWidth_wclus%u";
   if(loc == BARREL) histName += "_barrel";
   if(loc == ENDCAP) histName += "_endcap";
   Color_t colors[] = {kBlack,kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};

   //First fit the base function
   double bins5[] = {-.025,.025,.075,.125,.225,.325,.375,.425,.475,.525,.575,.625,.675,.725,.775,.825,.875,5.975}; //17 used for the low guys

//   double bins5[] = {-.025,.025,.075,.125,.225,5.975}; //5 used for the low guys


   RebinToNewBins b5(Axis::Y,17,bins5);


   //Now for the offsets!
   for(unsigned int iW = 1; iW <= 5; ++iW){
     TH2F * h = getOne(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL",TString::Format(histName,iW));
      if(h->GetEntries() == 0) continue;
      TGraphErrors * graph = calculate(h,&b5,TString::Format("%u",iW),useQuant,iW > 2 ? -1 : 5);
      graph->SetTitle(TString::Format("Cluster Width = %u",iW));
      TF1 * fit = fitAndDrawGraph(graph,TString::Format("%u",iW),0);

      cout << iW <<" "<< fit->GetParameter(0)<<" "<< fit->GetParameter(1)<<" "<< fit->GetParameter(2)<<" "<< fit->GetParameter(3)<<" "<< fit->GetParameter(4)<<endl;

   }





}


void plotAndFitNormal(TFile * file,TString name, Locations loc, bool useQuant = false, int minW = 1, int maxW =3){
   // double bins[] = {0,.25,.5,.75,1,1.25,2,5.5};
   
//   double bins[] = {-.025,.225,.525,.775,1.025,1.275,2.025,2.525,5.975};
   
//     double bins[] = {-.025,.025,.075,.125,.225,.475,.525,.575,.625,.675,.725,.775,.875,1.025,1.275,2.025,2.525,5.975}; //17

//     double bins[] = {-.025,.025,.075,.125,.225,.475,.525,.575,.625,.675,.725,.775,.825,.875,1.5,2,2.525,5.975}; //17 used for the low guys

     double bins[] = {-.025,.075,.125,.225,.475,.575,5.975}; //6 bins


   
   RebinToNewBins b(Axis::Y,6,bins);
   
   // double bins[] = {0,.25,.5,.75,1,1.25,1.75,2.75,5.5};
   // RebinToNewBins b(Axis::Y,8,bins);
   
   TString histName = "Res_MF_rphi_v_TrackWidth_wclus%u";
   if(loc == BARREL) histName += "_barrel";
   if(loc == ENDCAP) histName += "_endcap";
   if(loc == ALL) histName += "";
   if(loc == TID) histName += "_TID";
   if(loc == TEC) histName += "_TEC";


   TH2F * filteredH = applyFilter(file,"DQMData/Run 1/Tracking/Run summary/TrackingRecHits/Strip/ALL",histName,-99999,99999,minW,maxW);

   TGraphErrors * outputGraph = calculate(filteredH,&b,name,useQuant,5);
//   outputGraph->SetPoint(b.numBins,5,.169);
//   outputGraph->SetPointError(b.numBins,0,.01);

   fitAndDrawGraph(outputGraph,name,3);
   
   

   // 
   // outputGraph->SetPoint(bins->numBins+1,10,.2679);
   // outputGraph->SetPointError(bins->numBins+1,0,.05);
   
   
}




#endif //!CINT

//_____________________________________________________________________________
void getResParam         ()
{
   TFile * f = new TFile("std.root","read");

//   for(unsigned int iI = 1; iI <= 1; ++iI)
//     compareSubD(f,iI,iI,true,TString::Format("%u",iI));
   compareSubD(f,7,7,true,"");

//   for(unsigned int iI = 3; iI <= 3; ++iI)
//         plotAndFitNormal(f,iI,ALL,true,iI,iI);


//   plotAndFitNormal(f,"5",TID,true,5,5);


//             fitSubD(f,8,8,true);



//        fitSubD(f,4,4,true);

//        fitSubD(f,1,3,true);


//  TFile * f = new TFile("../100k/bePar.root","read");

//   studyClusterWidths(f,BARREL,false);
//   studyBarrelLowClusterWidths(f,BARREL,true);
//    plotAndFitNormal(f,"Filtered",ALL,true);
//   studyBarrelHighClusterWidths(f,BARREL,true);
   
//   studyClusterWidths(f,ENDCAP,true);
//    plotAndFitNormal(f,"Filtered",ENDCAP,true);
//   studyBarrelHighClusterWidths(f,ENDCAP,true);
   
   
   
   
}

// 
