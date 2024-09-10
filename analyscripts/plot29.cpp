#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>

void saveCombinedPlotImage() {
    TFile* file = TFile::Open("output.root");
    if (!file) {
        return;
    }


    TCanvas* canvas = new TCanvas("combined_canvas", "Combined Plot", 2000,1800);
    canvas->Divide(3,2,0,0);
    TLegend* legend = new TLegend(0,1,1,.1);
    TH1D* legendTH1D=new TH1D();
    TString plotNames[] = {"Avg Pulse Shape: R<2", "Avg Pulse Shape: 2<R<4", "Avg Pulse Shape: 4<R<8", "Avg Pulse Shape: 8<R<12", "Avg Pulse Shape: R>12","Legend"};
    for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 1-7
  


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

        
   for (Int_t plotindex=0; plotindex<6; ++plotindex){//Loop through the combined plots
	Ringplots[plotindex]->SetLineColor(chindex + 1);
	Ringplots[plotindex]->SetLineWidth(6);
	Ringplots[plotindex]->SetTitle(plotNames[plotindex]);




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

    canvas->Print("combotest.png");

    delete canvas;
    file->Close();
}

int main() {
    saveCombinedPlotImage();
    return 0;
}

