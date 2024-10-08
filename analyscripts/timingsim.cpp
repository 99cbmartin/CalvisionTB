#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <iostream> 
#include <TGraph.h>
#include <TF1.h>

Int_t icol[7] = { kOrange+10, kOrange-3, kSpring-1, kAzure+6, kAzure-1, kViolet-1, kBlack };
TGraph* timplots[7];//  7 Nominal Amplitudes. 

const Int_t nThr = 20;
Double_t intThr[nThr];
  
TCanvas* heat[20];//all 20 heatmaps
TCanvas* projects[7];//7 random projections



void plotarray() {
    TFile* file = TFile::Open("finalroots/PWOpar.root");
    if (!file) {
        return;
    }
    


for(Int_t i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
}
 Double_t NomAmp[7]={16,19,24,33,45,63,87};//Nominal Amplitudes for Adjusment (mV)


for (Int_t pulseindex =0; pulseindex < 19; ++pulseindex){//Run over 20 Threshold values in the code, Line 30 intThr of Sasha's Code

TH2D* heatmap = dynamic_cast<TH2D*>(file->Get(Form("h201_4_%d",pulseindex)));

Double_t wind =2.0;
heat[pulseindex]= new TCanvas((Form("heatmap_%d",pulseindex),"heat Canvas",2000,1800));
heat[pulseindex]->cd();
heatmap->Draw();
heat[pulseindex]->Print(Form("pull/heatmap_1_%d.png",pulseindex));

		for(Int_t Ampindex = 0; Ampindex < 7; ++Ampindex){

			timplots[Ampindex] = new TGraph();
			TH1D* proj = (TH1D*)heatmap->ProjectionY("proj",heatmap->GetXaxis()->FindBin(1/(63+wind)),heatmap->GetXaxis()->FindBin(1/(63-wind))); 
			projects[Ampindex] =new TCanvas(Form("proj %d ",Ampindex),"Canvas",2000,1800);
			projects[Ampindex]->cd();
			proj->Draw("HIST");
			projects[Ampindex]->Print(Form("pull/Projection_%d.png",pulseindex));
			if(proj->GetEntries() == 0){//sometimes, there is no data for a particular range of Amplitudes. Skip these plots.
			continue;
			}else{
			TF1* fitfunc = new TF1("fitfunc","gaus",proj->GetXaxis()->GetXmin(),proj->GetXaxis()->GetXmax());
                        proj->Fit(fitfunc,"RQ");
                        Double_t timeres = fitfunc->GetParameter(2);
			timplots[Ampindex]->SetPoint(pulseindex,intThr[pulseindex],timeres);//timing plots stored in an array  
		timplots[Ampindex]->SetMarkerSize(2.0);
		timplots[Ampindex]->SetMarkerStyle(20);
//		timplots[Ampindex]->GetXaxis()->SetRangeUser(0, 300);
		timplots[Ampindex]->GetYaxis()->SetRangeUser(0, 1);
std::cout<<"For Channel 1: and Amplitude:"<<NomAmp[Ampindex]<<" We will see a timing res:"<<timeres<<" for Thresh of"<<intThr[pulseindex]<<std::endl;	
}


}//Close Amp Loop
}//Close Pulse Loop
file->Close();
}//Close the plotarray function

void plotting(){//Begin the plotting scheme


    TCanvas* canvasC = new TCanvas("Individual Channels", "Individual Channels", 4000,3600);
    canvasC->SetLogx();
for (Int_t Ampindex = 0; Ampindex <7; ++Ampindex){
Int_t nump=timplots[Ampindex]->GetN();
std::cout<<"num points in Ch:1 and Amp:"<<Ampindex<<" is:"<<nump<<std::endl;
canvasC->cd();
if (Ampindex == 0){
timplots[Ampindex]->Draw("AP");
}//if close
else{
timplots[Ampindex]->Draw("AP SAME");
}//Else close
//canvascheck->cd();
//canvascheck->SetLogx();
//timplots[1]->SetPoint(3,10,0.2);
//timplots[1]->Draw("AP");
for (Int_t t=0; t<20; t++){
Double_t xval= timplots[Ampindex]->GetX()[t];
Double_t yval = timplots[Ampindex]->GetY()[t];
std::cout<<"point:"<<t<<" xval:"<<xval<<" and yval:"<<yval<<std::endl;
}
}//Amp close
canvasC->Print("pull/testingimg.png");
//canvascheck->Print("pull/wtf.png");
}//Close plotting function

int main(){
plotarray();
plotting();
return 0;
}

