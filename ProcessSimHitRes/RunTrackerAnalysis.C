#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TROOT.h>
#include <string>
#include <iostream>
#include <TSystem.h>
#include <TH2.h>
#include "TChain.h"
#include <stdlib.h>
#include <TDCacheFile.h>
#include "simHitResParamProcessor.cc"
//#include "simHitResHelper.h"

#endif

using namespace std;

int main(int argc, char ** argv){

    
  TFile *file3;
  TTree *tree3;  
  
  file3 = TFile::Open("/lustre/cms/store/user/defilip/Tracker/StripHitResolution/HitRes_data_SingleMuon_Run2016B-v2.root");
  cout << "Read file with name: HitRes_data_SingleMuon_Run2016B-v2.root" << endl;

  //file3 = TFile::Open("/lustre/cms//store/user/defilip/Tracker/StripHitResolution/HitRes_TT_RunIISummer16.root");
  //cout << "Read file with name: HitRes_TT_RunIISummer15.root" << endl;   

  TDirectory* dir = file3->GetDirectory("analysis");
  dir->GetObject("hits", tree3);
  
  cout << "Read file with entries: " << tree3->GetEntries() << endl;

  printTotalResolution(tree3,"HitResolution");
  makeCompOutputs(tree3,"HitResolution");

  
  delete tree3;
  file3 -> Close();  

  return 0; 

}

