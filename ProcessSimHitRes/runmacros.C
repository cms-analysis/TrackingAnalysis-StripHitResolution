{

  TFile *_file0 = TFile::Open("HitRes.root");
  TDirectory* dir = _file0->GetDirectory("analysis");
  dir->ls();
  TTree *hitstree;
  dir->GetObject("hits", hitstree);
  hitstree->Scan("*");
  
  .L simHitResParamProcessor.cc+
  printTotalResolution(hitstree,"HitResolution");
  makeCompOutputs(hitstree,"HitResolution");
}
