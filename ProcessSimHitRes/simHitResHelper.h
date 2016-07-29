
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
#include <stdint.h>

#include <TFile.h>
#include <TSystem.h>
#include <TObjArray.h>
#include <TVector2.h>
#include <TMath.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TH1D.h>
#include <TTree.h>
#include <TF1.h>

using namespace std;




///////////////////////////////////////// GEOMETRY HELPERS /////////////////////////////////////////

enum SubDetector { UNKNOWN=0, TIB=3, TID=4, TOB=5, TEC=6 , BARREL=7, ENDCAP=8, ALL=9 };
enum ModuleGeometry {UNKNOWNGEOMETRY, IB1, IB2, OB1, OB2, W1A, W2A, W3A, W1B, W2B, W3B, W4, W5, W6, W7};
enum ModuleType {MONO,STEREO,BOTH};

SubDetector subDetector(unsigned int id_){
  int subdetID = ((id_>>25)&0x7);
  if( subdetID == static_cast<int>(TIB) ) {
    return TIB;
  } else if ( subdetID == static_cast<int>(TID)  ) {
    return TID;
  } else if ( subdetID == static_cast<int>(TOB)  ) {
    return TOB;
  } else if ( subdetID == static_cast<int>(TEC)  ) {
    return TEC;
  }
  return UNKNOWN;

}



ModuleGeometry getModuleGeometry(unsigned int id_)  {
  static const unsigned layerStartBit_ = 14;
  static const unsigned layerMask_ = 0x7;
  static const unsigned ringStartBitTID_= 9;
  static const unsigned ringMaskTID_= 0x3;
  static const unsigned ringStartBitTEC_= 5;
  static const unsigned ringMaskTEC_= 0x7;
  switch(subDetector(id_)) {
  case TIB: return int((id_>>layerStartBit_) & layerMask_)<3? IB1 : IB2;
  case TOB: return int((id_>>layerStartBit_) & layerMask_)<5? OB2 : OB1;
  case TID: switch ((id_>>ringStartBitTID_) & ringMaskTID_) {
    case 1: return W1A;
    case 2: return W2A;
    case 3: return W3A;
    }
  case TEC: switch ((id_>>ringStartBitTEC_) & ringMaskTEC_) {
    case 1: return W1B;
    case 2: return W2B;
    case 3: return W3B;
    case 4: return W4;
    case 5: return W5;
    case 6: return W6;
    case 7: return W7;
    }
  case UNKNOWN: default: return UNKNOWNGEOMETRY;
  }
}

bool getType(unsigned int id_)  {
  static const uint32_t sterStartBit_ = 0;
  static const uint32_t sterMask_ = 0x3;
  if ( ((id_>>sterStartBit_ ) & sterMask_ ) == 1 ) {
    return true;
  } else { return false; }
}

TString getModuleGeometryTitle(ModuleGeometry geo){
  switch(geo){
  case IB1: return "IB1";
  case IB2: return "IB2";
  case OB1: return "OB1";
  case OB2: return "OB2";
  case W1A: return "W1A";
  case W2A: return "W2A";
  case W3A: return "W3A";
  case W1B: return "W1B";
  case W2B: return "W2B";
  case W3B: return "W3B";
  case W4 : return "W4";
  case W5 : return "W5";
  case W7 : return "W7";
  default: return "UNKNOWNGEOMETRY";
  }
}

TString getModuleTypeTitle(ModuleType type){
  switch(type){
  case MONO: return "MONO";
  case STEREO: return "STEREO";
  case BOTH: return "BOTH";
  default: return "UNKNOWNTYPE";
  }
}

TString getDetectorTitle(SubDetector det){
  switch(det){
  case TIB: return "TIB";
  case TID: return "TID";
  case TOB: return "TOB";
  case TEC: return "TEC";
  case BARREL: return "BARREL";
  case ENDCAP: return "ENDCAP";
  case ALL: return "ALL";
  default: return "UNKNOWNDET";
  }
}






