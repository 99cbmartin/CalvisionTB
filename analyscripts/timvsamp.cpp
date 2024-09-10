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
const char* sourcefiles[3] = {"PWOpAW.root","PWOpFAW.root","BGOpFAW.root"};//"BGOp.root","BGOpF.root"}//pull all 4 files
const TString pretitle[3] = {"PWOp","PWOpF","BGOpF"};//,"BGOp"}
TProfile* walkcor[7][20][4];





Int_t icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };
TGraph* timplots[7][10][2];// 7 channels, 7 Nominal Amplitudes, 2 methods (one with amplitude walk corrections) 

TGraph* timvsAmp[7][20];//one for each of 7 channels, and each of 20 thresh 

TLegend* legendC = new TLegend(0,1,1,.1);
TLegend* legendA = new TLegend(0,1,1,.1);

const int nThr = 20;
double intThr[nThr];

void initial(){
for (Int_t i=0;i<7;++i){
for (Int_t j=0;j<10;++j){
for (Int_t k=0;k<2;++k){
timplots[i][j][k] = new TGraph();
}
}
} 

for (Int_t i=0;i<7;++i){
for (Int_t j=0;j<20;++j){
timvsAmp[i][j] = new TGraph(); 
}
}
 
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
 TFile* timingplots = TFile::Open(Form("%s_timing.root",pretitle[in].Data()), "RECREATE");
for (Int_t pulseindex =0; pulseindex < 20; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code
    for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6
	file->cd();
	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_%d_%d",chindex,pulseindex)));
	TH2D* heatmapAW = dynamic_cast<TH2D*>(file->Get(Form("h201AW_%d_%d",chindex,pulseindex)));
        TString legendEntryC = TString::Format("Ch %d",chindex);
	TString legendEntryA = TString::Format(plotNamesA[chindex]);
	Double_t wind =2.0;
	for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){
	TF1* fitfunc;//initialize here such that we get a new one every channel,pulse,and amplitude as there are that many unique fits to produce
	TF1* fitfunc2;//for fitting after corrections
	TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 
	TH1D* proj2 = (TH1D*)heatmapAW->ProjectionY("proj2",heatmapAW->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmapAW->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 

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
                Double_t timeres2 = fitfunc2->GetParameter(2);
                 std::cout<<"For Channel "<<chindex<<" and Amplitude: "<<NomAmp[Ampindex]<<" a timeres of: "<<timeres2<<std::endl;
	//timvsAmp[chindex][pulseindex]->SetPoint(Ampindex,NomAmp[Ampindex],timeres2);
	timvsAmp[chindex][pulseindex]->SetMaximum(2.2);
        timvsAmp[chindex][pulseindex]->SetMinimum(0);
	timvsAmp[chindex][pulseindex]->SetMarkerStyle(20);
	timvsAmp[chindex][pulseindex]->SetTitle(Form("Timing Vs Amp: Ch_%d_Thr_%d mV",chindex,intThr[pulseindex]));
	timvsAmp[chindex][pulseindex]->SetLineColor(icol[chindex]);
	timvsAmp[chindex][pulseindex]->SetMarkerColor(icol[chindex]);
	timvsAmp[chindex][pulseindex]->SetMarkerStyle(kFullSquare);//kFullSquare for Filtered and kOpenSquare for Unfiltered Data
	timvsAmp[chindex][pulseindex]->SetMarkerColor(icol[chindex]);
	 }

        }//close Amp loop
    }//Close Channel Loop
}//Close Pulse Loop
timingplots->Close();
file->Close();
//file2->Close();
}//Close the plotarray function

void plotting(){

// =====================
//     Summary Plotting
// ======================

const TString mtitle[2] = {"","AWC"};
for (int m = 0 ; m < 2 ; m++){//m=0 is uncorrected and m=1 is corrected for amplitude walk   


    Int_t NomAmp[7]={16,19,24,33,45,63,87};
//    TCanvas* cantimvsAmp = new TCanvas(Form("%s_TimingVsAmplitude_Ch%d_Thr_%d mV",pretitle[in].Data(),chindex,intThr[pulseindex]),800,600);
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

            }
            mgC[chindex]->Draw("APL");
            mgC[chindex]->SetTitle(Form("%s_Channel %d Filtered%s",pretitle[in].Data(),chindex,mtitle[m].Data()));
            mgC[chindex]->GetXaxis()->SetTitle("Threshold mV*ns");
            mgC[chindex]->GetYaxis()->SetTitle("Timing Resolution ns");
        }

for (Int_t i = 0; i < 7; ++i){
    for (Int_t j = 0; j < 7; ++j){
        if(i == 6){
            legendC->AddEntry(timplots[i][j][m],Form("Amplitude %d mV",NomAmp[j]),"lp");
        }
    }
} 

canvasC->cd(9);
legendC->Draw();
//temp remove canvasC->Print(Form("plots/timing/Summary/%s_ChannelbyAmp%s.png",pretitle[in].Data(),mtitle[m].Data()));//plots of each channel (title) and organized by Amplitude



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






}//ch close
mgA[Ampindex]->Draw("APL");
mgA[Ampindex]->SetTitle(Form("%s Amplitude %d mV Filtered%s",pretitle[in].Data(),NomAmp[Ampindex],mtitle[m].Data()));
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
//tempremove canvasA->Print(Form("plots/timing/Summary/%s_AmplitudebyChan%s.png",pretitle[in].Data(),mtitle[m].Data()));
}//end method loop
//loops channel and pulse and makes some plots
//TMultiGraph* mgtva[20];//one for each Thresh
//TMultiGraph* mgtva2[7];
//for (int chindex = 0 ; chindex < 7 ; chindex++){
//for (int pulseindex = 0 ; pulseindex < 20 ; pulseindex++){
//
//TCanvas* cantimvsAmp = new TCanvas(Form("%s_TimingVsAmplitude_Ch%d_Thr_%d_mV",pretitle[in].Data(),chindex,intThr[pulseindex]),Form("%s_TimingVsAmplitude_Ch%d_Thr_%d_mV",pretitle[in].Data(),chindex,intThr[pulseindex]),800,600);
//cantimvsAmp->cd();
//timvsAmp[chindex][pulseindex]->Draw();
//cantimvsAmp->Print(Form("plots/timvsAmp/%s_TimingVsAmp_Ch%d_Thr%d.png",pretitle[in].Data(),chindex,pulseindex));
//mgtva[pulseindex]->Add(timvsAmp[chindex][pulseindex]);
////timvsAmp[chindex][pulseindex]->SetMarkerColor
////timvsAmp[chindex][pulseindex]->SetLineColor
//mgtva2[chindex]->Add(timvsAmp[chindex][pulseindex]);
//}
//}
//for(int i= 0 ; i<20 ; i++){
//TCanvas* Changr = new TCanvas(Form("%s_ChannelsTimingVsAmplitude__Thr_%d_mV",pretitle[in].Data(),intThr[i]),Form("%s_ChannelsTimingVsAmplitude__Thr_%d_mV",pretitle[in].Data(),intThr[i]),800,600);
//Changr->cd();
//mgtva[i]->Draw();
//legendA->Draw();
//Changr->Print(Form("plots/timvsAmp/%s_ChannelsTimVsAmp_Thr%d.png",pretitle[in].Data(),i));
//}


}
//}//end file loop
int main(){
for (Int_t i=0;i<3;i++){//file loop
initial();
//walkcorrections();//dont forget to add the new functions
plotarray();
plotting();
in++;
}
return 0;
}
