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
for (Int_t R = 0; R < 5; ++R){
 TCanvas* cans[7];  
 cans[0] = new TCanvas("Ch 0", "Ch 0", 3000,2800);
 cans[1] = new TCanvas("Ch 1", "Ch 1", 3000,2800);
 cans[2] = new TCanvas("Ch 2", "Ch 2", 3000,2800);
 cans[3] = new TCanvas("Ch 3", "Ch 3", 3000,2800);
 cans[4] = new TCanvas("Ch 4", "Ch 4", 3000,2800);
 cans[5] = new TCanvas("Ch 5", "Ch 5", 3000,2800);
 cans[6] = new TCanvas("Ch 6", "Ch 6", 3000,2800);
 TLegend* legend[4];// = new TLegend(0.7, 0.7, 0.9, 0.9);
TLegend* legend2[3];// = new TLegend(0.7, 0.7, 0.9, 0.9);
//Plot Back end
//cans[1]->cd();
    for (Int_t i = 0; i < 4; ++i) {
	cans[i]->cd();
        TH1D* plot = dynamic_cast<TH1D*>(file->Get(Form("h11%d_%d",R+1, i)));
        if (!plot) {
            continue;
        	     }
	TH1D* plota = dynamic_cast<TH1D*>(file2->Get(Form("h11%d_%d",R+1, i)));
        if (!plota) {
            continue;
                     }
        

        plot->SetLineColor(icol[i]);
	plota->SetLineColor(icol[i]);
	plota->SetLineWidth(3);
	plot->SetLineWidth(3);
	plot->SetLineStyle(10);
        TString legendEntry = TString::Format("Ch %d Filtered", i);
	TString legendEntrya = TString::Format("Ch %d", i);
        legend[i] = new TLegend(0.7, 0.7, 0.9, 0.9);
        legend[i]->AddEntry(plot, legendEntry, "l");
	legend[i]->AddEntry(plota, legendEntrya, "l");
        plot->GetYaxis()->SetTitle("Events");
       
       // if (i == 0) {
            plot->Draw("HIST");
	    plot->SetTitle(Form("Back End SiPMs, Ch_%d_Ring_%d",i,R+1));
	//	plot->GetYaxis()->SetTitle("Events");
	    plota->Draw("HIST SAME");
	    cans[i]->Update();
	    plot->GetXaxis()->SetRangeUser(-10,60);//See if this does the trick
	    plota->GetXaxis()->SetRangeUser(-10,60);
	    plot->GetYaxis()->SetRangeUser(0,1000);
	    plota->GetYaxis()->SetRangeUser(0,1000);
	    legend[i]->Draw();
   										    }

for (Int_t i = 4; i < 7; ++i) {
        cans[i]->cd();
        TH1D* plot2 = dynamic_cast<TH1D*>(file->Get(Form("h11%d_%d",R+1, i)));
        if (!plot2) {
            continue;
        }

	TH1D* plotb = dynamic_cast<TH1D*>(file2->Get(Form("h11%d_%d",R+1, i)));
        if (!plotb) {
            continue;
                     }
       plotb->SetLineColor(icol[i]);
       plot2->SetLineColor(icol[i]);
       plot2->SetLineWidth(3);
       plotb->SetLineWidth(3);
	TString legendEntryb = TString::Format("Ch %d", i);
       TString legendEntry = TString::Format("Ch %d Filtered", i);
       legend2[i]= new TLegend(0.7, 0.7, 0.9, 0.9);
	legend2[i]->AddEntry(plot2, legendEntry, "l");
	legend2[i]->AddEntry(plotb, legendEntryb, "l");
       plot2->SetLineStyle(10); 
plot2->GetYaxis()->SetTitle("Events");
      // if (i == 4) {
                     plot2->Draw("HIST");
		    plot2->SetTitle(Form("Front End SiPMs Ch_%d_Ring_%d",i,R+1));	
		    //plot2->GetYaxis()->SetTitle("Events"); 
		    plotb->Draw("HIST SAME");
			cans[i]->Update();			 
                     plot2->GetXaxis()->SetRangeUser(-10,60);//See if this does the trick
                     plot2->GetYaxis()->SetRangeUser(0,1000);
		     plotb->GetXaxis()->SetRangeUser(-10,60);
                     plotb->GetYaxis()->SetRangeUser(0,1000);
                     legend2[i]->Draw();
	 //               } else {
          //                     plot2->Draw("HIST SAME");
	//			plotb->Draw("HIST SAME");
          //                                             }
         // delete cans[i];
                                                         }
 //   legend2->Draw();
	
    cans[0]->Print(Form("pull2/SiPMAmp_PWO_Ch0_R%d.png",R+1));
    cans[1]->Print(Form("pull2/SiPMAmp_PWO_Ch1_R%d.png",R+1));	
    cans[2]->Print(Form("pull2/SiPMAmp_PWO_Ch2_R%d.png",R+1));
    cans[3]->Print(Form("pull2/SiPMAmp_PWO_Ch3_R%d.png",R+1));
    cans[4]->Print(Form("pull2/SiPMAmp_PWO_Ch4_R%d.png",R+1));
    cans[5]->Print(Form("pull2/SiPMAmp_PWO_Ch5_R%d.png",R+1));
    cans[6]->Print(Form("pull2/SiPMAmp_PWO_Ch6_R%d.png",R+1));
   // delete cans[i];
   // delete legend;
//
}//close my loop over Rings, R
    file->Close();
    file2->Close();
}

int main() {
    saveCombinedPlotImage();
    return 0;
}