///////////////////////////////////////// Binning storage /////////////////////////////////////////

class ResContainer{
public:
  //Storage classes
  class ResUnit{
  public:
    int nBins;
    double * trackWidths;
    double * avgWidths  ;
    double * trueRes    ;
    double * pairRes    ;
    double * trackRes   ;
    double * trueResErr    ;
    double * pairResErr    ;
    double * trackResErr   ;

    // Histograms + values used to compute the final numbers
    TH1D* ptHistos        ;
    TH1D* trueResHistos   ;
    TH1D* trackResHistos  ;
    TH1D* doubleDiffHistos;
    TH1D* meanPredEXHistos;

    int * nInputs;
    int * nPairs;


    ResUnit(unsigned int numBins,const double *bins) : nBins(numBins){

      trackWidths      = new double[nBins + 1];
      avgWidths        = new double[nBins];
      trueRes          = new double[nBins];
      pairRes          = new double[nBins];
      trackRes         = new double[nBins];
      trueResErr       = new double[nBins];
      pairResErr       = new double[nBins];
      trackResErr      = new double[nBins];
      ptHistos         = new TH1D[nBins];
      trueResHistos    = new TH1D[nBins];
      trackResHistos   = new TH1D[nBins];
      doubleDiffHistos = new TH1D[nBins];
      meanPredEXHistos = new TH1D[nBins];
      nInputs          = new int[nBins];
      nPairs           = new int[nBins];

      for(unsigned int iB = 0; iB < (unsigned int)(nBins); ++iB){
        trackWidths     [iB] =  bins[iB];
        avgWidths       [iB] = -1;
        trueRes         [iB] = -1;
        pairRes         [iB] = -1;
        trackRes        [iB] = -1;
        trueResErr      [iB] = -1;
        pairResErr      [iB] = -1;
        trackResErr     [iB] = -1;
        ptHistos        [iB].SetBins(500,0,1000);
        trueResHistos   [iB].SetBins(300,-6,6);
        trackResHistos  [iB].SetBins(300,-6,6);
        doubleDiffHistos[iB].SetBins(300,-6,6);
        meanPredEXHistos[iB].SetBins(300,0,2);
        nInputs         [iB] = 0;
        nPairs          [iB] = 0;
      }
      trackWidths[nBins] =  bins[nBins];
    }

    ~ResUnit(){
      delete[] trackWidths ;
      delete[] avgWidths   ;
      delete[] trueRes     ;
      delete[] pairRes     ;
      delete[] trackRes    ;
      delete[] trueResErr     ;
      delete[] pairResErr     ;
      delete[] trackResErr    ;
      delete[] ptHistos        ;
      delete[] trueResHistos   ;
      delete[] trackResHistos  ;
      delete[] doubleDiffHistos;
      delete[] meanPredEXHistos;
      delete[] nInputs;
      delete[] nPairs;
    }

    int getBin(double val){
//      return TMath::Max(0, TMath::Min(nBins-1, static_cast<Int_t>(TMath::BinarySearch(nBins, &(trackWidths[0]), val))));
      int fillBin = 0;
      cout << "Number of bins=" <<  nBins<< endl;
      for(unsigned int iB = 0; iB < nBins; ++iB){
	cout << "Track width bin= " << trackWidths[iB] << endl;
        if(iB ==  nBins -1) fillBin = iB;
        else if(val >= trackWidths[iB] &&  val < trackWidths[iB + 1] ){
          fillBin = iB;
          break;
        }
      }
      return fillBin;


    }

  };
  
  class ResBin{
  public:
    SubDetector    detector_  ;
    ModuleGeometry geometry_  ;
    ModuleType     type_      ;
    int            minCluster_;
    int            maxCluster_;

