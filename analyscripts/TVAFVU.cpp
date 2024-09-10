#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
void combineAndPlot(const char* file1, const char* file2) {
    TFile* f1 = new TFile(file1);
    TFile* f2 = new TFile(file2);

    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 7; ++y) {
            TString name1 = Form("PWOp_TVA_Ch%d_Thr%d", x, y);
            TString name2 = Form("PWOpF_TVA_Ch%d_Thr%d", x, y);

            TGraphErrors* graph1 = (TGraphErrors*)f1->Get(name1);
            TGraphErrors* graph2 = (TGraphErrors*)f2->Get(name2);

            TCanvas* canvas = new TCanvas("canvas", "Combined Plot", 800, 600);

            if (graph1 && graph2) {
                graph1->SetLineColor(kGreen);
                graph1->SetMarkerColor(kGreen);
                graph2->SetLineColor(kRed);
                graph2->SetMarkerColor(kRed);

                graph1->Draw("ALP");
                
                graph2->Draw("LP SAME");

                TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
                legend->AddEntry(graph1, "Unfiltered", "l");
                legend->AddEntry(graph2, "Filtered", "l");
                legend->Draw();
                graph1->GetXaxis()->SetTitle("Amp (mV)");
                graph1->GetYaxis()->SetTitle("timing resolution (ns)");
                TString title = Form("Filtered Vs. Unfiltered Ch %d Threshold %d", x, y);
                graph1->SetTitle(title);

                TString filename = Form("TVAFVUplots/TVAFVUCh%dThr%d.png", x, y);
                canvas->Print(filename);
            }

            delete canvas;
        }
    }

    f1->Close();
    f2->Close();
}

int main() {
    const char* file1 = "PWOp_TVA.root";
    const char* file2 = "PWOpF_TVA.root";
    combineAndPlot(file1, file2);
    return 0;
}

