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
TGraph* unplots[7][7];// 7 channels, 7 Nominal Amplitudes. 
TGraph* filtplots[7][7]; 

TLegend* legendC = new TLegend(0.1,0.7,0.4,0.9);//change sizing

const int nThr = 20;
double intThr[nThr];


void initial(){
for (Int_t i=0;i<7;++i){
for (Int_t j=0;j<7;++j){
unplots[i][j] = new TGraph();
filtplots[i][j] = new TGraph();
}
}  
}




void plotarray() {
    

    TFile* file = TFile::Open("finalroots/PWOpar.root");
    if (!file) {
        return;
    }
    
    TFile* file2 = TFile::Open("finalroots/PWOparfilter.root");
    if (!file2){
	return;
    }


for(int i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
  }




 Double_t NomAmp[7]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)

 TString plotNamesA[] = {"Amp 16 mV", "Amp 19 mV", "Amp 24 mV", "Amp 33 mV", "Amp 45 mV", "Amp 63 mV", "Amp 87 mV","Legend"};


for (Int_t pulseindex =0; pulseindex < 19; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code



   for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6


	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_%d_%d",chindex,pulseindex)));
	TH2D* filtheatmap = dynamic_cast<TH2D*>(file2->Get(Form("h201_%d_%d",chindex,pulseindex)));
	TString legendEntryA = TString::Format(plotNamesA[chindex]);

	Double_t wind =2.0;


		for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){

			
			TF1* fitfunc;
			TF1* filtfitfunc;
		TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 
		TH1D* filtproj = (TH1D*)filtheatmap->ProjectionY("filtproj",filtheatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),filtheatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind)));

			if(proj->GetEntries() == 0){//sometimes, there is no data for a particular range of Amplitudes. Skip these plots.
			continue;
			}else{
			Int_t maxbin = proj->GetMaximumBin();
			Double_t peak = proj->GetBinCenter(maxbin);
			//if (pulseindex < 11){
			//fitfunc = new TF1("fitfunc","gaus",peak-.6,peak+.6);
			//}else{
			fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
			//}

			if (filtproj->GetEntries() == 0){
			continue;
			 }else{
                        Int_t filtmaxbin = filtproj->GetMaximumBin();
                        Double_t filtpeak = filtproj->GetBinCenter(filtmaxbin);
                       // if (pulseindex < 11){
                        //filtfitfunc = new TF1("filtfitfunc","gaus",peak-.6,peak+.6);
                        //}else{
                        filtfitfunc = new TF1("filtfitfunc","gaus",peak-5,peak+5);
                        //}	



                        proj->Fit(fitfunc,"RQ");
			filtproj->Fit(filtfitfunc,"RQ");
                        Double_t timeres = fitfunc->GetParameter(2);
			Double_t filttimeres = filtfitfunc->GetParameter(2);
			unplots[chindex][Ampindex]->SetPoint(pulseindex,intThr[pulseindex],timeres);//timing plots stored in an array  
			filtplots[chindex][Ampindex]->SetPoint(pulseindex, intThr[pulseindex],filttimeres);
	//	timplots[chindex][Ampindex]->SetMarkerSize(2.0);
		unplots[chindex][Ampindex]->SetMarkerStyle(20);
		unplots[chindex][Ampindex]->GetXaxis()->SetRangeUser(0, 300);
		unplots[chindex][Ampindex]->SetMaximum(2.2);
		unplots[chindex][Ampindex]->SetMinimum(0);
		filtplots[chindex][Ampindex]->SetMarkerStyle(20);
                filtplots[chindex][Ampindex]->GetXaxis()->SetRangeUser(0, 300);
                filtplots[chindex][Ampindex]->SetMaximum(2.2);
                filtplots[chindex][Ampindex]->SetMinimum(0);
std::cout<<"For Channel:"<<chindex<<" and Amplitude:"<<NomAmp[Ampindex]<<" We will see a timing res:"<<timeres<<" for Thresh of"<<pulseindex<<std::endl;	
}
}
}//Close Amp Loop
}//Close Channel Loop
}//Close Pulse Loop
file->Close();
file2->Close();
}//Close the plotarray function

void plotting(){
Int_t NomAmp[7]={16,19,24,33,45,63,87};
    TLegend* legendA = new TLegend(0.2, 0.7, 0.5, 0.9);

for (Int_t chindex = 0; chindex < 7; ++chindex){
TCanvas* Dplots[7];
TMultiGraph* Dgraphs[7];
  for(Int_t Ampindex = 0; Ampindex <7; ++Ampindex){

Dplots[Ampindex] = new TCanvas(Form("Channel_%d_Amp_%d mV",chindex,Ampindex),Form("Channel_%d_Amp_%d",chindex,Ampindex),800,600);
Dgraphs[Ampindex]= new TMultiGraph();
Dplots[Ampindex]->SetLogx();
Dplots[Ampindex]->cd();
gPad->SetLogx();
Dgraphs[Ampindex]->SetMaximum(2.2);
Dgraphs[Ampindex]->SetMinimum(0);
unplots[chindex][Ampindex]->SetLineColor(icol[1]);
unplots[chindex][Ampindex]->SetMarkerColor(icol[1]);
filtplots[chindex][Ampindex]->SetLineColor(icol[2]);
filtplots[chindex][Ampindex]->SetMarkerColor(icol[2]);
Dgraphs[Ampindex]->Add(unplots[chindex][Ampindex]);
Dgraphs[Ampindex]->Add(filtplots[chindex][Ampindex]);
//Dgraph[Ampindex]->Draw("APL");
Dgraphs[Ampindex]->SetTitle(Form("Timing: Channel_%d Amplitude_%d mV",chindex,NomAmp[Ampindex]));
Dgraphs[Ampindex]->GetXaxis()->SetTitle("Threshold (mV*ns)");
Dgraphs[Ampindex]->GetYaxis()->SetTitle("Timing Resolution (ns)");
legendA->AddEntry(unplots[chindex][Ampindex], Form("Unfiltered, Channel %d",chindex),"lp");
legendA->AddEntry(filtplots[chindex][Ampindex], Form("Filtered, Channel %d",chindex),"lp");
Dgraphs[Ampindex]->Draw("APL");
legendA->Draw();
Dplots[Ampindex]->Print(Form("Dec19/Timing-Ch%dAmp%d.png",chindex,NomAmp[Ampindex]));
legendA->Clear("D");
}//Close Ampindex D
}//Close Chindex D




}

int main(){
initial();
plotarray();
plotting();
return 0;
}
