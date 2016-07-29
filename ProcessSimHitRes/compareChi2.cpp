{
   // TString name = "new_mc";
   // TString name = "new_data";
   TString name = "old_mc";
   // TString name = "old_data";
   analysis->cd();
   tracks->Draw("trackChi2_2>>h(100,0,1)","momentum > 10");
   h->SetTitle(name);
   double integral = h->Integral(0,-1);
   h->Scale(1/integral);
   h->draw();
}

{
   TString files[] = {"new_mc","old_mc","new_data","old_data",""};
   p = new Plot();
   for(unsigned int iF = 0; files[iF][0]; ++iF){
      f = new TFile("HitRes_" + files[iF] + ".root");
      f->cd("analysis");
      TTree * tracks  = 0;
      gDirectory->GetObject("tracks", tracks);
      tracks->Draw("trackChi2_2>>" +files[iF]+"(30,0,1)","momentum > 10");
      TH1 * hist  = 0;
      gDirectory->GetObject(files[iF], hist);
      p->add(hist,files[iF]);
   }

   p->toUnderOverflow();
   p->normalize();
   p->SetTitle(" ");
   p->setXTitle("Chi2 probability");
   p->draw();
   
   p = (Plot*)p->Clone();
   pM = new Plot();
   pM->add(p->at("new_mc"),"new");
   pM->add(p->at("old_mc"),"old");
   
   pD = new Plot();
   pD->add(p->at("new_data"),"new");
   pD->add(p->at("old_data"),"old");
   
   
   pM->divide(*pD);
   pM->SetTitle(" ");
   pM->setXTitle("Chi2 probability");
   new TCanvas();
   pM->draw();
   
}


{
   TString files[] = {"new_mc","old_mc","new_data","old_data",""};
   p = new Plot();
   for(unsigned int iF = 0; files[iF][0]; ++iF){
      f = new TFile("HitRes_" + files[iF] + ".root");
      f->cd("analysis");
      TTree * tracks  = 0;
      gDirectory->GetObject("tracks", tracks);
      tracks->Draw("trackChi2_2>>" +files[iF]+"(30,0,1)","momentum > 10 && (eta < -1.4 || eta > 1.4)");
      // tracks->Draw("trackChi2_2>>" +files[iF]+"(30,0,1)","momentum > 10 && (eta < 1.4 && eta > -1.4)");
      
      TH1 * hist  = 0;
      gDirectory->GetObject(files[iF], hist);
      p->add(hist,files[iF]);
   }

   p->toUnderOverflow();
   p->normalize();
   p->SetTitle("|Eta| > 1.4");
   p->setXTitle("Chi2 probability");
   p->draw();
   
   p = (Plot*)p->Clone();
   pM = new Plot();
   pM->add(p->at("new_mc"),"new");
   pM->add(p->at("old_mc"),"old");
   
   pD = new Plot();
   pD->add(p->at("new_data"),"new");
   pD->add(p->at("old_data"),"old");
   
   
   pM->divide(*pD);
   pM->SetTitle(" ");
   pM->setXTitle("Chi2 probability");
   new TCanvas();
   pM->draw();
   
}