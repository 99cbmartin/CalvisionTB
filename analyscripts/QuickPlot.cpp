#include <iostream>
#include <TFile.h>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
    auto fitAndPrintWidth = [](TH1D* hist, double rangeMin, double rangeMax) {
        TF1 *fitFunc = new TF1("fitFunc", "gaus", rangeMin, rangeMax);
        hist->Fit(fitFunc, "QRN");
        double width = fitFunc->GetParameter(2);
        std::cout << "For " << hist->GetName() << " the width is " << width << std::endl;
    };



int main(){
TFile *file = TFile::Open("../../outputroots/PWOpFSDL.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file " << std::endl;
	}
    TH1D *h201A0HIT_4 = (TH1D*)file->Get("h201A0HIT_4");
    TH1D *h201A01HIT_4 = (TH1D*)file->Get("h201A01HIT_4");
    TH1D *h201A012HIT_4 = (TH1D*)file->Get("h201A012HIT_4");
    TH1D *h201A0123HIT_4 = (TH1D*)file->Get("h201A0123HIT_4");

    fitAndPrintWidth(h201A0HIT_4, -5, 5);
    fitAndPrintWidth(h201A01HIT_4, -5, 5);
    fitAndPrintWidth(h201A012HIT_4, -5, 5);
    fitAndPrintWidth(h201A0123HIT_4, -5, 5);

    TCanvas *c1 = new TCanvas("c1", "Combined Canvas 1", 800, 600);
    h201A0HIT_4->SetLineColor(kRed);
    h201A01HIT_4->SetLineColor(kOrange);
    h201A012HIT_4->SetLineColor(kBlue);
    h201A0123HIT_4->SetLineColor(kGreen);
    h201A0123HIT_4->GetXaxis()->SetRangeUser(-5,5);
    h201A0123HIT_4->SetStats(0);
    h201A0123HIT_4->Draw();
    h201A012HIT_4->Draw("SAME");
    h201A01HIT_4->Draw("SAME");
    h201A0HIT_4->Draw("SAME");

    TLegend* leg1 = new TLegend(0.1,0.7,0.4,0.9);
    leg1->AddEntry(h201A0HIT_4,"Channel 0","l");
    leg1->AddEntry(h201A01HIT_4,"Avg. Channels 0 and 1", "l");
    leg1->AddEntry(h201A012HIT_4,"Avg. Channels 0,1, and 2", "l");
    leg1->AddEntry(h201A0123HIT_4,"Avg. Channels 0,1,2, and 3", "l");
    leg1->Draw();
    c1->Update();
    //c1->SaveAs("PWOpF_BackEndHIT.C");

    TH1D *h201A4HIT_9 = (TH1D*)file->Get("h201A4HIT_9");
    TH1D *h201A45HIT_9 = (TH1D*)file->Get("h201A45HIT_9");
    TH1D *h201A456HIT_9 = (TH1D*)file->Get("h201A456HIT_9");

    fitAndPrintWidth(h201A4HIT_9, -5, 5);
    fitAndPrintWidth(h201A45HIT_9, -5, 5);
    fitAndPrintWidth(h201A456HIT_9, -5, 5);

    TCanvas *c6 = new TCanvas("c6", "Combined Canvas 2", 800, 600);
    h201A4HIT_9->SetLineColor(kRed);
    h201A45HIT_9->SetLineColor(kOrange);
    h201A456HIT_9->SetLineColor(kGreen);
    h201A456HIT_9->GetXaxis()->SetRangeUser(-5,5);
    h201A456HIT_9->SetStats(0);
    h201A456HIT_9->Draw();
    h201A45HIT_9->Draw("SAME");
    h201A4HIT_9->Draw("SAME");


    TLegend* leg2 = new TLegend(0.1,0.7,0.4,0.9);
    leg2->AddEntry(h201A4HIT_9,"Channel 4","l");
    leg2->AddEntry(h201A45HIT_9,"Avg. Channels 4 and 5", "l");
    leg2->AddEntry(h201A456HIT_9,"Avg. Channels 4,5, and 6", "l");
    leg2->Draw();
    c6->Update();
    c6->SaveAs("PWOpF_FrontEndHITbest.C");



    file->Close();
}

