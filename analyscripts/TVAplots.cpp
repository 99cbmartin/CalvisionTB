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

Int_t icol[7] = {kBlack,kRed,kBlue,kGreen+2,kOrange,kMagenta };

TGraphErrors* Terr[7][6];
void plotarray() {
    TFile* file = TFile::Open("PWOpF_TVA.root");
    if (!file) {
        return;
    }
    


 for (Int_t chindex = 0; chindex < 7; ++chindex) {
TCanvas* can1 = new TCanvas(Form("PWOpF Channel %d TVA",chindex),Form("PWOpF Channel %d TVA",chindex), 800,600);
can1->SetRightMargin(0.25);
TLegend* leg1 = new TLegend(0.75,0,1,0.3);
for (Int_t pulseindex =0; pulseindex < 6; ++pulseindex){



	 Terr[chindex][pulseindex] = dynamic_cast<TGraphErrors*>(file->Get(Form("PWOpF_TVA_Ch%d_Thr%d",chindex,pulseindex)));
	double room = .05;
		can1->cd();
	leg1->AddEntry(Terr[chindex][pulseindex],Form("Thr %d",pulseindex),"LP");
	Terr[chindex][pulseindex]->SetLineColor(icol[pulseindex]);
	Terr[chindex][pulseindex]->SetTitle(Form("PWOpF Timing Resolution Vs. Amplitude: Ch %d",chindex));
	Terr[chindex][pulseindex]->SetLineWidth(2);
	Terr[chindex][pulseindex]->GetYaxis()->SetRangeUser(0.1,0.65);
	Terr[chindex][pulseindex]->GetYaxis()->SetTitle("Timing Resolution (ns)");
	Terr[chindex][pulseindex]->GetXaxis()->SetTitle("Amp (mV)");
	if (pulseindex ==0){
	Terr[chindex][pulseindex]->Draw();
	}else{
	 Terr[chindex][pulseindex]->Draw("SAME");
}
leg1->Draw();
can1->Update();		 	

}
can1->Print(Form("ram/PWOpF_TVA_Ch%d.png",chindex));
}

file->Close();
}//Close the plotarray function

































int main(){
plotarray();

return 0;
}
