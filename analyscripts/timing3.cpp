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
//TFile* fitsFile = TFile::Open("fitsFile.root", "RECREATE");

Int_t icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };
TGraphErrors* timplots[7][10][2];// 7 channels, 7 Nominal Amplitudes, 2 methods (one with amplitude walk corrections) 
TGraph* meanvsamp[7][20]; 
TF1* fitfunc2[7][20];
TLegend* legendC = new TLegend(0,1,1,.1);
TLegend* legendA = new TLegend(0,1,1,.1);

const int nThr = 20;
double intThr[nThr];
//for(Int_t t = 0 ; t < nThr; ++t){
  // intThr[t] = pow(10, 0.4 + 0.1 * t);
// }//This is the loop for creating Threshold values. MUST change in Sasha's anlysis to see a difference. 

void initial(){
for (Int_t i=0;i<7;++i){
for (Int_t j=0;j<10;++j){
for (Int_t k=0;k<2;++k){
timplots[i][j][k] = new TGraphErrors();
}
}
} 

//for (Int_t i=0;i<7;++i){
//for (Int_t j=0;j<20;++j){
//TH1D* meanvsamp[i][j]; 
//}
//}
 
}




void plotarray() {
    TFile* file = TFile::Open("../outputroots/PWOpAWCNew_350.root");
    if (!file) {
        return;
    }
    
    TFile* file2 = TFile::Open("../outputroots/PWOparFilterAmpwalk.root");
    if (!file2){
	return;
    }


for(int i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
  }//This is the loop for creating Threshold values. MUST change in Sasha's anlysis to see a difference.

 Double_t NomAmp[10]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)

 TString plotNamesA[] = {"Amp 16 mV", "Amp 19 mV", "Amp 24 mV", "Amp 33 mV", "Amp 45 mV", "Amp 63 mV", "Amp 87 mV","Legend"};


for (Int_t pulseindex =0; pulseindex < 20; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code



   for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6
	//TF1* fitfunc2;
//	TF1* meanfit;//For fitting to the meanvsamp plot correlating to each h201
	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201R_Ring%d_Thr%d",chindex,pulseindex)));
        TString legendEntryC = TString::Format("Ch %d",chindex);
	TString legendEntryA = TString::Format(plotNamesA[chindex]);
      // legend->AddEntry(Ringplots[5], legendEntry, "l");
//	 meanvsamp[chindex][pulseindex]= new TGraph();
//meanvsamp[chindex][pulseindex]->SetTitle(Form("meanvsamp_CH%d_Thr%d",chindex,pulseindex));//,Form("Channel %d and Thresh %d",chindex,pulseindex));//This will contain the information of meanvsamp for each channel and threshold, in the Amp loop i will fill with 7 discreet points
//	TCanvas* meanvsampcanvas = new TCanvas();//new Tcanvas for each h201 to plot the meanvsamp
	Double_t wind =2.0;


		for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){

		 	
			TF1* fitfunc;//initialize here such that we get a new one every channel,pulse,and amplitude as there are that many unique fits to produce
			//TF1* fitfunc2;//for fitting after corrections
			TCanvas* projcan = new TCanvas();
			TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind)));
			if(proj->GetEntries() == 0){//sometimes, there is no data for a particular range of Amplitudes. Skip these plots.
			continue;
			}else{
			Int_t maxbin = proj->GetMaximumBin();
			Double_t peak = proj->GetBinCenter(maxbin);
		//	if (pulseindex < 11){
		//	fitfunc = new TF1("fitfunc","gaus",peak-.6,peak+.6);
		//	}else{
			fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
		//	}
                        proj->Fit(fitfunc,"RQ");
                        if (chindex ==0){
			proj->Draw();
			//projcan->Print(Form("timecheck/proj_R%d_T%d_A%d.png",chindex,pulseindex,Ampindex));
}
			Double_t timeres = fitfunc->GetParameter(2);
			Double_t timeer = fitfunc->GetParError(2);
			timplots[chindex][Ampindex][0]->SetPoint(pulseindex,intThr[pulseindex],timeres);//timing plots stored in an array 
			timplots[chindex][Ampindex][0]->SetPointError(pulseindex,0.0,timeer); 
	        timplots[chindex][Ampindex][0]->SetMarkerSize(0.5);
		timplots[chindex][Ampindex][0]->SetMarkerStyle(20);
		timplots[chindex][Ampindex][0]->GetXaxis()->SetRangeUser(0, 300);
		timplots[chindex][Ampindex][0]->SetMaximum(2.2);
		timplots[chindex][Ampindex][0]->SetMinimum(0);
		//std::cout<<"For Channel:"<<chindex<<" and Amplitude:"<<NomAmp[Ampindex]<<" We will see a timing res:"<<timeres<<" for Thresh of"<<pulseindex<<std::endl;	

//here I will perform amplitude walk corrections:

//double meantime = proj->GetMean();
//std::cout<<"For Channel: "<<chindex<<" and Amplitude: "<<NomAmp[Ampindex]<<" We will see a timing res: "<<timeres<<" for Thresh of "<<pulseindex<<" and a mean of: "<< meantime<<std::endl;
//meanvsamp[chindex][pulseindex]->SetPoint(Ampindex,1/(NomAmp[Ampindex]),meantime);
}
}//close Amp loop
//meanvsamp[chindex][pulseindex]->SetMarkerStyle(23);
//meanvsamp[chindex][pulseindex]->SetMarkerSize(3.0);
//meanvsampcanvas->cd();
//meanvsamp[chindex][pulseindex]->Draw("AP");
//meanvsampcanvas->Print(Form("plots/timing/meanvsamp_CH%d_Thresh%d.png",chindex,pulseindex));
//fitfunc2[chindex][pulseindex] = new TF1(Form("fitfunc2_CH%d_Thresh%d",chindex,pulseindex),"pol1",(1/89),(1/5));
//meanvsamp[chindex][pulseindex]->Fit(fitfunc2[chindex][pulseindex],"RQ");
//meanvsampcanvas->Update();
//meanvsampcanvas->Print(Form("plots/timing/meanvsamp_CH%d_Thresh%dFIT.png",chindex,pulseindex));
//fitsFile->cd();
//fitfunc2[chindex][pulseindex]->Write(Form("fitFunc2_C%d_T%d", chindex,pulseindex));
//fitsFile->Close();
}//Close Channel Loop
}//Close Pulse Loop
file->Close();
file2->Close();
//fitsFile->Close();
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
    TMultiGraph* mgA[10];
    TMultiGraph* mgC[10];


