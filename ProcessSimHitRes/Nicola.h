//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May 19 20:12:35 2016 by ROOT version 6.02/13
// from TTree hits/Hits
// found on file: HitRes.root
//////////////////////////////////////////////////////////

#ifndef Nicola_h
#define Nicola_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class Nicola {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         momentum;
   Int_t           numHits;
   Float_t         trackChi2;
   UInt_t          detID1;
   Float_t         ptich1;
   UInt_t          clusterW1;
   Float_t         expectedW1;
   Float_t         trueRes1;
   Bool_t          atEdge1;
   Float_t         simpleRes1;
   UInt_t          detID2;
   UInt_t          clusterW2;
   Float_t         expectedW2;
   Bool_t          atEdge2;
   Float_t         pairPath;
   Float_t         hitDX;
   Float_t         trackDX;
   Float_t         trackDXE;
   Float_t         trackParamX;
   Float_t         trackParamY;
   Float_t         trackParamDXDZ;
   Float_t         trackParamDYDZ;
   Float_t         trackParamXE;
   Float_t         trackParamYE;
   Float_t         trackParamDXDZE;
   Float_t         trackParamDYDZE;

   // List of branches
   TBranch        *b_momentum;   //!
   TBranch        *b_numHits;   //!
   TBranch        *b_trackChi2;   //!
   TBranch        *b_detID1;   //!
   TBranch        *b_pitch1;   //!
   TBranch        *b_clusterW1;   //!
   TBranch        *b_expectedW1;   //!
   TBranch        *b_trueRes1;   //!
   TBranch        *b_atEdge1;   //!
   TBranch        *b_simpleRes1;   //!
   TBranch        *b_detID2;   //!
   TBranch        *b_clusterW2;   //!
   TBranch        *b_expectedW2;   //!
   TBranch        *b_atEdge2;   //!
   TBranch        *b_pairPath;   //!
   TBranch        *b_hitDX;   //!
   TBranch        *b_trackDX;   //!
   TBranch        *b_trackDXE;   //!
   TBranch        *b_trackParamX;   //!
   TBranch        *b_trackParamY;   //!
   TBranch        *b_trackParamDXDZ;   //!
   TBranch        *b_trackParamDYDZ;   //!
   TBranch        *b_trackParamXE;   //!
   TBranch        *b_trackParamYE;   //!
   TBranch        *b_trackParamDXDZE;   //!
   TBranch        *b_trackParamDYDZE;   //!

   Nicola(TTree *tree=0);
   virtual ~Nicola();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Nicola_cxx
Nicola::Nicola(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("HitRes.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("HitRes.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("HitRes.root:/analysis");
      dir->GetObject("hits",tree);

   }
   Init(tree);
}

Nicola::~Nicola()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Nicola::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Nicola::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Nicola::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("momentum", &momentum, &b_momentum);
   fChain->SetBranchAddress("numHits", &numHits, &b_numHits);
   fChain->SetBranchAddress("trackChi2", &trackChi2, &b_trackChi2);
   fChain->SetBranchAddress("detID1", &detID1, &b_detID1);
   fChain->SetBranchAddress("ptich1", &ptich1, &b_pitch1);
   fChain->SetBranchAddress("clusterW1", &clusterW1, &b_clusterW1);
   fChain->SetBranchAddress("expectedW1", &expectedW1, &b_expectedW1);
   fChain->SetBranchAddress("trueRes1", &trueRes1, &b_trueRes1);
   fChain->SetBranchAddress("atEdge1", &atEdge1, &b_atEdge1);
   fChain->SetBranchAddress("simpleRes1", &simpleRes1, &b_simpleRes1);
   fChain->SetBranchAddress("detID2", &detID2, &b_detID2);
   fChain->SetBranchAddress("clusterW2", &clusterW2, &b_clusterW2);
   fChain->SetBranchAddress("expectedW2", &expectedW2, &b_expectedW2);
   fChain->SetBranchAddress("atEdge2", &atEdge2, &b_atEdge2);
   fChain->SetBranchAddress("pairPath", &pairPath, &b_pairPath);
   fChain->SetBranchAddress("hitDX", &hitDX, &b_hitDX);
   fChain->SetBranchAddress("trackDX", &trackDX, &b_trackDX);
   fChain->SetBranchAddress("trackDXE", &trackDXE, &b_trackDXE);
   fChain->SetBranchAddress("trackParamX", &trackParamX, &b_trackParamX);
   fChain->SetBranchAddress("trackParamY", &trackParamY, &b_trackParamY);
   fChain->SetBranchAddress("trackParamDXDZ", &trackParamDXDZ, &b_trackParamDXDZ);
   fChain->SetBranchAddress("trackParamDYDZ", &trackParamDYDZ, &b_trackParamDYDZ);
   fChain->SetBranchAddress("trackParamXE", &trackParamXE, &b_trackParamXE);
   fChain->SetBranchAddress("trackParamYE", &trackParamYE, &b_trackParamYE);
   fChain->SetBranchAddress("trackParamDXDZE", &trackParamDXDZE, &b_trackParamDXDZE);
   fChain->SetBranchAddress("trackParamDYDZE", &trackParamDYDZE, &b_trackParamDYDZE);
   Notify();
}

Bool_t Nicola::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Nicola::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Nicola::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Nicola_cxx
