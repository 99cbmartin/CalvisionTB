#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream> 
void saveCombinedPlotImage() {
    
    TFile* file = TFile::Open("finalroots/PWOpar.root");
    if (!file) {
        return;
    }
    
    TFile* file2 = TFile::Open("finalroots/PWOparfilter.root");
    if (!file2){
	return;
    }


 int icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };
    TCanvas* canvas = new TCanvas("combined_canvas", "PWO (Parallel to Beam, With Filter) Avg. Waveform", 2000,1800);
    TCanvas* canvasB = new TCanvas("Back End SiPMs (PWO)","PWO Back",2000,1800);
    TCanvas* canvasF = new TCanvas("Front End SiPMS (PWO)", "PWO FRONT",2000,1800);
    canvas->SetTitle("PWO (Parallel to Beam, With Filter) Avg. Waveform");
    canvas->Divide(3,2,0,0);
    canvasB->Divide(3,2,0,0);
    canvasF->Divide(3,2,0,0);
    TLegend* legend = new TLegend(0,1,1,.1);
    TLegend* legendB = new TLegend(0,1,1,.1);
    TLegend* legendF = new TLegend(0,1,1,.1);   
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

TH1D* Ringplotsf[6];
    Ringplotsf[0] = dynamic_cast<TH1D*>(file2->Get(Form("h121_%d",chindex)));
    Ringplotsf[1] = dynamic_cast<TH1D*>(file2->Get(Form("h122_%d",chindex)));
    Ringplotsf[2] = dynamic_cast<TH1D*>(file2->Get(Form("h123_%d",chindex)));
    Ringplotsf[3] = dynamic_cast<TH1D*>(file2->Get(Form("h124_%d",chindex)));
    Ringplotsf[4] = dynamic_cast<TH1D*>(file2->Get(Form("h125_%d",chindex)));
    Ringplotsf[5] = new TH1D();




      //  TString legendEntry = TString::Format("Ch %d",chindex);
//	TString legendEntryf = TString::Format("Ch %d Filterd", chindex);
  //      legend->AddEntry(Ringplots[5], legendEntry, "l");
    //    legend->AddEntry(Ringplots[5],legendEntryf,"l");

      //  if (!Ringplots[chindex]) {//"Ringplot5"+ doesn't exist so it'll skip those
        //    continue;
       // }

//	if (!Ringplotsf[chindex]){
//		continue;
//	}
Amaxplots[4]->GetXaxis()->SetRangeUser(1, 100);

//if (chindex == 6){
//Ringplots[0]->GetXaxis()->SetRangeUser(0,30);
//	}

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
   
//if(chindex == 6){
//    Ringplots[0]->Scale(1.0/35.5); 
//}
//else{}
    Ringplots[0]->Scale(1.0/Ringplots[0]->GetMaximum());
    Ringplots[1]->Scale(1.0/Ringplots[1]->GetMaximum());
    Ringplots[2]->Scale(1.0/Ringplots[2]->GetMaximum());
    Ringplots[3]->Scale(1.0/Ringplots[3]->GetMaximum());
    Ringplots[4]->Scale(1.0/Ringplots[4]->GetMaximum());

    Ringplotsf[0]->Scale(1.0/Ringplotsf[0]->GetMaximum()); 
    Ringplotsf[1]->Scale(1.0/Ringplotsf[1]->GetMaximum());
    Ringplotsf[2]->Scale(1.0/Ringplotsf[2]->GetMaximum());
    Ringplotsf[3]->Scale(1.0/Ringplotsf[3]->GetMaximum());
    Ringplotsf[4]->Scale(1.0/Ringplotsf[4]->GetMaximum());
std::cout << "Ringplots 0-4 in the channel:" << chindex << "Has a maximum at " << Ringplots[0]->GetMaximum()<<" " <<Ringplots[1]->GetMaximum()<<" "<< Ringplots[2]->GetMaximum()<<" "<<Ringplots[3]->GetMaximum()<<" "<<Ringplots[4]->GetMaximum() << std::endl;

        
   for (Int_t plotindex=0; plotindex<6; ++plotindex){//Loop through the combined plots
	
	Ringplots[plotindex]->SetLineColor(icol[chindex]);
	Ringplots[plotindex]->SetLineWidth(1);
	Ringplots[plotindex]->SetTitle(plotNames[plotindex]);
        Ringplots[plotindex]->GetYaxis()->SetRangeUser(0.0,1.2);
	Ringplots[plotindex]->SetNdivisions(5);
	Ringplots[plotindex]->GetYaxis()->SetTitle("Amp/Amp_max (mv)");	 
	//Set filtered Data attributes (dashed lines Esp)
	Ringplotsf[plotindex]->SetLineColor(icol[chindex]);
        Ringplotsf[plotindex]->SetLineWidth(4);
        Ringplotsf[plotindex]->SetTitle(plotNames[plotindex]);
        Ringplotsf[plotindex]->GetYaxis()->SetRangeUser(0.0,1.2);
        Ringplotsf[plotindex]->SetNdivisions(5);
        Ringplotsf[plotindex]->GetYaxis()->SetTitle("Amp/Amp_max (mv)");
	Ringplotsf[plotindex]->SetLineStyle(3);

if (plotindex == 0){
 TString legendEntry = TString::Format("Ch %d",chindex);
        TString legendEntryf = TString::Format("Ch %d Filtered", chindex);
        legend->AddEntry(Ringplots[5], legendEntry, "l");
        legend->AddEntry(Ringplotsf[5],legendEntryf,"l");
if (chindex <4){
 legendB->AddEntry(Ringplots[5], legendEntry, "l");
 legendB->AddEntry(Ringplotsf[5],legendEntryf,"l");
}else if(chindex >= 4){
 legendF->AddEntry(Ringplots[5], legendEntry, "l");
 legendF->AddEntry(Ringplotsf[5],legendEntryf,"l");
}
        if (!Ringplots[chindex]) {//"Ringplot5"+ doesn't exist so it'll skip those
            continue;
        }

        if (!Ringplotsf[chindex]){
                continue;
        }
}



	canvas->cd(plotindex+1);
	if (chindex == 0){
	Ringplots[plotindex]->Draw("HIST");
	Ringplotsf[plotindex]->Draw("HIST SAME");
     }  else{
	Ringplots[plotindex]->Draw("HIST SAME");
	Ringplotsf[plotindex]->Draw("HIST SAME");
       }

canvasB->cd(plotindex+1);
        if (chindex == 0 && chindex < 4){
        Ringplots[plotindex]->Draw("HIST");
	Ringplotsf[plotindex]->Draw("HIST SAME");
     }  else if (chindex < 4){
        Ringplots[plotindex]->Draw("HIST SAME");
	Ringplotsf[plotindex]->Draw("HIST SAME");
       }

canvasF->cd(plotindex+1);
        if (chindex == 4){
        Ringplots[plotindex]->Draw("HIST");
	Ringplotsf[plotindex]->Draw("HIST SAME");
     }  else if (chindex > 4){
        Ringplots[plotindex]->Draw("HIST SAME");
	Ringplotsf[plotindex]->Draw("HIST SAME");
       }



}

        }
   canvas->cd(6);
   legend->Draw(); 
  canvasB->cd(6);
legendB->Draw();
canvasF->cd(6);
legendF->Draw(); 
    	canvas->Print("test1/PWO_AvgPulse_All.png");
	canvasB->Print("test1/PWO_AvgPulse_Back.png");
	canvasF->Print("test1/PWO_AvgPulse_Front.png");

    delete canvas;
    file->Close();
	file2->Close();
}

int main() {
    saveCombinedPlotImage();
    return 0;
}

