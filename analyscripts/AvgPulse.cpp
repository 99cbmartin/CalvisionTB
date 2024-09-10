#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream> 
void saveCombinedPlotImage() {
    
    TFile* file = TFile::Open("BGOfiltered.root");
    if (!file) {
        return;
    }


    TCanvas* canvas = new TCanvas("combined_canvas", "BGO (Parallel to Beam, With Filter) Avg. Waveform", 2000,1800);
    canvas->SetTitle("BGO (Parallel to Beam, With Filter) Avg. Waveform");
    canvas->Divide(3,2,0,0);
    TLegend* legend = new TLegend(0,1,1,.1);
    TH1D* legendTH1D=new TH1D();
    TString plotNames[] = {"Avg Pulse Shape: R<2", "Avg Pulse Shape: 2<R<4", "Avg Pulse Shape: 4<R<8", "Avg Pulse Shape: 8<R<12", "Avg Pulse Shape: R>12","Legend"};
    for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6
 
TH1D* Amaxplots[5];
	 Amaxplots[0] = dynamic_cast<TH1D*>(file->Get(Form("h221_%d",chindex)));
	 Amaxplots[1] = dynamic_cast<TH1D*>(file->Get(Form("h222_%d",chindex)));
	 Amaxplots[2] = dynamic_cast<TH1D*>(file->Get(Form("h223_%d",chindex)));
	 Amaxplots[3] = dynamic_cast<TH1D*>(file->Get(Form("h224_%d",chindex)));
	 Amaxplots[4] = dynamic_cast<TH1D*>(file->Get(Form("h225_%d",chindex)));
TH1D* Ringplots[6];
    Ringplots[0] = dynamic_cast<TH1D*>(file->Get(Form("h121_%d",chindex)));
    Ringplots[1] = dynamic_cast<TH1D*>(file->Get(Form("h122_%d",chindex)));
    Ringplots[2] = dynamic_cast<TH1D*>(file->Get(Form("h123_%d",chindex)));
    Ringplots[3] = dynamic_cast<TH1D*>(file->Get(Form("h124_%d",chindex)));
    Ringplots[4] = dynamic_cast<TH1D*>(file->Get(Form("h125_%d",chindex)));
    Ringplots[5] = new TH1D();
        TString legendEntry = TString::Format("Ch %d",chindex + 1);
        legend->AddEntry(Ringplots[5], legendEntry, "l");

        if (!Ringplots[chindex]) {//"Ringplot5"+ doesn't exist so it'll skip those
            continue;
        }

Amaxplots[4]->GetXaxis()->SetRangeUser(1, 100);

if (chindex == 6){
Ringplots[0]->GetXaxis()->SetRangeUser(0,30);
	}

Double_t maxVals[5];
for ( Int_t maxindex = 0; maxindex<5; maxindex++){

Double_t bin=Amaxplots[maxindex]->GetMaximumBin();

maxVals[maxindex]=Amaxplots[maxindex]->GetXaxis()->GetBinCenter(bin);

//Double_t check = (maxVals[maxindex] / Ringplots[maxindex]->GetMaximum())*100.0;

//if ( check < 70){//An aribitrary Check to ensure we don't get false peak around 0

//for (Int_t bins = 1 ; bins <= Amaxplots[maxindex]->GetNbinsX(); ++bins) {

//    Double_t binCenter = Amaxplots[maxindex]->GetXaxis()->GetBinCenter(bins);
//    Double_t binContent = Amaxplots[maxindex]->GetBinContent(bins);

//    if (binCenter != 0.0 && binContent > maxVals[maxindex]) {
//              maxVals[maxindex] = binCenter;
//    }
//}
 
//}


std::cout << "maxVals[" << maxindex << "] = " << maxVals[maxindex] << std::endl;
}
   

    Ringplots[0]->Scale(1.0/Ringplots[0]->GetMaximum()); 
    Ringplots[1]->Scale(1.0/Ringplots[1]->GetMaximum());
    Ringplots[2]->Scale(1.0/Ringplots[2]->GetMaximum());
    Ringplots[3]->Scale(1.0/Ringplots[3]->GetMaximum());
    Ringplots[4]->Scale(1.0/Ringplots[4]->GetMaximum());
std::cout << "Ringplots 0-4 in the channel:" << chindex << "Has a maximum at " << Ringplots[0]->GetMaximum()<<" " <<Ringplots[1]->GetMaximum()<<" "<< Ringplots[2]->GetMaximum()<<" "<<Ringplots[3]->GetMaximum()<<" "<<Ringplots[4]->GetMaximum() << std::endl;

        
   for (Int_t plotindex=0; plotindex<6; ++plotindex){//Loop through the combined plots
	Ringplots[plotindex]->SetLineColor(chindex + 1);
	Ringplots[plotindex]->SetLineWidth(6);
	Ringplots[plotindex]->SetTitle(plotNames[plotindex]);
        Ringplots[plotindex]->GetYaxis()->SetRangeUser(0.0,1.2);
	Ringplots[plotindex]->SetNdivisions(5);
	Ringplots[plotindex]->GetYaxis()->SetTitle("Amp/Amp_max (mv)");	 


	canvas->cd(plotindex+1);
	if (chindex == 0){
	Ringplots[plotindex]->Draw("HIST");
     }  else{
	Ringplots[plotindex]->Draw("HIST SAME");
       }
}

        }
   canvas->cd(6);
   legend->Draw(); 

    canvas->Print("BGOparfilter_AvgPulse.png");

    delete canvas;
    file->Close();
}

int main() {
    saveCombinedPlotImage();
    return 0;
}

