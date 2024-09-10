#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream> 
#include <TGraph.h>
#include <TF1.h>

int icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };
TGraph* timplots[10][7][7];//10 pulses, 7 channels, 7 Nominal Amplitudes. Should Be accessible to other functions
 TLegend* legendC = new TLegend(0,1,1,.1);
 TLegend* legendA = new TLegend(0,1,1,.1);


void plotarray() {
    TFile* file = TFile::Open("finalroots/PWOpar.root");
    if (!file) {
        return;
    }
    
    TFile* file2 = TFile::Open("finalroots/PWOparfilter.root");
    if (!file2){
	return;
    }


 Double_t NomAmp[7]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)

 TString plotNamesA[] = {"Amp 16 mV", "Amp 19 mV", "Amp 24 mV", "Amp 33 mV", "Amp 45 mV", "Amp 63 mV", "Amp 87 mV","Legend"};


for (Int_t pulseindex =0; pulseindex < 19; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code



   for (Int_t chindex = 0; chindex < 7; ++chindex) {//Will run through channels 0-6

//	TGraph* IWF = dynamic_cast<TGraph*>(file->Get(Form("grIF_%d_%d",chindex,pulseindex))); 

//	TH1D* Wideplot = new TH1D("Hist","Hist",IWF->GetN(),IWF->GetXaxis()->GetXmin(),IWF->GetXaxis()->GetXmax());//Giving Wideplot same range as IWF, may be an issue?

	TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_%d_%d",chindex,pulseindex)));
        TString legendEntryC = TString::Format("Ch %d",chindex);
	TString legendEntryA = TString::Format(plotNamesA[chindex]);
       // legend->AddEntry(Ringplots[5], legendEntry, "l");
  
	//Here, we set a threshold value from grIF and extract T_Threshold//Honestly, not needed, use Sasha's values
//	Double_t IFMax = IWF->GetMaximum();
//	Double_t IFChunk = IFMax*.05;
//	Int_t IFsteps = IFMax/IFChunk;
//	Double_t Threshsteps[IFsteps];
	Double_t wind = 2.5;//A test for a window size on the Nominal Amplitude
		//for(Int_t ind = 0; ind < IFsteps+1; ++ind){//Should Fill an Array with the Threshhold values, each being 5% the max of the grIF
		//	Threshsteps[ind]=IFChunk+ind*IFChunk;
  		//					  }


		for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){

			
			timplots[pulseindex][chindex][Ampindex] = new TGraph();//idk if this is needed
			TH1D* proj = <TH1D*>heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]+wind)),heatmap->GetXaxis()->FindBin(1/(NomAmp[Ampindex]-wind))); 
			TF1* fitfunc = new TF1("fitfunc","gauss",Wideplot->GetXaxis()->GetXmin(),Wideplot->GetXaxis()->GetXmax());
                        proj->Fit(fitfunc,"R");
                        Double_t timeres = fitfunc->GetParameter(2);
			timplots[pulseindex][chindex][Ampindex]->SetPoint(pulseindex,timeres,pulseindex);//timing plots stored in an array  




	//if (ampI[chindex] > NomAmp[Ampindex] - wind && ampI[chindex] < NomAmp[Ampindex] + wind){//!!!ampI[] is not a global from Sasha's code// Change this to a simple ProjectionY of bins, no need to ref. ampI
			
			//	for (Int_t Threshindex = 0; Threshindex < IFsteps; ++Threshindex){//Will Want to derive a timing resolution for EACH Threshold, so Fill Wideplot, reset it, Fill again, etc
			//	        Double_t TargetY = Threshsteps[Threshindex];
			//		Double_t Widepoint = IWF->GetX(TargetY);//!!!This method doesn't work as I thought. Will need to return to it and maybe cycle through bins
			//		Wideplot->Fill(Widepoint-tMCP_mean);//tMCP_mean also not a global, will need to derive it myself
					//Fit the histogram to a Gauss and find Parameter 2, the width. This will be timing resolution of a particular: Channel, nom amp, and threshhold. 
		//			TF1* fitfunc = new TF1("fitfunc","gauss",Wideplot->GetXaxis()->GetXmin(),Wideplot->GetXaxis()->GetXmax());
		//			Wideplot->Fit(fitfunc,"R");
		//			Double_t timeres = fitfunc->GetParameter(2);
		//			timplots[pulseindex][chindex][Ampindex]->SetPoint(Threshindex,timeres,Threshsteps[Threshindex]);//Point number is same as Threshindex
				
		//									   }//Close THreshold loop//Not needed		

//
//				       										}//Close If statement
}//Close Amp Loop
}//Close Channel Loop
}//Close Pulse Loop
file->Close();
file2->Close();
}//Close the plotarray function

void plotting(){//Begin the plotting scheme


for (Int_t pulseindex = 0; pulseindex < 10; ++pulseindex){
 TCanvas* canvascheck = new TCanvas("Testing","Testing",2000,1800);//A canvas to plot random sanity Checks
    TCanvas* canvasC = new TCanvas(Form("Individual Channels_Pulse_%d",pulseindex), "Individual Channels", 2000,1800);
    TCanvas* canvasA = new TCanvas(Form("Individual Nominal Amplitudes_Pulse_%d",pulseindex),"Individual Nominal Amplitudes",2000,1800);
    canvasC->Divide(3,3,0,0);//an extra Row just for the legend
    canvasA->Divide(3,3,0,0);
    canvascheck->Divide(3,2,0,0);
for (Int_t chindex = 0; chindex < 7; ++chindex){
for (Int_t Ampindex = 0; Ampindex <7; ++Ampindex){
canvasC->cd(chindex+1);
if (Ampindex == 0){
timplots[pulseindex][chindex][Ampindex]->Draw("L");
}//if close
else{
timplots[pulseindex][chindex][Ampindex]->Draw("L SAME");
}//Else close
canvasA->cd(Ampindex+1);
if (chindex == 0){
timplots[pulseindex][chindex][Ampindex]->Draw("L");
}//if close
else{
timplots[pulseindex][chindex][Ampindex]->Draw("L SAME");
}//Else close
}//Amp close
}//Chindex Close
canvasA->Print(Form("Channels_ Amplitude_ Pulse_%d.png",pulseindex));
canvasC->Print(Form("Ampltudes_Channel_Pulse_%d.png",pulseindex));
}//pulseindex close
}//Close plotting function

int main(){
plotarray();
plotting();
return 0;
}

