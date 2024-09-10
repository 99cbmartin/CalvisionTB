#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
#include <TAxis.h>
#include <TMath.h>
int main() {
    TFile* file = TFile::Open("smallSDLTGraphs.root", "READ");
    if (!file || !file->IsOpen()) {
        std::cerr << "Error: Could not open SDLTGraphs.root file!" << std::endl;
    }

    for (int ich = 0; ich < 8; ich++) {
        for (int del = 1; del <= 10; del++) {
            TString nameU = Form("SDL_Ch%d_del%d_U", ich, del);
            TString nameF = Form("SDL_Ch%d_del%d_F", ich, del);

            TGraph* graphU = (TGraph*)file->Get(nameU);
            TGraph* graphF = (TGraph*)file->Get(nameF);

            if (!graphU || !graphF) {
                std::cerr << "Error: Could not find graphs " << nameU << " or " << nameF << " in file!" << std::endl;
                continue;
            }

            double maxU = graphU->GetY()[TMath::LocMax(graphU->GetN(), graphU->GetY())];
            double maxF = graphF->GetY()[TMath::LocMax(graphF->GetN(), graphF->GetY())];

            TCanvas* canvas = new TCanvas("canvas", "Canvas", 800, 600);
            canvas->cd();

            graphU->SetLineColor(kBlue);
            graphU->SetLineWidth(2);
            graphF->SetLineColor(kRed);
            graphF->SetLineWidth(2);

            graphU->GetXaxis()->SetTitle("t - t_{MCP} (ns)");
            graphU->GetYaxis()->SetTitle("a(t) - a(t-d)");
	    graphF->GetXaxis()->SetTitle("t - t_{MCP} (ns)");
            graphF->GetYaxis()->SetTitle("a(t) - a(t-d)");
	    graphU->SetTitle(Form("Channel %d Delay %d ns",ich,del));
            graphF->SetTitle(Form("Channel %d Delay %d ns",ich,del));
            if (maxU > maxF) {
                graphU->Draw("AL");
                graphF->Draw("L SAME");
            } else {
                graphF->Draw("AL");
                graphU->Draw("L SAME");
            }

            TLegend* legend = new TLegend(0.75, 0.75, 0.9, 0.9);
            legend->AddEntry(graphU, "Unfiltered", "l");
            legend->AddEntry(graphF, "Filtered", "l");
            legend->Draw();

            canvas->Print(Form("smallSDLFVU/SDL_Ch%d_del%d.png", ich, del));

            delete legend;
            delete canvas;
        }
    }

    file->Close();
    delete file;
}

