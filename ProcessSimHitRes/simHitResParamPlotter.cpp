{
   Color_t colors[] = {kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};
   
   vector<TFile*> files;
   files.push_back(new TFile("MCPt100_compOutputs.root"));
   files.push_back(new TFile("MCPtExp_compOutputs.root"));
   TString FILES[] = {"MC 100","MC Exp",""};
   // vector<TFile*> files;
   // files.push_back(new TFile("MCAR_compOutputs.root"));
   // files.push_back(new TFile("data_compOutputs.root"));
   // TString FILES[] = {"MCAR","DataAR",""};
   
   
   // TString RES[] = {"trueRes","pairRes",""};
   TString RES[] = {"trueRes",""};
   
   TString TYPE[]  = {"TEC_1_4","","IB2_1_4","","IB1_MONO","IB1_STEREO","IB2","OB1","OB2_MONO","OB2_STEREO",""};

   for(unsigned int iT = 0; TYPE[iT][0]; ++iT){
      TMultiGraph * mg = new TMultiGraph(TYPE[iT],TYPE[iT]);
      int iC = 0;      
      for(unsigned int iF = 0; FILES[iF][0]; ++iF){
         for(unsigned int iR = 0; RES[iR][0]; ++iR){
         TGraph* graph = 0;
         files[iF]->GetObject(TYPE[iT] +"_" + RES[iR], graph);
         if(graph == 0) continue;
         graph->SetTitle(FILES[iF]+"_" + RES[iR]);
         graph->SetMarkerStyle(20);
         graph->SetMarkerColor(colors[iC]);
         iC++;
         mg->Add(graph);
      }
      }
      TCanvas * c1 = new TCanvas();
      mg->SetMaximum(.35);
      mg->SetMinimum(.1);
      mg->Draw("AP");
      
      c1->BuildLegend();
      mg->GetXaxis()->SetTitle("Track Width");
      mg->GetYaxis()->SetTitle("Resolution");
      
   }
   Pint::refresh();
   
}


{
   Color_t colors[] = {kBlack,kBlue,kRed,kGreen,kGray,kMagenta,kBlue -10};
   TFile  *f1 = new TFile("MCPt100_paramOutputs.root");
   // TString DET[] = {"IB1","IB2","OB1","OB2",""};
   // TString DET[] = {"BARREL","TIB","TOB",""}
   TString DET[] = {"UNKNOWNDET","BARREL","ENDCAP","TEC","TID",""}

   //
   // TString DET[] = {"TOB","IB2","OB1","OB2","TID","TEC",""}
   
   // TString DET[] = {"TIB","TOB","W1A", "W2A", "W3A", "W1B", "W2B", "W3B", "W4", "W5", "W6", "W7",""}
   // TString CLUS[]  = {"1_4","1","2_3","4",""};
   

   // TString CLUS[]  = {"1","2","3","4",""};
      // TString CLUS[]  = {"2_4",""};
   // TString CLUS[]  = {"2_4","1","2","3","4",""};
      TString CLUS[]  = {"1_4","2_4","5","6","7",""};
   
   // TString CLUS[]  = {"1_4","5","6","7","","8_-1",""};

   for(unsigned int iD = 0; DET[iD][0]; ++iD){
      TMultiGraph * mg = new TMultiGraph(DET[iD],DET[iD]);
      int iCl = 0;      
      for(unsigned int iC = 0; CLUS[iC][0]; ++iC){
         TGraph* graph = 0;
         f1->GetObject(DET[iD] +"_" + CLUS[iC] +"_trueRes", graph);
         if(graph == 0) continue;
         graph->SetTitle(CLUS[iC]);
         graph->SetMarkerStyle(20);
         graph->SetMarkerColor(colors[iCl]);
         iCl++;
         mg->Add(graph);
      }
      if(iCl == 0) continue;
      TCanvas * c1 = new TCanvas();
      // mg->SetMaximum(.35);
      mg->SetMinimum(.1);
      mg->Draw("AP");
      
      c1->BuildLegend();
      mg->GetXaxis()->SetTitle("Track Width");
      mg->GetYaxis()->SetTitle("Resolution");
      
   }
   Pint::refresh();
   
}

