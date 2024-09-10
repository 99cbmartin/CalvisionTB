#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream> 
#include <TGraph.h>
#include <TF1.h>


const Int_t nThr = 20;
Double_t intThr[nThr];
  
TCanvas* heat;//[20];//all 20 heatmaps
TCanvas* projects;//[7];//7 random projections
TGraph* timplots;


void plotarray() {
    TFile* file = TFile::Open("finalroots/PWOparfilter.root");
    if (!file) {
        return;
    }
    


for(Int_t i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
}
 Double_t NomAmp[7]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)


for (Int_t pulseindex =0; pulseindex < 10; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code

Int_t Channel = 0;
Double_t Amp= 16.0;



TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("grIF_%d_%d",Channel,pulseindex)));

Double_t wind =2.0;
heat= new TCanvas("heatmap","heat Canvas",2000,1800);
heat->cd();
heatmap->Draw();
heat->Print(Form("problem2/grIF_%d_%d_filt.png",Channel,pulseindex));


//			TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(Amp+wind)),heatmap->GetXaxis()->FindBin(1/(Amp-wind))); 
//			projects =new TCanvas("proj","Canvas",2000,1800);
//			projects->cd();
//			proj->Draw("HIST");
///			Int_t maxbin=proj->GetMaximumBin();
//			Double_t peak=proj->GetBinCenter(maxbin);
//			TF1* fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
//			proj->Fit(fitfunc,"RQ");
  //                      Double_t timeres = fitfunc->GetParameter(2);
//			fitfunc->Draw("SAME");
//			std::cout<<timeres<<"is the thing"<<std::endl;
//			projects->Print(Form("problem/Projection_%d_%d_16mV_filt.png",Channel,pulseindex));


}//Close Pulse Loop
file->Close();
}//Close the plotarray function

int main(){
plotarray();
return 0;
}

