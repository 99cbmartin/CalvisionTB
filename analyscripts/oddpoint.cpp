#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream> 
#include <TGraph.h>
#include <TF1.h>
#include <TLine.h>

const Int_t nThr = 20;
Double_t intThr[nThr];
  
TCanvas* heat;//[20];//all 20 heatmaps
TCanvas* projects;//[7];//7 random projections
TGraph* timplots;


void plotarray() {
    TFile* file = TFile::Open("PWOparFilterNarrow_Dec18.root");
    if (!file) {
        return;
    }
    


for(Int_t i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
}
 Double_t NomAmp[7]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)


for (Int_t pulseindex =0; pulseindex < 19; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code

Int_t Channel = 0;
Double_t Amp= 16.0;



TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_%d_%d",Channel,pulseindex)));
heatmap->GetXaxis()->SetRangeUser(.02,.4);
Double_t wind =2.0;
heat= new TCanvas("heatmap","heat Canvas",8000,8000);
heat->Divide(2,1,.02,.02);
heat->cd(1);
TLine* line1 = new TLine(.071, -5,.071, 20);
line1->SetLineColor(kRed);
line1->SetLineWidth(3);
TLine* line2 = new TLine(.055, -5,.055, 20);
line2->SetLineColor(kRed);
line2->SetLineWidth(3);
heat->cd(1);
heatmap->Draw("COLZ");
line1->Draw();
line2->Draw();
//heat->Print(Form("frontend/heatmap_%d_%d_16mV_FiltPWO.png",Channel,pulseindex));


			TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(Amp+wind)),heatmap->GetXaxis()->FindBin(1/(Amp-wind))); 
			//projects =new TCanvas("proj","Canvas",2000,1800);
			heat->cd(2);
			proj->Draw("HIST");
			Int_t maxbin=proj->GetMaximumBin();
			Double_t peak=proj->GetBinCenter(maxbin);
			TF1* fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
			proj->Fit(fitfunc,"RQ");
                        Double_t timeres = fitfunc->GetParameter(2);
			fitfunc->Draw("SAME");
			//std::cout<<timeres<<"is the thing"<<std::endl;
			heat->Print(Form("Dec18/Heatmap_%d_%d_16mV_PWOparfilterNarrow.png",Channel,pulseindex));


}//Close Pulse Loop
file->Close();
}//Close the plotarray function

int main(){
plotarray();
return 0;
}

