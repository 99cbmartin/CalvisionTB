void plotting(){
   Int_t NomAmp[7]={16,19,24,33,45,63,87};
    TCanvas* canvasC = new TCanvas("Individual Channels", "Individual Channels", 4000,3600);
    TCanvas* canvasA = new TCanvas("Individual Nominal Amplitudes","Individual Nominal Amplitudes",4000,3600);
    canvasC->Divide(3,3,0,0);//an extra Row just for the legend
    canvasC->SetLogx();
    canvasA->Divide(3,3,0,0);
    canvasA->SetLogx();
    TLegend* legendA = new TLegend(0,1,1,.1);
    TLegend* legendC = new TLegend(0,1,1,.1);
    TMultiGraph* mgA[7];
    TMultiGraph* mgC[7];


for (Int_t chindex = 0; chindex < 7; ++chindex){//Filling the Channel plots with each Amplitude
        canvasC->cd(chindex+1);
        canvasC->SetLogx();
        mgC[chindex] = new TMultiGraph();
        for(Int_t Ampindex = 0; Ampindex <7; ++Ampindex){
                timplots[chindex][Ampindex]->SetLogx();
		timplots[chindex][Ampindex]->SetLineColor(icol[Ampindex]);
                timplots[chindex][Ampindex]->SetMarkerColor(icol[Ampindex]);
       	        mgC[chindex]->Add(timplots[chindex][Ampindex]);

 }
        mgC[chindex]->Draw("APL");
	mgC[chindex]->SetTitle(Form("Channel %d",chindex));
        mgC[chindex]->GetXaxis()->SetTitle("Threshold mV*ns");
        mgC[chindex]->GetYaxis()->SetTitle("Timing Resolution ns");
}

for (Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){//Filling the Amplitude plots with each Channel
        canvasA->cd(Ampindex+1);
        mgA[Ampindex] = new TMultiGraph();
        for(Int_t chindex = 0; chindex <7; ++chindex){
                timplots[chindex][Ampindex]->SetLineColor(icol[chindex]);
                timplots[chindex][Ampindex]->SetMarkerColor(icol[chindex]);
 	        mgA[Ampindex]->Add(timplots[chindex][Ampindex]);
  }
        mgA[Ampindex]->Draw("APL");
	mgA[Ampindex]->SetTitle(Form("Amplitude %d mV",NomAmp[Ampindex])); 
        mgA[Ampindex]->GetXaxis()->SetTitle("Threshold mV*ns");
        mgA[Ampindex]->GetYaxis()->SetTitle("Timing Resolution ns");
}

for (Int_t i = 0; i < 7; ++i){
	for (Int_t j = 0; j < 7; ++j){
 		if(j == 6){
 		legendA->AddEntry(timplots[i][j],Form("Channel %d",i),"lp");
		}
	}
}

for (Int_t i = 0; i < 7; ++i){
	for (Int_t j = 0; j < 7; ++j){
 		if(i == 6){
		legendC->AddEntry(timplots[i][j],Form("Amplitude %d",NomAmp[j]),"lp");
		}
	}
}

canvasC->cd(9);
legendC->Draw();
canvasC->Print("pull/Chantitles.png");
canvasA->cd(9);
legendA->Draw();
canvasA->Print("pull/Amptitles.png");

}