for (Int_t chindex = 0; chindex < 7; ++chindex){//Filling the Channel plots with each Amplitude
        canvasC->cd(chindex+1);
        gPad->SetLogx();
        mgC[chindex] = new TMultiGraph();
	mgC[chindex]->SetMaximum(2.2);
	mgC[chindex]->SetMinimum(0);
        for(Int_t Ampindex = 0; Ampindex <7; ++Ampindex){
                timplots[chindex][Ampindex][0]->SetLineColor(icol[Ampindex]);
		timplots[chindex][Ampindex][0]->SetLineWidth(3);
                timplots[chindex][Ampindex][0]->SetMarkerColor(icol[Ampindex]);
                mgC[chindex]->Add(timplots[chindex][Ampindex][0]);

 }
        mgC[chindex]->Draw("APL");
        mgC[chindex]->SetTitle(Form("Ring %d",chindex+1));//was CHannel
        mgC[chindex]->GetXaxis()->SetTitle("Threshold mV*ns");
        mgC[chindex]->GetYaxis()->SetTitle("Timing Resolution ns");
}

for (Int_t i = 0; i < 7; ++i){
        for (Int_t j = 0; j < 7; ++j){
                if(i == 6){
                legendC->AddEntry(timplots[i][j][0],Form("Amplitude %d mV",NomAmp[j]),"lp");
                }
        }
} 
canvasC->cd(9);
legendC->Draw();
canvasC->Print("plots/timing/Summary/New/Ringtitles350.png");
for (Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){//Filling the Amplitude plots with each Channel
        canvasA->cd(Ampindex+1);
	gPad->SetLogx();
        mgA[Ampindex] = new TMultiGraph();
        mgA[Ampindex]->SetMaximum(2.2);
	mgA[Ampindex]->SetMinimum(0);
	for(Int_t chindex = 0; chindex <7; ++chindex){
                timplots[chindex][Ampindex][0]->SetLineColor(icol[chindex]);
		timplots[chindex][Ampindex][0]->SetMarkerStyle(kFullSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
                timplots[chindex][Ampindex][0]->SetMarkerColor(icol[chindex]);
                mgA[Ampindex]->Add(timplots[chindex][Ampindex][0]);
  }
        mgA[Ampindex]->Draw("APL");
        mgA[Ampindex]->SetTitle(Form("Amplitude %d mV",NomAmp[Ampindex]));
        mgA[Ampindex]->GetXaxis()->SetTitle("Threshold mV*ns");
        mgA[Ampindex]->GetYaxis()->SetTitle("Timing Resolution ns");
}

for (Int_t i = 0; i < 7; ++i){
        for (Int_t j = 0; j < 7; ++j){
                if(j == 6){
                legendA->AddEntry(timplots[i][j][0],Form("Ring %d",i+1),"lp");
                }
        }
}

canvasA->cd(9);
legendA->SetLineWidth(5);
legendA->Draw();
canvasA->Print("plots/timing/Summary/New/RingsAmptitles350.png");

}

int main(){
initial();
plotarray();
plotting();
return 0;
}
