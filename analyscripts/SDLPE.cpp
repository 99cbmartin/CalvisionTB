//This script is meant to perform the SDL PE count analysis on FNAL data (2023). The script will take in two files. One is the fixed PbF2 output root file (this will remain constant). From this file I will pull the average pulse and the SDL pulse. With these, I will construct the fit function. The other file will be whichever is relevant to perform a component fit to. 

#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream>
#include <TGraph.h>
#include <TAxis.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TProfile.h>
#include <TMath.h>

int main(){
	TString* sourcefiles{2}={PbF2file.root,pulsefile.root};
	TFile* PbF2file = TFile::Open(Form("../outputroots/%s",sourcefiles[0]));
	TFile* pulsefile = TFile::Open(Form("../outputroots/%s",sourcefiles[1]));
   	 if (!PbF2file) {
       		return;
    	}
    	if (!pulsefile) {
		return;
    	}
double tau = 314; //(ns) Determined by Renyan
TF1* decayFunc = new TF1("decayFunc", "(1/[0])*exp(-t/[0])", 0, 1000); 
decayFunc->SetParameters(tau);

TF1 *fitty = new TF1("fitty", [](double *x, double *par) {
    double t = x[0];      // Independent variable
    double pC = par[0];   // Parameter pC
    double pS = par[1];   // Parameter pS
    double t0 = par[2];   // Parameter t0


for (int chindex = 0 ; chindex < 7; chindex++){
	
	PbF2file->cd();
	TProfile* PbAvgPulse = dynamic_cast<TProfile*>(file->Get(Form("AvgPulseAug_%d",chindex));
	TH1D* PbDist = dynamic_cast<TH1D*>(PbF2file->Get(Form("h105_%d",chindex));
	TProfile* PbSDLPulse = dynamic_cast<TProfile*>(PbF2file->Get(Form("SDLpulse_%d",chindex));
	//Performing Convolution Between normalized SDL PbF2 Pulse and the Decay Function above
	TH1D* PbSDLHist = PbSDLPulse->ProjectionX();
	ROOT::Math::Convolution conv(PbSDLHist,decayFunc);
	TH1D* Convol = (TH1D*)PbSDLHist->Clone("Convol");
	conv(Convol,false);
	
	//Now, I have the need to define f_{C} and f_{S} where I will need to perform conversions to TF1's
	
	TF1 *FC = new TF1("FC", "gaus", Convol->GetXaxis()->GetXmin(), Convol->GetXaxis()->GetXmax());
	Convol->Fit(FC,"R");
	TF1 *FS = new TF1("FS", "gaus", PbAvgPulse->GetXaxis()->GetXmin(), PbAvgPulse->GetXaxis()->GetXmax());
	PbAvgPulse->Fit(FS,"R");
	
	FC->Eval(t-t0);
	FS->Eval(t-t0);// Little stuck here in making a composite function to fit to Pulses below
	
	//========================================================//
	// PbF2 Pulses casted above, pulse to be fit casted below //
	//========================================================//
	
	pulsefile->cd();
	TProfile* AvgPulse = dynamic_cast<TProfile*>(file->Get(Form("AvgPulseAug_%d",chindex));
        TH1D* Dist = dynamic_cast<TH1D*>(PbF2file->Get(Form("h105_%d",chindex));
        TProfile* SDLPulse = dynamic_cast<TProfile*>(PbF2file->Get(Form("SDLpulse_%d",chindex));




	}



















}
