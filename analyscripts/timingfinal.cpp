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
#include <TProfile.h>

int in = 0;
TMultiGraph* mgFVU[7][7][3];//!!NEEDS TO BE OUTSIDE LOOP,[4]: 0,1 uncorrected, 2,3 Corrected
TMultiGraph* mgCVU[7][7][4];//Corrected Vs Uncorrected
TMultiGraph* mgHVM[7][7][4];//Hit Vs Miss SiPM
TMultiGraph* mgNVW[7][7][4];//Narrow Vs Wide Radial cut
TCanvas* CVU[7][7][4];//0 = PWO, 2 = BGO
TCanvas* HVM[7][7][4];
TCanvas* NVW[7][7][4];
TCanvas* PWOFVU[7][7]; //PWO Filtered Vs Unfiltered[10]
TCanvas* BGOFVU[7][7]; 
TCanvas* PWOFVUC[7][7];
TCanvas* BGOFVUC[7][7];//BGO Filt Vs Unfilt Corrected 
const char* sourcefiles[2] = {"PWOpAWCNew_Bins.root","PWOpFAWCNew_Bins.root"};//pull both files
const TString pretitle[2] = {"PWOp","PWOpF"};//"BGOpF"};//,"BGOp"}
TProfile* walkcor[7][20][4];





Int_t icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };
TGraphErrors* timplots[7][10][2];// 7 channels, 7 Nominal Amplitudes, 2 methods (one with amplitude walk corrections) 
TLegend* legendC = new TLegend(0,1,1,.1);
TLegend* legendA = new TLegend(0,1,1,.1);

const int nThr = 20;
double intThr[nThr];

void initial(){
for (Int_t i=0;i<7;++i){
for (Int_t j=0;j<10;++j){
for (Int_t k=0;k<2;++k){
timplots[i][j][k] = new TGraphErrors();
}
}
} 
}

void walkcorrections(){
// ==========================
//    Amp. Walk Corrections
// ==========================
TFile* fitXFile = TFile::Open(Form("%s_fitXFile.root",pretitle[in].Data()), "RECREATE");
TFile* file = TFile::Open(Form("../outputroots/%s",sourcefiles[in]));
    if (!file) {
        return;
    }


for (Int_t pulseindex =0; pulseindex < 20; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code
    for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6
        file->cd();
	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_%d_%d",chindex,pulseindex)));
	walkcor[chindex][pulseindex][in] = heatmap->ProfileX("profilex");
	TCanvas* meancanvas = new TCanvas();
	walkcor[chindex][pulseindex][in]->Draw();
	meancanvas->Update();
//temp remove	meancanvas->Print(Form("plots/timing/FitCheck/%s_h201Walk_CH%dThr%d.png",pretitle[in].Data(),chindex,pulseindex));
	fitXFile->cd(); 	
	walkcor[chindex][pulseindex][in]->Write(Form("%s_walkcor_C%d_T%d",pretitle[in].Data(),chindex,pulseindex));	
   }
}
fitXFile->Close();






file->Close();
            }


void plotarray() {
    TFile* file = TFile::Open(Form("../outputroots/%s",sourcefiles[in]));
    if (!file) {
        return;
    }
    

for(int i=0; i<nThr; i++){
    intThr[i] = pow(10, 0.4 + 0.1 * i);
}//This is the loop for creating Threshold values. MUST change in Sasha's anlysis to see a difference.

Double_t NomAmp[7]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)

TString plotNamesA[] = {"Amp 16 mV", "Amp 19 mV", "Amp 24 mV", "Amp 33 mV", "Amp 45 mV", "Amp 63 mV", "Amp 87 mV","Legend"};