    ResBin(SubDetector detector,  ModuleType type, double minCluster, double maxCluster) :
      detector_  (detector),
      geometry_  (UNKNOWNGEOMETRY),
      type_      (type),
      minCluster_(minCluster),
      maxCluster_(maxCluster)
    {}
    ResBin(ModuleGeometry geometry,  ModuleType type, int minCluster, int maxCluster) :
      detector_  (UNKNOWN),
      geometry_  (geometry),
      type_      (type),
      minCluster_(minCluster),
      maxCluster_(maxCluster)
    {}

    bool isBin(unsigned int id, int clusW){
      if(detector_ != UNKNOWN){
        SubDetector sub = subDetector(id);
        if(detector_ == ALL){

        }
        else if(detector_ == BARREL){
          if (sub != TIB && sub != TOB)
            return false;
        }
        else if(detector_ == ENDCAP){
          if(sub != TID && sub != TEC)
            return false;
        }
        else if (detector_ != sub)
          return false;
      }
      if(geometry_ != UNKNOWNGEOMETRY && geometry_ != getModuleGeometry(id) )
        return false;
      if(type_ != BOTH){
        if(type_ == STEREO  && getType(id) == false )return false;
        if(type_ == MONO   && getType(id) == true )  return false;
      }
      if(minCluster_ >= 0 && clusW < minCluster_ )
        return false;
      if(maxCluster_ >= 0 && clusW > maxCluster_ )
        return false;
      return true;
    }

    TString getBinTitle(){
      TString title;
      if(detector_ != UNKNOWN)
        title += getDetectorTitle(detector_);
      if(geometry_ != UNKNOWNGEOMETRY)
        title += getModuleGeometryTitle(geometry_);
      if(type_ != BOTH){
        title += "_";
        title += getModuleTypeTitle(type_);
      }
        if(minCluster_ !=maxCluster_ )
          title += TString::Format("_%i_%i",minCluster_,maxCluster_);
        else
          title += TString::Format("_%i",minCluster_);

      return title;
    }


  };


  //Now for the actual container class
  vector<ResUnit *> resUnits;
  vector<ResBin * >  resBins;

  ~ResContainer(){
    for(unsigned int iB = 0; iB < resUnits.size(); ++iB){
      delete resUnits[iB];
      delete resBins[iB];
    }
  }


  //add a bin
  void inputBin(SubDetector detector,  ModuleType type, double minCluster, double maxCluster, int nBins, double* bins){
    resBins.push_back( new ResBin( detector, type, minCluster, maxCluster ) );
    resUnits.push_back( new ResUnit( nBins, bins ) );
  }
  void inputBin(ModuleGeometry geometry,  ModuleType type, int minCluster, int maxCluster, int nBins, double* bins){
    resBins.push_back( new ResBin( geometry, type, minCluster, maxCluster ) );
    resUnits.push_back( new ResUnit( nBins, bins ) );
  }

  //get bin location
  int getResBin (unsigned int id, int clusW){
    for(unsigned int iB = 0; iB < resBins.size(); ++iB)
      if(resBins[iB]->isBin(id,clusW))
        return iB;
    return -1;
  }

  int getTrackBin(unsigned int resBin, double trackW){
    if(resBin >= resUnits.size()) return -1;
    return resUnits[resBin]->getBin(trackW);
  }

  //GetResults --- Returns the average widths...the actual x axis
  bool getTrackAxis(unsigned int resBin, int& numBins, double* & widths){
    if(resBin >= resBins.size()) return false;
    numBins = resUnits[resBin]->nBins;
    widths = resUnits[resBin]->avgWidths;
    return true;
  }
  bool getTrueResults(unsigned int resBin,  double* & vals, double* & errs){
    if(resBin >= resBins.size()) return false;
    vals = resUnits[resBin]->trueRes;
    errs = resUnits[resBin]->trueResErr;
    return true;
  }
  bool getPairResults(unsigned int resBin, double* & vals, double* & errs){
    if(resBin >= resBins.size()) return false;
    vals = resUnits[resBin]->pairRes;
    errs = resUnits[resBin]->pairResErr;
    return true;
  }
  bool getTrackResults(unsigned int resBin, double* & vals, double* & errs){
    if(resBin >= resBins.size()) return false;
    vals = resUnits[resBin]->trackRes;
    errs = resUnits[resBin]->trackResErr;
    return true;
  }
};


