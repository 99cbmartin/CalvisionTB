#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream> 
#include <TGraph.h>
#include <TAxis.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
Int_t icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };
TGraph* timplots[7][7];// 7 channels, 7 Nominal Amplitudes. 
 

TLegend* legendC = new TLegend(0,1,1,.1);
 TLegend* legendA = new TLegend(0,1,1,.1);

const int nThr = 20;
double intThr[nThr];
  //for(int i=0; i<nThr; i++){
   //   intThr[i] = pow(10, 0.4 + 0.1 * i);
 // }//This is the loop for creating Threshold values. MUST change in Sasha's anlysis to see a difference. 
void initial(){
for (Int_t i=0;i<7;++i){
for (Int_t j=0;j<7;++j){
timplots[i][j] = new TGraph();
}
}  
}




void plotarray() {
    TFile* file = TFile::Open("finalroots/BGOfiltered.root");
    if (!file) {
        return;
    }
    
    TFile* file2 = TFile::Open("finalroots/PWOpar.root");
    if (!file2){
	return;
    }


for(int i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
  }//This is the loop for creating Threshold values. MUST change in Sasha's anlysis to see a difference.

 Double_t NomAmp[7]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)

 TString plotNamesA[] = {"Amp 16 mV", "Amp 19 mV", "Amp 24 mV", "Amp 33 mV", "Amp 45 mV", "Amp 63 mV", "Amp 87 mV","Legend"};


for (Int_t pulseindex =0; pulseindex < 19; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code



   for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6


	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_%d_%d",chindex,pulseindex)));
        TString legendEntryC = TString::Format("Ch %d",chindex);
	TString legendEntryA = TString::Format(plotNamesA[chindex]);
       // legend->AddEntry(Ringplots[5], legendEntry, "l");

	Double_t wind =2.0;


		for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){

			
//			timplots[chindex][Ampindex] = new TGraph();
			TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 
			if(proj->GetEntries() == 0){//sometimes, there is no data for a particular range of Amplitudes. Skip these plots.
			continue;
			}else{
			Int_t maxbin = proj->GetMaximumBin();
			Double_t peak = proj->GetBinCenter(maxbin);
			TF1* fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
                        proj->Fit(fitfunc,"RQ");
                        Double_t timeres = fitfunc->GetParameter(2);
			timplots[chindex][Ampindex]->SetPoint(pulseindex,intThr[pulseindex],timeres);//timing plots stored in an array  
	//	timplots[chindex][Ampindex]->SetMarkerSize(2.0);
		timplots[chindex][Ampindex]->SetMarkerStyle(20);
		timplots[chindex][Ampindex]->GetXaxis()->SetRangeUser(0, 300);
		timplots[chindex][Ampindex]->SetMaximum(2.2);
		timplots[chindex][Ampindex]->SetMinimum(0);
std::cout<<"For Channel:"<<chindex<<" and Amplitude:"<<NomAmp[Ampindex]<<" We will see a timing res:"<<timeres<<" for Thresh of"<<pulseindex<<std::endl;	
}


}//Close Amp Loop
}//Close Channel Loop
}//Close Pulse Loop
file->Close();
file2->Close();
}//Close the plotarray function

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
        gPad->SetLogx();
        mgC[chindex] = new TMultiGraph();
	mgC[chindex]->SetMaximum(2.2);
	mgC[chindex]->SetMinimum(0);
        for(Int_t Ampindex = 0; Ampindex <7; ++Ampindex){
                timplots[chindex][Ampindex]->SetLineColor(icol[Ampindex]);
                timplots[chindex][Ampindex]->SetMarkerColor(icol[Ampindex]);
                mgC[chindex]->Add(timplots[chindex][Ampindex]);

 }
        mgC[chindex]->Draw("APL");
        mgC[chindex]->SetTitle(Form("Channel %d",chindex));
        mgC[chindex]->GetXaxis()->SetTitle("Threshold mV*ns");
        mgC[chindex]->GetYaxis()->SetTitle("Timing Resolution ns");
}

for (Int_t i = 0; i < 7; ++i){
        for (Int_t j = 0; j < 7; ++j){
                if(i == 6){
                legendC->AddEntry(timplots[i][j],Form("Amplitude %d mV",NomAmp[j]),"lp");
                }
        }
} 
canvasC->cd(9);
legendC->Draw();
canvasC->Print("pull/ChantitlesBGO.png");
for (Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){//Filling the Amplitude plots with each Channel
        canvasA->cd(Ampindex+1);
	gPad->SetLogx();
        mgA[Ampindex] = new TMultiGraph();
        mgA[Ampindex]->SetMaximum(2.2);
	mgA[Ampindex]->SetMinimum(0);
	for(Int_t chindex = 0; chindex <7; ++chindex){
                timplots[chindex][Ampindex]->SetLineColor(icol[chindex]);
		timplots[chindex][Ampindex]->SetMarkerStyle(kFullSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
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

canvasA->cd(9);
legendA->SetLineWidth(5);
//legendA->GetListOfPrimitives()->First()->SetLineWidth(4);
//legendA->GetListofPrimitives()->Next()->SetLineWidth(4);
legendA->Draw();
canvasA->Print("pull/AmptitlesBGO.png");

}

int main(){
initial();
plotarray();
plotting();
return 0;
}