//GetFIT
{
   TFile  *f1 = new TFile("MCPt100_paramOutputs.root");
   TGraphErrors* graph;
   f1->GetObject("ALL_1_4_trueRes", graph);
   graph->Draw("AP");
   TF1 * func = new TF1("fit","[0]*x*exp(-1*x*[1]) +[2]")      ;
   func->SetParameters(-.30,.90,.30);
      
      
   graph->Fit(func,"QS");      
   

   //Compare params

   {
      TF1 * func = new TF1("fit","[0]*x*exp(-1*x*[1]) +[2]",0,3)      ;
      func->SetParameters(-.326,.618,.30);
      func->SetLineColor(kBlue);
      
      TF1 * func2 = new TF1("fit","[0]*x*exp(-1*x*[1]) +[2]",0,3)      ;
      func2->SetParameters(-0.339,0.90,.279);
      
      
      
   TCanvas * c1 = new TCanvas();
   TLegend * legend = new TLegend(.6, .7, .9, .85, 0);
   legend->SetFillColor(0);
   legend->AddEntry(func, "New Par.","l");
   legend->AddEntry(func2, "Old Par.","l");
   
   func->SetTitle("Error parameterization for hits with < 5 strips cluster size");
   func->GetXaxis()->SetTitle("Expected cluster width from track angle");
   func->GetYaxis()->SetTitle("RecHit resolution");
   
   c1 = new TCanvas();
   func->Draw();
   // func2->Draw("SAME");
   legend->Draw();
      
      // func->Draw();
   
   }
   
   //Compare params at high width
   
   {
      TF1 * tib = new TF1("tob","[0]+x*[1]",5,10)      ;
      tib->SetParameters(-.742,.202);
      tib->SetLineColor(kBlue);
      
      TF1 * tob = new TF1("tob","[0]+x*[1]",5,10)      ;
      tob->SetParameters(-1.026,.253);
      tob->SetLineColor(kGreen);
      
      TF1 * tid = new TF1("tid","[0]+x*[1]",5,10)      ;
      tid->SetParameters(-1.427,.433);
      tid->SetLineColor(kRed);
      
      TF1 * tec = new TF1("tec","[0]+x*[1]",5,10)      ;
      tec->SetParameters(-1.885,.471);
      tec->SetLineColor(kYellow);
      
      TF1 * old = new TF1("old","[0]+x/sqrt([1])",4,10)      ;
      old->SetParameters(0,12);
      old->SetLineColor(kGray);
      
      
      
   TCanvas * c1 = new TCanvas();
   TLegend * legend = new TLegend(.6, .7, .9, .85, 0);
   legend->SetFillColor(0);
   legend->AddEntry(tib, "TIB Param.","l");
   legend->AddEntry(tob, "TOB Param.","l");
   legend->AddEntry(tid, "TID Param.","l");
   legend->AddEntry(tec, "TEC Param.","l");
   legend->AddEntry(old, "Old Param.","l");

   
   tib->SetTitle("Error parameterization for high cluster widths");
   tib->GetXaxis()->SetTitle("Cluster width");
   tib->GetYaxis()->SetTitle("RecHit resolution");
   
   c1 = new TCanvas();
   // old->Draw("");
   tib->Draw("");
   tob->Draw("SAME");
   tid->Draw("SAME");
   tec->Draw("SAME");

   legend->Draw();
      
      // func->Draw();
   
   }




//Fit params
{
   TFile  *f1 = new TFile("MCPt100_paramOutputs.root");
   // TGraphErrors* graph;
   // f1->GetObject("ENDCAP_1_4_trueRes", graph);
   // double x,y;
   // graph->GetPoint(0,x,y);
   // cout << endl << x <<" "<<y <<" "<< graph->GetErrorX(0)<<endl;
 
   TString DET[] = {"TIB","TOB","TID","TEC",""};
   TF1 * func = new TF1("fit","[0] + [1]*x")      ;
 
   for(unsigned iD = 0; DET[iD][0]; ++iD){
      TGraphErrors * graphComp = new TGraphErrors();
      
      int nPt  = 0;
      for(unsigned int iB = 5; iB <= 10; ++iB){
         TGraphErrors* graph == 0;
         f1->GetObject(DET[iD] +  TString::Format("_%u_trueRes",iB), graph);
         if(graph == 0 ||graph->GetN() == 0 ) continue;
         double x,y;
         graph->GetPoint(0,x,y);
         double err = graph->GetErrorY(0);
         
         graphComp->SetPoint(nPt ,iB,y);
         graphComp->SetPointError(nPt ,0,err);        
         nPt++;
         
      }
      
      graphComp->SetTitle(DET[iD]);
      graphComp->Fit(func);
      
      new TCanvas();
      graphComp->Draw("AP");
      
      
   }
   
}