///////////////////////////////////////// Read and process a tree /////////////////////////////////////////
void getWidthByRMS(const TH1* hist, double& width, double& error, double maxDev = -1){
   double mu = 0;
   double total = 0;

   for(int iB = 0; iB <= hist->GetNbinsX() +1; ++iB){
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

   for(int iB = 0; iB <= hist->GetNbinsX() +1; ++iB){
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


void fillRes(const TString name, TTree * tree, ResContainer& container, const double maxDev = 0){
  //Fill the values from each ttree entry
  float        momentum      ;
  int          numHits       ;
  float        trackChi2     ;
  unsigned int detID1        ;
  float        pitch1        ;
  unsigned int clusterW1     ;
  float        expectedW1    ;
  float        trueRes1      ;
  bool         atEdge1       ;
  float        simpleRes1    ;
  unsigned int detID2        ;
  //float        pitch2        ;
  unsigned int clusterW2     ;
  float        expectedW2    ;
  bool         atEdge2       ;
  float        pairPath      ;
  float        hitDX         ;
  float        trackDX       ;
  float        trackDXE      ;

  bool includeTruth = tree->FindBranch("trueRes1"  ) >= 0;

  tree->SetBranchAddress("momentum"   ,&momentum  );
  tree->SetBranchAddress("numHits"    ,&numHits   );
  tree->SetBranchAddress("trackChi2"  ,&trackChi2 );
  tree->SetBranchAddress("detID1"     ,&detID1    );
  tree->SetBranchAddress("ptich1"     ,&pitch1    );
  tree->SetBranchAddress("clusterW1"  ,&clusterW1 );
  tree->SetBranchAddress("expectedW1" ,&expectedW1);
  if(includeTruth) tree->SetBranchAddress("trueRes1" ,&trueRes1  );
  tree->SetBranchAddress("atEdge1"    ,&atEdge1   );
  tree->SetBranchAddress("simpleRes1" ,&simpleRes1);
  tree->SetBranchAddress("detID2"     ,&detID2    );
  //tree->SetBranchAddress("ptich2"     ,&pitch2    );
  tree->SetBranchAddress("clusterW2"  ,&clusterW2 );
  tree->SetBranchAddress("expectedW2" ,&expectedW2);
  tree->SetBranchAddress("atEdge2"    ,&atEdge2   );
  tree->SetBranchAddress("pairPath"   ,&pairPath  );
  tree->SetBranchAddress("hitDX"      ,&hitDX     );
  tree->SetBranchAddress("trackDX"    ,&trackDX   );
  tree->SetBranchAddress("trackDXE"   ,&trackDXE  );




  for (Long64_t iEntry = 0;  tree->LoadTree(iEntry) >= 0; ++iEntry)
  {
    //if (iEntry >10) break;
    tree->GetEntry(iEntry);

    if(momentum < 10) continue;
    cout << "Entry is= " << iEntry << endl;
    cout << "Found a hit along a track with momentum= " << momentum << " and number of nHits along a track= " << numHits << endl;

    int resBin1 = container.getResBin(detID1,clusterW1);
    if (resBin1 !=-1) 
      cout << "\t detector cluster found in detector= " << detID1 << " and cluster width= " << clusterW1 << " -> bin width= " << resBin1 << endl;

    if(resBin1 < 0) continue;

    ResContainer::ResUnit& resUnit1 = *container.resUnits[resBin1];
    cout << "Res Unit size= " << resUnit1.nBins << endl;
    int trackBin1 = resUnit1.getBin(expectedW1);

    //int trackBin1 = container.getTrackBin(resBin1,expectedW1);

    cout << "\t detector cluster found in detector= " << getDetectorTitle(subDetector(detID1)) 
	 << " and expected width= " << expectedW1 
	 << " -> bin track width= " << trackBin1 << endl;

    if(clusterW1 <= 4 && expectedW1 > 2)
      cout << getDetectorTitle(subDetector(detID1)) << resBin1 <<" "<<trackBin1<<" "<< clusterW1 <<" "<< expectedW1 <<endl;

    resUnit1.avgWidths[trackBin1] +=  expectedW1;
    resUnit1.nInputs[trackBin1]   +=  1;
    if(includeTruth) resUnit1.trueResHistos[trackBin1].Fill(trueRes1);
    resUnit1.trackResHistos[trackBin1].Fill(simpleRes1/pitch1);
    resUnit1.ptHistos[trackBin1].Fill(momentum);

    //Fill pair information
    if(detID2 == 0) continue;
    int resBin2 = container.getResBin(detID2,clusterW2);
    if(resBin1 != resBin2) continue;
    int trackBin2 = resUnit1.getBin(expectedW2);
    if(trackBin1 != trackBin2) continue;
    //if(pitch1 != pitch2) continue;

    //Quality cuts
    if(trackChi2 < .001) continue;
    if(numHits < 6) continue;
    if ( trackDXE>0.0025 ) continue;
    if(atEdge1 || atEdge2) continue;
    if ( fabs(pairPath) > 7 ) continue;



    resUnit1.doubleDiffHistos[trackBin1].Fill((hitDX - trackDX)/pitch1 );
    resUnit1.meanPredEXHistos[trackBin1].Fill(trackDXE/pitch1 );
    resUnit1.nPairs[trackBin1]   +=  1;

  }

  


  //Now process

  TF1 * gausFitter =new TF1("gausFitter","gaus",-6,6);

  for(unsigned int iB = 0; iB < container.resBins.size(); ++iB){
    ResContainer::ResUnit& resUnit = *container.resUnits[iB];
    for(unsigned int iW = 0; iW < (unsigned int)(resUnit.nBins); ++iW){
      if(resUnit.nInputs[iW] == 0) continue;
      resUnit.avgWidths[iW] /=  resUnit.nInputs[iW];

      //True Res
      if(resUnit.trueResHistos[iW].GetEntries() != 0){
        //Use central 68% for resolution
        if(false){
//          TH1D* trueResHisto = &resUnit.trueResHistos[iW];
//          const double xMin = trueResHisto->GetXaxis()->GetXmin();
//          const double xMax = trueResHisto->GetXaxis()->GetXmax();
//          const int   nBins = trueResHisto->GetXaxis()->GetNbins();
//
//          const double rangeMin = maxDev > 0 ? -1*maxDev : xMin ;
//          const double rangeMax = maxDev > 0 ?    maxDev : xMax ;
//
//          const int firstBin = maxDev > 0 ? trueResHisto->FindFixBin(rangeMin) : 1 ;
//          const int lastBin  = maxDev > 0 ? trueResHisto->FindFixBin(rangeMax) : nBins ;
//          const int nEntries = trueResHisto->Integral(firstBin,lastBin);
//          const double underNEntries =  maxDev > 0 ? trueResHisto->Integral(1,firstBin -1) :0;
//
//          TH1D * histInt = (TH1D*)(Properties::makeIntegral(trueResHisto,false,false,0));
//          resUnit.trueRes[iW] = (Properties::findAbscissa(histInt->GetArray(),histInt->GetXaxis(),  .841*double(nEntries)  + underNEntries) - Properties::findAbscissa(histInt->GetArray(),histInt->GetXaxis(),  .159*double(nEntries)  + underNEntries ) ) /2;
//          resUnit.trueResErr[iW] = 0;
        }
        // else use rms
        else {
          double res = 0;
          double resErr = 0;
          getWidthByRMS(&resUnit.trueResHistos[iW],res,resErr, container.resBins[iB]->maxCluster_ > 2 ? -1 : 5  )  ;
          resUnit.trueRes[iW] = res;
          resUnit.trueResErr[iW] = resErr;
        }
      }


      //Track Res
      resUnit.trackRes[iW] =  resUnit.trackResHistos[iW].GetRMS();

      //Pair Res
      if(resUnit.nPairs[iW] == 0) continue;
      gausFitter->SetParameters(10,0,.3);
      resUnit.doubleDiffHistos[iW].Fit(gausFitter,"QS");
      double doubleDiffSig = gausFitter->GetParameter(2);
      double doubleDiffSigE = gausFitter->GetParError(2);
      double meanPredE      = resUnit.meanPredEXHistos[iW].GetMean();
      double meanPredERMS   = resUnit.meanPredEXHistos[iW].GetRMS();
      resUnit.pairRes[iW] = TMath::Sqrt( doubleDiffSig*doubleDiffSig - meanPredE*meanPredE ) /TMath::Sqrt(2);
      resUnit.pairResErr[iW]  = TMath::Sqrt(doubleDiffSigE*doubleDiffSigE + meanPredERMS*meanPredERMS)/TMath::Sqrt(2);

//      new TCanvas();
//      resUnit.doubleDiffHistos[iW].SetTitle(container.resBins[iB]->getBinTitle() + TString::Format("_%u",iW));
//      resUnit.doubleDiffHistos[iW].Draw();
//      new TCanvas();
//      resUnit.meanPredEXHistos[iW].SetTitle(container.resBins[iB]->getBinTitle() + TString::Format("_%u",iW));
//      resUnit.meanPredEXHistos[iW].Draw();
//      new TCanvas();


//      new TCanvas();
//      resUnit.trueResHistos[iW].SetTitle(container.resBins[iB]->getBinTitle());
//      resUnit.trueResHistos[iW].Draw();
//      new TCanvas();

    }

  }

  delete gausFitter;

}




//
//
//
//void makeGraphs(const TString name, const vector<double>& trueRes, const vector<double>& pairRes, vector<double>& pairResE, const vector<double>& fastRes , const vector<double>& averageX){
//  static Color_t colors[] = {kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};
//  TMultiGraph * mg = new TMultiGraph("mg","mg");
//  TGraph * graphTrue = new TGraphErrors();
//  TGraphErrors * graphPair = new TGraphErrors();
//  TGraph * graphSimple = new TGraphErrors();
//  for(unsigned int iL = 0; iL < trueRes.size(); ++iL){
//    if(averageX[iL] == 0) continue;
//    graphTrue->SetPoint(iL ,averageX[iL],trueRes[iL]);
//    graphPair->SetPoint(iL ,averageX[iL],pairRes[iL]);
//    graphPair->SetPointError(iL ,0,pairResE[iL]);
//    graphSimple->SetPoint(iL ,averageX[iL],fastRes[iL]);
//  }
//  graphTrue->SetTitle("true");graphTrue->SetMarkerStyle(20);graphTrue->SetMarkerColor(colors[0]);mg->Add(graphTrue);
//  graphPair->SetTitle("pair");graphPair->SetMarkerStyle(20);graphPair->SetMarkerColor(colors[1]);mg->Add(graphPair);
//  graphSimple->SetTitle("simple");graphSimple->SetMarkerStyle(20);graphSimple->SetMarkerColor(colors[2]);mg->Add(graphSimple);
//  TCanvas * c1 = new TCanvas();
//  mg->SetTitle(name);
//  mg->Draw("AP");
//  mg->SetMaximum(.35);
//    mg->SetMinimum(.1);
//  c1->BuildLegend();
//  mg->GetXaxis()->SetTitle("Track Width");
//  mg->GetYaxis()->SetTitle("Resolution");
//  Pint::refresh();
//
//}




// 