// =====================
//      Timing
// =====================
// TFile* timingplots = TFile::Open(Form("%s_timing.root",pretitle[in].Data()), "RECREATE");
for (Int_t pulseindex =0; pulseindex < 20; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code
    for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6
	file->cd();
	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_%d_%d",chindex,pulseindex)));
	TH2D* heatmapAW = dynamic_cast<TH2D*>(file->Get(Form("h201AW_%d_%d",chindex,pulseindex)));
        TString legendEntryC = TString::Format("Ch %d",chindex);
	TString legendEntryA = TString::Format(plotNamesA[chindex]);
	Double_t wind =2.0;
	for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){
	TCanvas* BinCheck = new TCanvas();
	TCanvas* BinCheckAW = new TCanvas();
	TF1* fitfunc;//initialize here such that we get a new one every channel,pulse,and amplitude as there are that many unique fits to produce
	TF1* fitfunc2;//for fitting after corrections
	TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 
	TH1D* proj2 = (TH1D*)heatmapAW->ProjectionY("proj2",heatmapAW->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmapAW->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 
	   if(proj->GetEntries() == 0){//sometimes, there is no data for a particular range of Amplitudes. Skip these plots.
	        continue;
	   }else{
	        Int_t maxbin = proj->GetMaximumBin();
		Double_t peak = proj->GetBinCenter(maxbin);
		fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
                proj->Fit(fitfunc,"RQ");
		BinCheck->cd();
		proj->Draw();
		//BinCheck->Print(Form("plots/BinCheck1000/%s/C%dT%dA%d.png",pretitle[in].Data(),chindex,pulseindex,Ampindex));
		Double_t timeres = fitfunc->GetParameter(2);
		Double_t timeer = fitfunc->GetParError(2);
		std::cout<<"error is:"<<" "<<timeer<<std::endl;
		timplots[chindex][Ampindex][0]->SetPoint(pulseindex,intThr[pulseindex],timeres);//timing plots stored in an array  
		timplots[chindex][Ampindex][0]->SetPointError(pulseindex,0.0,timeer);
		timplots[chindex][Ampindex][0]->SetMarkerSize(0.5);
		timplots[chindex][Ampindex][0]->SetMarkerStyle(20);
		timplots[chindex][Ampindex][0]->GetXaxis()->SetRangeUser(0, 300);
		timplots[chindex][Ampindex][0]->SetMaximum(2.2);
		timplots[chindex][Ampindex][0]->SetMinimum(0);
		//std::cout<<"For Channel:"<<chindex<<" and Amplitude:"<<NomAmp[Ampindex]<<" We will see a timing res:"<<timeres<<" for Thresh of"<<pulseindex<<std::endl;	
             //   timingplots->cd(); THIS NEEDS TO BE IN A DIFFERENT FUNCTION. You're writing these to a file before they're finished filling with data. It's not working obv
            //    timplots[chindex][Ampindex][0]->Write(Form("%s_Ch%d_Amp%d",pretitle[in].Data(),chindex,Ampindex));

            }
// =====================================
//    Amplitude Walk Corrected Timing
//  ===================================
            
	    if(proj2->GetEntries() == 0){
                continue;
            }else{
		file->cd();
		Int_t maxbin2 = proj2->GetMaximumBin();
                Double_t peak2 = proj2->GetBinCenter(maxbin2);
                fitfunc2 = new TF1("fitfunc2","gaus",peak2-5,peak2+5);
                proj2->Fit(fitfunc2, "RQ");
		BinCheckAW->cd();
		proj2->Draw();
          	//BinCheckAW->Print(Form("plots/BinCheck1000/%sAW/C%dT%dA%d.png",pretitle[in].Data(),chindex,pulseindex,Ampindex));
		Double_t timeres2 = fitfunc2->GetParameter(2);
		Double_t timeer2 = fitfunc2->GetParError(2);
		timplots[chindex][Ampindex][1]->SetPoint(pulseindex,intThr[pulseindex],timeres2);
                timplots[chindex][Ampindex][1]->SetPointError(pulseindex,0.0,timeer2);
		timplots[chindex][Ampindex][1]->SetMarkerSize(0.5);
                timplots[chindex][Ampindex][1]->SetMarkerStyle(20);
                timplots[chindex][Ampindex][1]->GetXaxis()->SetRangeUser(0, 300);
                timplots[chindex][Ampindex][1]->SetMaximum(2.2);
                timplots[chindex][Ampindex][1]->SetMinimum(0);
             //   std::cout<<"For Channel:"<<chindex<<" and Amplitude:"<<NomAmp[Ampindex]<<" We will see a corrected timing res:"<<timeres2<<" for Thresh of"<<pulseindex<<std::endl;
	//	timingplots->cd();
	//	timplots[chindex][Ampindex][1]->Write(Form("%s_Corr_Ch%d_Amp%d",pretitle[in].Data(),chindex,Ampindex));
	 }

        }//close Amp loop
    }//Close Channel Loop
}//Close Pulse Loop

