
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


    TCanvas* canvas = new TCanvas("canvas","canvas", 2000,1800);
	Divide->(4,2,0,0);
 TLegend* legend = new TLegend(0,1,1,.1);
    //for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6
	for(Int_t wi = 0; wi < 9; ++wi){//Runnign through these extra WF indices
TH1D* chplots[8];
    chplots[0] = dynamic_cast<TH1D*>(file->Get(Form("grIF_0_%d",wi)));
    chplots[1] = dynamic_cast<TH1D*>(file->Get(Form("grIF_1_%d",wi)));
    chplots[2] = dynamic_cast<TH1D*>(file->Get(Form("grIF_2_%d",wi)));
    chplots[3] = dynamic_cast<TH1D*>(file->Get(Form("grIF_3_%d",wi)));
    chplots[4] = dynamic_cast<TH1D*>(file->Get(Form("grIF_4_%d",wi)));
    chplots[5] = dynamic_cast<TH1D*>(file->Get(Form("grIF_5_%d",wi)));
    chplots[6] = dynamic_cast<TH1D*>(file->Get(Form("grIF_6_%d",wi)));
    chplots[7] = new TH1D;
        TString legendEntry = TString::Format("WF %d",wi);
        legend->AddEntry(Ringplots[7], legendEntry, "l");






        }
