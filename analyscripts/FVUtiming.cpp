#include <iostream>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TAxis.h>

 int NomAmp[7]={16,19,24,33,45,63,87};
void combineAndPlot(const char* file1, const char* file2) {
    TFile *f1 = new TFile(file1);
    TFile *f2 = new TFile(file2);

    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 7; ++y) {
            char graphNamePWOp[50];
            char graphNamePWOpF[50];
            sprintf(graphNamePWOp, "PWOp_Corr_Ch%d_Amp%d", x, y);
            sprintf(graphNamePWOpF, "PWOpF_Corr_Ch%d_Amp%d", x, y);

            TGraphErrors *graphPWOp = (TGraphErrors*)f1->Get(graphNamePWOp);
            TGraphErrors *graphPWOpF = (TGraphErrors*)f2->Get(graphNamePWOpF);

            graphPWOpF->SetLineColor(kRed);
	    graphPWOpF->SetMarkerColor(kRed);
            graphPWOp->SetLineColor(kGreen);
	    graphPWOp->SetMarkerColor(kGreen);

            TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
	    canvas->SetLogx();
            graphPWOp->Draw("ALP");
            graphPWOpF->Draw("LP SAME");



	graphPWOp->GetXaxis()->SetTitle("Threshold (mV*ns)");
	graphPWOp->GetYaxis()->SetTitle("timing resolution (ns)");
	graphPWOp->SetTitle(Form("Filtered Vs. UnFiltered Ch %d, Amp %d mV",x,NomAmp[y]));


            TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
            legend->AddEntry(graphPWOp, "Unfiltered", "l");
            legend->AddEntry(graphPWOpF, "Filtered", "l");
            legend->Draw();

            canvas->Update();

            char title[100];
            sprintf(title, "Filtered Versus Unfiltered: Channel %d, Amp %d", x, y);
            canvas->SetTitle(title);

            char filename[100];
            sprintf(filename, "FiltVUnFilt_Ch%d_Amp%d.png", x, y);
            //canvas->SaveAs(filename);

            Double_t minX, minY, minErrorY;
            Int_t minIndexPWOp = graphPWOp->GetMinimum();
            graphPWOp->GetPoint(minIndexPWOp, minX, minY);
            minErrorY = graphPWOp->GetErrorY(minIndexPWOp);

            Int_t minIndexPWOpF = graphPWOpF->GetMinimum();
            graphPWOpF->GetPoint(minIndexPWOpF, minX, minY);
            minErrorY = graphPWOpF->GetErrorY(minIndexPWOpF);

            std::cout << "For Amplitude " << y << ", the lowest Y value was from Channel " << x << std::endl;
            std::cout << "For Channel " << x << ", the lowest Y value was from Amplitude " << y << std::endl;

            delete canvas;
            delete legend;
        }
    }

    f1->Close();
    f2->Close();
}

int main() {
    const char* file1 = "PWOp_timing.root";
    const char* file2 = "PWOpF_timing.root";
    combineAndPlot(file1, file2);
    return 0;
}