//timingplots->Close();
file->Close();
}//Close the plotarray function

void Bin(){
    TFile* file = TFile::Open(Form("../outputroots/%s",sourcefiles[in]));
    if (!file) {
        return;
    }
Int_t NomAmp[7]={16,19,24,33,45,63,87};
TGraphErrors* TimVsBin[7][7][20];
TCanvas* TimVsBinC[7][7][20]; 
TCanvas* projC[7][7][20];
int Bins[6]={1000,800,600,400,350,250};
for(int i = 0 ; i < 7 ; i++){//Channel
for (int j = 0 ; j < 7 ; j++){//Amp
for(int k = 0 ; k < 20 ; k++){//Thr
TimVsBin[i][j][k] = new TGraphErrors();//Fill this in later
TimVsBinC[i][j][k] = new TCanvas();
projC[i][j][k] = new TCanvas();
}
}
}

for (int B =0 ; B < 6 ; B++){
    for (Int_t pulseindex =0; pulseindex < 20; ++pulseindex){
        for (Int_t chindex = 0; chindex < 7; ++chindex) {
            file->cd();
            TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201AW%d_%d_%d",Bins[B],chindex,pulseindex)));
	    Double_t wind =2.0;
                for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){
                    TF1* fitfunc;
		    TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind)));
                        if(proj->GetEntries() == 0){
                            continue;
                        }else{
                            Int_t maxbin = proj->GetMaximumBin();
                            Double_t peak = proj->GetBinCenter(maxbin);
                            fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
                            proj->Fit(fitfunc,"RQ");
			    projC[chindex][Ampindex][pulseindex]->cd();
			    proj->Draw();
			   // projC[chindex][Ampindex][pulseindex]->Print(Form("plots/BinCheck/%sAW/C%dA%dT%d/B%d.png",pretitle[in].Data(),chindex,Ampindex,pulseindex,B));//where should it go?
		            Double_t timeres = fitfunc->GetParameter(2);
			    TimVsBin[chindex][Ampindex][pulseindex]->SetPoint(B,Bins[B],timeres);
}
}
}
}
}
for(int i = 0 ; i < 7 ; i++){//Channel
for (int j = 0 ; j < 7 ; j++){//Amp
for(int k = 0 ; k < 20 ; k++){//Thr
TimVsBinC[i][j][k]->cd();
TimVsBin[i][j][k]->Draw();
//TimVsBinC[i][j][k]->Print(Form("plots/TimVsBin/%sAW/C%dA%dT%d.png",pretitle[in].Data(),i,j,k));
}
}
}
file->Close();

}
void plotting(){

// =====================
//     Summary Plotting
// ======================

const TString mtitle[2] = {"","AWC"};
for (int m = 0 ; m < 2 ; m++){//m=0 is uncorrected and m=1 is corrected for amplitude walk   
TFile* timingplots = TFile::Open(Form("%s_timing.root",pretitle[in].Data()), "RECREATE");

    Int_t NomAmp[7]={16,19,24,33,45,63,87};
    TCanvas* canvasC = new TCanvas(Form("%s_Individual Channels%s",pretitle[in].Data(),mtitle[m].Data()), "Individual Channels", 4000,3600);
    TCanvas* canvasA = new TCanvas(Form("%s_Individual Nominal Amplitudes%s",pretitle[in].Data(),mtitle[m].Data()),"Individual Nominal Amplitudes",4000,3600);
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
                timplots[chindex][Ampindex][m]->SetLineColor(icol[Ampindex]);
                timplots[chindex][Ampindex][m]->SetMarkerColor(icol[Ampindex]);
                mgC[chindex]->Add(timplots[chindex][Ampindex][m]);

		timingplots->cd();
		 timplots[chindex][Ampindex][0]->Write(Form("%s_Ch%d_Amp%d",pretitle[in].Data(),chindex,Ampindex));		
		timplots[chindex][Ampindex][1]->Write(Form("%s_Corr_Ch%d_Amp%d",pretitle[in].Data(),chindex,Ampindex));	
            }
            mgC[chindex]->Draw("APL");
            mgC[chindex]->SetTitle(Form("%s_Channel %d %s",pretitle[in].Data(),chindex,mtitle[m].Data()));
            mgC[chindex]->GetXaxis()->SetTitle("Threshold mV*ns");
            mgC[chindex]->GetYaxis()->SetTitle("Timing Resolution ns");
        }
