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
TGraphErrors* timvsAmp[7][20];
TLegend* legendC = new TLegend(0,1,1,.1);
TLegend* legendA = new TLegend(0,1,1,.1);

const int nThr = 20;
double intThr[nThr];

void initial(){
for (Int_t i=0;i<7;++i){
for (Int_t j=0;j<20;++j){
timvsAmp[i][j]=new TGraphErrors();
}
} 
 
}
TFile* plotfile = TFile::Open("PWOp_TVA.root", "RECREATE");



void plotarray() {
    TFile* file = TFile::Open("../outputroots/PWOpAWCNew.root");
    if (!file) {
        return;
    }
    


for(int i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
  }
   Double_t NomAmp[13]={15,20,25,30,35,40,45,50,55,60,65,75,80};
// Double_t NomAmp[7]={16,19,24,33,45,63,87};


 for (Int_t chindex = 0; chindex < 7; ++chindex) {
for (Int_t pulseindex =0; pulseindex < 15; ++pulseindex){



	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201AW_%d_%d",chindex,pulseindex)));
        TString legendEntryC = TString::Format("Ch %d",chindex);
	Double_t wind =1.0;


		for(Int_t Ampindex = 0; Ampindex < 13; ++Ampindex){

		 	
			TF1* fitfunc;
			TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 
			if(proj->GetEntries() == 0){//sometimes, there is no data for a particular range of Amplitudes. Skip these plots.
			continue;
			}else{
			Int_t maxbin = proj->GetMaximumBin();
			Double_t peak = proj->GetBinCenter(maxbin);
			fitfunc = new TF1("fitfunc","gaus",peak-5,peak+5);
                        proj->Fit(fitfunc,"RQ");
                        Double_t timeres = fitfunc->GetParameter(2);
			Double_t resErr = fitfunc->GetParError(2);
			std::cout<<"time error of: "<<resErr<<std::endl;
		//	std::cout<<"the width error is: "<<fitfunc->GetParError(2)<<std::endl;
		timvsAmp[chindex][pulseindex]->SetPoint(Ampindex,NomAmp[Ampindex],timeres);
		timvsAmp[chindex][pulseindex]->SetPointError(Ampindex,0,resErr);
}
}
plotfile->cd();
timvsAmp[chindex][pulseindex]->Write(Form("PWOp_TVA_Ch%d_Thr%d",chindex,pulseindex));
}
}
file->Close();
plotfile->Close();
}//Close the plotarray function

void plotting(){
for(int i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
}
TCanvas* chan[7];
TLegend* leg[7];
for (int chindex = 0; chindex < 7; chindex++){
chan[chindex] = new TCanvas(Form("Channel %d: Timing Resolution Vs. Amplitude",chindex),Form("Channel %d: Timing Resolution Vs. Amplitude",chindex),800,600);
leg[chindex] = new TLegend(0.7, 0.7, 0.9, 0.9);
chan[chindex]->cd();
for (int pulseindex = 0; pulseindex < 4; pulseindex++){
leg[chindex]->AddEntry(timvsAmp[chindex][pulseindex],Form("Thresh %d (~%d mV)",pulseindex,intThr[pulseindex]),"lp");
timvsAmp[chindex][pulseindex]->SetLineColor(icol[pulseindex]);
timvsAmp[chindex][pulseindex]->SetMarkerColor(icol[pulseindex]);
timvsAmp[chindex][pulseindex]->SetTitle(Form("PWOp Channel %d: Timing Resolution Vs. Amplitude",chindex));
timvsAmp[chindex][pulseindex]->GetXaxis()->SetTitle("Amplitude (mV)");
timvsAmp[chindex][pulseindex]->GetYaxis()->SetTitle("Timing Resolution (ns)");
timvsAmp[chindex][pulseindex]->GetXaxis()->SetRangeUser(12,85);
timvsAmp[chindex][pulseindex]->GetYaxis()->SetRangeUser(0.1,0.7);
if (pulseindex == 0){
timvsAmp[chindex][pulseindex]->Draw();
}else{
timvsAmp[chindex][pulseindex]->Draw("SAME");
}
}
leg[chindex]->Draw();
chan[chindex]->Print(Form("plots/TVA/PWOp_Ch%d_TVA.png",chindex));
}


}

int main(){
initial();
plotarray();
plotting();

return 0;
}
