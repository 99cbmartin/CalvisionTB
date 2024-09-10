#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>

void saveCombinedPlotImage() {
    TFile* file = TFile::Open("finalroots/PWOparfilter.root");
    if (!file) {
        return;
    }
TFile* file2 = TFile::Open("finalroots/PWOpar.root");
	if (!file){
	return;
	}
 int icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };

 TCanvas* cans[2];  
 cans[0] = new TCanvas("Front End SiPMs", "Front", 3000,2800);
 cans[1] = new TCanvas("Back End SiPMs","Back",3000,2800);
 TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
TLegend* legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
cans[1]->cd();
    for (Int_t i = 0; i < 4; ++i) {
        TH1D* plot = dynamic_cast<TH1D*>(file->Get(Form("h105_%d", i)));
        if (!plot) {
            continue;
        	     }
	TH1D* plota = dynamic_cast<TH1D*>(file2->Get(Form("h105_%d", i)));
        if (!plot) {
            continue;
                     }
        

        plot->SetLineColor(icol[i]);
	plota->SetLineColor(icol[i]);
	plota->SetLineWidth(3);
	plot->SetLineWidth(3);
	plot->SetLineStyle(10);
        TString legendEntry = TString::Format("Ch %d Filtered", i);
	TString legendEntrya = TString::Format("Ch %d", i);
        legend->AddEntry(plot, legendEntry, "l");
	legend->AddEntry(plota, legendEntrya, "l");
 plot->GetYaxis()->SetTitle("Events");
        if (i == 0) {
            plot->Draw("HIST");
		plot->SetTitle("Back End SiPMs");
	//	plot->GetYaxis()->SetTitle("Events");
	    plota->Draw("HIST SAME");
	    plot->GetXaxis()->SetRangeUser(-10,60);//See if this does the trick
	    plota->GetXaxis()->SetRangeUser(-10,60);
	    plot->GetYaxis()->SetRangeUser(0,70000);
	    plota->GetYaxis()->SetRangeUser(0,70000);
                                                   } else {
           						   plot->Draw("HIST SAME");
							   plota->Draw("HIST SAME");
       										   }
   										    }

legend->Draw();
cans[0]->cd();
for (Int_t i = 4; i < 7; ++i) {
        TH1D* plot2 = dynamic_cast<TH1D*>(file->Get(Form("h105_%d", i)));
        if (!plot2) {
            continue;
        }

	TH1D* plotb = dynamic_cast<TH1D*>(file2->Get(Form("h105_%d", i)));
        if (!plotb) {
            continue;
                     }
       plotb->SetLineColor(icol[i]);
       plot2->SetLineColor(icol[i]);
       plot2->SetLineWidth(3);
       plotb->SetLineWidth(3);
	TString legendEntryb = TString::Format("Ch %d", i);
       TString legendEntry = TString::Format("Ch %d Filtered", i);
       legend2->AddEntry(plot2, legendEntry, "l");
	legend2->AddEntry(plotb, legendEntryb, "l");
       plot2->SetLineStyle(10); 
plot2->GetYaxis()->SetTitle("Events");
       if (i == 4) {
                     plot2->Draw("HIST");
		    plot2->SetTitle("Front End SiPMs");	
		    //plot2->GetYaxis()->SetTitle("Events"); 
		    plotb->Draw("HIST SAME");
                     plot2->GetXaxis()->SetRangeUser(-10,60);//See if this does the trick
                     plot2->GetYaxis()->SetRangeUser(0,30000);
		     plotb->GetXaxis()->SetRangeUser(-10,60);
                     plotb->GetYaxis()->SetRangeUser(0,30000);
                       } else {
                               plot2->Draw("HIST SAME");
				plotb->Draw("HIST SAME");
                                                       }
                                                         }
    legend2->Draw();
	
    cans[0]->Print("SiPMAmp_PWO_Front.png");
    cans[1]->Print("SiPMAmp_PWO_Back.png");	
    delete legend;
//}
    file->Close();
    file2->Close();
}

int main() {
    saveCombinedPlotImage();
    return 0;
}