timingplots->Close();
for (Int_t i = 0; i < 7; ++i){
    for (Int_t j = 0; j < 7; ++j){
        if(i == 6){
            legendC->AddEntry(timplots[i][j][m],Form("Amplitude %d mV",NomAmp[j]),"lp");
        }
    }
} 

canvasC->cd(9);
legendC->Draw();
canvasC->Print(Form("plots/timing/Summary/New/%s_ChannelbyAmp%s.png",pretitle[in].Data(),mtitle[m].Data()));//plots of each channel (title) and organized by Amplitude



for (Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){//Filling the Amplitude plots with each Channel
        canvasA->cd(Ampindex+1);
	gPad->SetLogx();
        mgA[Ampindex] = new TMultiGraph();
        mgA[Ampindex]->SetMaximum(2.2);
	mgA[Ampindex]->SetMinimum(0);
	for(Int_t chindex = 0; chindex <7; ++chindex){
                timplots[chindex][Ampindex][m]->SetLineColor(icol[chindex]);
		timplots[chindex][Ampindex][m]->SetMarkerStyle(kFullSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
                timplots[chindex][Ampindex][m]->SetMarkerColor(icol[chindex]);
                mgA[Ampindex]->Add(timplots[chindex][Ampindex][m]);
 //was ch brack

// =====================
// 	FVU Plotting
// =====================
//if (in == 0){//do PWO FVU
//mgFVU[chindex][Ampindex][2] = new TMultiGraph();
//mgFVU[chindex][Ampindex][2]->SetMaximum(2.2);
//mgFVU[chindex][Ampindex][2]->SetMinimum(0);
//PWOFVUC[chindex][Ampindex] = new TCanvas();
//PWOFVUC[chindex][Ampindex]->cd();
//gPad->SetLogx();
//timplots[chindex][Ampindex][1]->SetLineColor(icol[chindex]);
//timplots[chindex][Ampindex][1]->SetMarkerStyle(kOpenSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
//timplots[chindex][Ampindex][1]->SetMarkerColor(icol[chindex]);
//mgFVU[chindex][Ampindex][2]->Add(timplots[chindex][Ampindex][1]);
//}else if (in == 1){
//PWOFVUC[chindex][Ampindex]->cd();
//timplots[chindex][Ampindex][1]->SetLineColor(icol[chindex]);
//timplots[chindex][Ampindex][1]->SetMarkerStyle(kFullSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
//timplots[chindex][Ampindex][1]->SetMarkerColor(icol[chindex]);
//mgFVU[chindex][Ampindex][2]->Add(timplots[chindex][Ampindex][1]);
//mgFVU[chindex][Ampindex][2]->Draw("APL");
//mgFVU[chindex][Ampindex][2]->SetTitle(Form("%s_FVU_Ch%d_Amp%d mV",pretitle[1].Data(),chindex,NomAmp[Ampindex]));
//mgFVU[chindex][Ampindex][2]->GetXaxis()->SetTitle("Threshold mV*ns");
//mgFVU[chindex][Ampindex][2]->GetYaxis()->SetTitle("Timing Resolution ns");
////temp remove PWOFVUC[chindex][Ampindex]->Print(Form("plots/timing/FVU/%s_FVU_Ch%d_Amp%dmV.png",pretitle[1].Data(),chindex,NomAmp[Ampindex]));
//}
//if (in == 2){//do PWO FVU
//mgFVU[chindex][Ampindex][3] = new TMultiGraph();
//mgFVU[chindex][Ampindex][3]->SetMaximum(2.2);
//mgFVU[chindex][Ampindex][3]->SetMinimum(0);
//BGOFVUC[chindex][Ampindex] = new TCanvas();
//BGOFVUC[chindex][Ampindex]->cd();
//gPad->SetLogx();
//timplots[chindex][Ampindex][1]->SetLineColor(icol[chindex]);
//timplots[chindex][Ampindex][1]->SetMarkerStyle(kOpenSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
//timplots[chindex][Ampindex][1]->SetMarkerColor(icol[chindex]);
//mgFVU[chindex][Ampindex][3]->Add(timplots[chindex][Ampindex][1]);
//}//else if (in == 3){
////BGOFVUC[chindex][Ampindex]->cd();
////timplots[chindex][Ampindex][1]->SetLineColor(icol[chindex]);
////timplots[chindex][Ampindex][1]->SetMarkerStyle(kFullSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
////timplots[chindex][Ampindex][1]->SetMarkerColor(icol[chindex]);
////mgFVU[chindex][Ampindex][3]->Add(timplots[chindex][Ampindex][1]);
////mgFVU[chindex][Ampindex][3]->Draw("APL");
////mgFVU[chindex][Ampindex][3]->SetTitle(Form("%s_FVU_Ch%d_Amp%d mV",pretitle[2].Data(),chindex,NomAmp[Ampindex]));
////mgFVU[chindex][Ampindex][3]->GetXaxis()->SetTitle("Threshold mV*ns");
////mgFVU[chindex][Ampindex][3]->GetYaxis()->SetTitle("Timing Resolution ns");
////There is no Unfiltered BGO>>>>>BGOFVUC[chindex][Ampindex]->Print(Form("plots/timing/FVU/%s_FVU_Ch%d_Amp%dmV.png",pretitle[2],chindex,NomAmp[Ampindex]));
////}
////
////==================================
////	Corrected Vs. Uncorrected
////=================================
//CVU[chindex][Ampindex][in] = new TCanvas();
//CVU[chindex][Ampindex][in]->cd();
//gPad->SetLogx();
//mgCVU[chindex][Ampindex][in] = new TMultiGraph();
//mgCVU[chindex][Ampindex][in]->SetMaximum(2.2);
//mgCVU[chindex][Ampindex][in]->SetMinimum(0);
//mgCVU[chindex][Ampindex][in]->Add(timplots[chindex][Ampindex][0]);
//mgCVU[chindex][Ampindex][in]->Add(timplots[chindex][Ampindex][1]);
//mgCVU[chindex][Ampindex][in]->Draw("APL");
//mgCVU[chindex][Ampindex][in]->SetTitle(Form("%s_CVU_Ch%d_Amp%d mV",pretitle[in].Data(),chindex,NomAmp[Ampindex]));
//mgCVU[chindex][Ampindex][in]->GetXaxis()->SetTitle("Threshold mV*ns");
//mgCVU[chindex][Ampindex][in]->GetYaxis()->SetTitle("Timing Resolution ns");
//// temp remove CVU[chindex][Ampindex][in]->Print(Form("plots/timing/CVU/%s_CVU_Ch%d_Amp%dmV.png",pretitle[in].Data(),chindex,NomAmp[Ampindex]));
//
//
//
////=========================
////	Hit Vs. Miss
////=========================
//
//
//
//
//
//
//
//
//
////===============================
////	Narrow Vs. Wide
////===============================
//
//
//
//
//
//
//
//
}//ch close
mgA[Ampindex]->Draw("APL");
mgA[Ampindex]->SetTitle(Form("%s Amplitude %d mV %s",pretitle[in].Data(),NomAmp[Ampindex],mtitle[m].Data()));
mgA[Ampindex]->GetXaxis()->SetTitle("Threshold mV*ns");
mgA[Ampindex]->GetYaxis()->SetTitle("Timing Resolution ns");





}//amp close

for (Int_t i = 0; i < 7; ++i){
        for (Int_t j = 0; j < 7; ++j){
                if(j == 6){
                legendA->AddEntry(timplots[i][j][m],Form("Channel %d",i),"lp");
                }
        }
}

canvasA->cd(9);
legendA->SetLineWidth(5);
legendA->Draw();
canvasA->Print(Form("plots/timing/Summary/New/%s_AmplitudebyChan%s.png",pretitle[in].Data(),mtitle[m].Data()));
}//end method loop

}

int main(){
for (Int_t i=0;i<2;i++){//file loop
initial();
walkcorrections();//dont forget to add the new functions
plotarray();
Bin();
plotting();
in++;
}
return 0;
}
