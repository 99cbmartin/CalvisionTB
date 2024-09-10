#include "pulse.hh"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TLegend.h"
using namespace std;

void pulse::Analysis()
{  
  if (fChain == 0) return;
 //double tmp = sqrt( pow(xtrk - x0center[ich],2) + pow(ytrk - y0center[ich],2) );
// double tmp = sqrt( pow(xtrk - x0center[ich],2) + pow(ytrk - y0center[ich],2) );
  
  double int2amp[] = { 7.31775e-02, 7.31946e-02, 7.34309e-02, 7.30913e-02, 7.79333e-02, 7.41691e-02, 7.59032e-02 };
  int chPair[3][2] = {
      {0, 4},
      {1, 6},
      {3, 5}
  };
  //double maxZ = 0.0;
  //double maxZ2 = 0.0;
  double x0center[] = { -1.0, -10.0, -1.6, -10.3, -2.2, -12.0, -11.4};  
  double y0center[] = { -1.8,  -2.1, +7.3,  +7.1, -1.8,  +6.9,  -2.1};  
  double dR[] = { 2.0, 4.0, 8.0, 12.0 };
  double NomAmp[]={16,19,24,33,45,63,87};//For timing Amplitude bins  
  const int nThr = 20;
  double intThr[nThr];
  for(int i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
  }
TFile* fitsFile = TFile::Open("PWOp_fitXFile.root");
TProfile* fitfunc2[7][nThr];
 for(int i=0; i<7; i++){
   for(int j=0; j<nThr; j++){
        TString fitname =  Form("PWOp_walkcor_C%d_T%d",i, j);
        fitfunc2[i][j] = dynamic_cast<TProfile*>(fitsFile->Get(fitname));
}
 }
  TFile *fout = new TFile("../outputroots/PWOpPlotstuff.root","recreate");


 int numr = 0; 
  double xMin = -28; 
  double xMax = +20; 
  double yMin = -24;  
  double yMax = +24;
  int nbXY = 200;
  
  TH1D *h001 = new TH1D("h001", ";N of tracks ;events", 10, -0.5, 9.5);
  TH1D *h001b = new TH1D("h001b", ";xIntecept (mm);events", nbXY, xMin, xMax);
  TH2D *h002 = new TH2D("h002", ";x_{ trk} (mm);y_{ trk} (mm); events", nbXY, xMin, xMax, nbXY, yMin, yMax);
  TH2D *h002b = new TH2D("h002b", ";x_{ trk} (mm);y_{ trk} (mm); events", nbXY, xMin, xMax, nbXY, yMin, yMax);
  TH2D *h003 = new TH2D("h003", ";slopeX_{ trk};slopeY_{ trk}; events", 200, -2e-3, 2e-3, 200, -2e-3, 2e-3);
  TH1D *h003a = new TH1D("h003a", ";slopeX_{ trk}; events", 200, -2e-3, 2e-3);
  TH1D *h003b = new TH1D("h003b", ";slopeY_{ trk}; events", 200, -2e-3, 2e-3);
  TProfile2D *h004 = new TProfile2D("h004", ";xIntercept (mm);yIntercept (mm); avg nplanes", nbXY, xMin, xMax, nbXY, yMin, yMax);

  TH1D *h005 = new TH1D("h005", "MCP ;baseline RMS (mV); events", 200, 0, 5);
  TH1D *h006 = new TH1D("h006", "MCP ;amp[7] (mV); events", 200, -50, 1000);
  TProfile2D *h007a = new TProfile2D("h007a", " MCP ;xIntercept (mm);yIntercept (mm); avg amp[7] (mV)", nbXY, xMin, xMax, nbXY, yMin, yMax);
  TProfile2D *h007b = new TProfile2D("h007b", "MCP ;x_{ trk} (mm);y_{ trk} (mm); avg amp[7] (mV)", nbXY, xMin, xMax, nbXY, yMin, yMax);
  TH2D *h008a = new TH2D("h008a", "MCP ;LP2_30mV[7] - LP2_50[7] (ns); gaus_mean[7] - LP2_50[7] (ns) ;events", 200,  -0.4, 0.1,  200,  -8, 1);
  TH2D *h008b = new TH2D("h008b", "MCP ;tMCP_30mV - tMCP_50 (ns); tMCP_mean - tMCP_50 (ns) ;events", 200,  -0.4, 0.1,  200,  -8, 1);
  TH2D *h008c = new TH2D("h008c", "MCP ;LP2_30mV[7] - LP2_50[7] (ns); gaus_mean[7] - LP2_50[7] (ns) ;events", 200,  -0.4, 0.1,  200,  0.2, 0.3);
  TH2D *h008d = new TH2D("h008d", "MCP ;tMCP_30mV - tMCP_50 (ns); tMCP_mean - tMCP_50 (ns) ;events", 200,  -0.4, 0.1,  200,  0.2, 0.3);
  TH1D *h009a = new TH1D("h009a", "MCP ;aMCP (mV); events", 200, 0, 400);
  TH1D *h009b = new TH1D("h009b", "MCP ;tMCP_mean (s); events", 200, -90e-9, -40e-9);
  TH1D *h009c = new TH1D("h009c", "MCP ;aMCP (mV); events", 200, 0, 400);
  TCanvas* Avtime[nThr]; 
  TH1D *h101[7];
  TH1D *h102[7];
  TProfile2D *h103[7];
  TProfile2D *h104[7];
  TH1D *h105[7];
  TH1D *h221[7];
  TH1D *h222[7];
  TH1D *h223[7];
  TH1D *h224[7];
  TH1D *h225[7];
  TProfile2D *h106[7];
  TH1D *h111[7];
  TH1D *h112[7];
  TH1D *h113[7];
  TH1D *h114[7];
  TH1D *h115[7];
  TProfile *h121[7];
  TProfile* AvgPulseAug[7];
  TProfile *h122[7];
  TProfile *h123[7];
  TProfile *h124[7];
  TProfile *h125[7];
  TProfile* SDLpulse[7];
  for(int i=0; i<7; i++){
      h101[i] = new TH1D(Form("h101_%d",i),Form("SiPM ;baseline_RMS[%d] (mV)",i),200, 0, 20);
      h102[i] = new TH1D(Form("h102_%d",i),Form("SiPM ;log10 amp[%d] (mV) ;events",i),200, 0, 4);
      h103[i] = new TProfile2D(Form("h103_%d",i),";t_{ i} - t_{ MCP} (ns) ;log10 A_{ max}  (mV); avg a_{ i} / A_{ max}", 1000, -50, 50, 100, 0, 3.5);
      h104[i] = new TProfile2D(Form("h104_%d",i),";t_{ i} - t_{ MCP} (ns)  ;log I  (mV*ns); avg a_{ i} (mV)", 1000, -10, 40, 100, 0, 5);      
      h105[i] = new TH1D(Form("h105_%d",i),";A'_{max} (mV); events", 832,-20,500);//Was -20-100 x domain
      h106[i] = new TProfile2D(Form("h106_%d",i), ";x_{ trk} (mm);y_{ trk} (mm); avg A'_{ max} (mV)", nbXY, xMin, xMax, nbXY, yMin, yMax);
      h111[i] = new TH1D(Form("h111_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h112[i] = new TH1D(Form("h112_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h113[i] = new TH1D(Form("h113_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h114[i] = new TH1D(Form("h114_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h115[i] = new TH1D(Form("h115_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h121[i] = new TProfile(Form("h121_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, 40);
      h122[i] = new TProfile(Form("h122_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      h123[i] = new TProfile(Form("h123_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      h124[i] = new TProfile(Form("h124_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      h125[i] = new TProfile(Form("h125_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      AvgPulseAug[i] = new TProfile(Form("AvgPulseAug_%d",i),";t_{i} (ns); average a_{i] (mV)",1000,-90,-40); 
//CM Inserts some more Histograms for Analysis of Avg. Pulse Shape:
     h221[i] = new TH1D(Form("h221_%d",i),";A'_{max} (mV); events", 200, -20, 100);//Designated One histogram for each Ring (Distance from the Center of the crystal)
     h222[i] = new TH1D(Form("h222_%d",i),";A'_{max} (mV); events", 200, -20, 100);
     h223[i] = new TH1D(Form("h223_%d",i),";A'_{max} (mV); events", 200, -20, 100);
     h224[i] = new TH1D(Form("h224_%d",i),";A'_{max} (mV); events", 200, -20, 100);
     h225[i] = new TH1D(Form("h225_%d",i),";A'_{max} (mV); events", 200, -20, 100);//I set the lower Bin starting at 1 to avoid the extreme peak around zero
     SDLpulse[i] = new TProfile(Form("SDLPulse_%d",i),"t_{i}-t_{MCP} (ns);a(t)-a(t-1)/a_{max} (mV)", 1000, -90, 40);//not sure about bounds at the moment
  }//Lazy TH2D construction below, should have looped (embarassing)
  TH1D* h105C = new TH1D("h105C","Amplitude of All Events;log10 amp (mV); events", 200, 0,5);
TGraph* amicrazy = new TGraph();

//SDL Method Approach:
TGraph* singlepulse = new TGraph();//Here I will collect single pulses for comparison and likely give a matching dynamic name for the other TGraphs
int Delays[10] = {1,2,3,4,5,6,7,8,9,10};//(100 ps per sample) These are 1-10 ns delays in units of samples just mulitply by 10
TProfile* avgSDL[8][10];
TCanvas* avgSDLc1 = new TCanvas();
for (int ich =0 ; ich <7; ich++){
for(int del=0; del < 10;del++){
 avgSDL[ich][del] = new TProfile(Form("Average SDL Ch%d del%d_ns",ich,Delays[del]),Form("Average SDL Ch%d del%d_ns",ich,Delays[del]),500,-20,40);
// avgSDL[ich][del]->SetTitle(Form("Average SDL Ch%d del%d_ns",ich,Delays[del]));
// avgSDL->GetXaxis()->SetRangeUser(-20,40);
}
}

amicrazy->SetName("crazy");
TCanvas* trigger = new TCanvas(); 
 TH2D* h201R[7][nThr]; 
for (int i =0; i <7; i++){
 for (int j=0; j < nThr ; j++){
  h201R[i][j] = new TH2D(Form("h201R_Ring%d_Thr%d",i,j),"1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, 0.01, 0.21, 350, -5, 20); 
 }
}
 //Going to use the two below to see the average waveforms of each regime in similar amplitudes Gnote: TProfile Construction
  TProfile* h126 = new TProfile("h126",";t_{i} (ns); average a_{ i} (mV)", 1000,-20,40);
  TProfile* h127 = new TProfile("h127",";t_{i} (ns); average a_{ i} (mV)", 1000,-20, 40); 
TH2D *h201[7][nThr];
TH1D *h201A0[nThr];
TH1D *h201A0HIT[nThr];
TH1D *h201A0MISS[nThr];
TH1D *h201A4[nThr];
TH1D *h201A4HIT[nThr];
TH1D *h201A4MISS[nThr];
TH1D *h201A1[nThr];
TH1D *h201A1HIT[nThr];
TH1D *h201A1MISS[nThr];
TH1D *h201A3[nThr];
TH1D *h201A3HIT[nThr];
TH1D *h201A3MISS[nThr];
TH1D *h201A2[nThr];
TH1D *h201A01[nThr];
TH1D *h201A01HIT[nThr];
TH1D *h201A01MISS[nThr];
TH1D *h201A012[nThr];
TH1D *h201A012HIT[nThr];
TH1D *h201A012MISS[nThr];
TH1D *h201A0123[nThr];
TH1D *h201A0123HIT[nThr];
TH1D *h201A0123MISS[nThr];
TH1D *h201A45[nThr];
TH1D *h201A45HIT[nThr];
TH1D *h201A45MISS[nThr];
TH1D *h201A456[nThr];
TH1D *h201A456HIT[nThr];
TH1D *h201A456MISS[nThr];
TH1D *h201Atotal[nThr];
TH1D *h201AtotalHIT[nThr];
TH1D *h201AtotalMISS[nThr];
TH2D *h201AFVB[nThr];
TH2D *h201AFVBHIT[nThr];
TH2D *h201ThVA0 = new TH2D("h201ThVA0","SiPM ; [CH 0] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); Threshold (mV*ns)",1000,-5,20,20,0,20);
TH2D *h201ThVA01= new TH2D("h201ThVA01","SiPM ; [CH 01] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); Threshold (mV*ns)",1000,-5,20,20,0,20);
TH2D *h201ThVA012 = new TH2D("h201ThVA012","SiPM ; [CH 012] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); Threshold (mV*ns)",1000,-5,20,20,0,20);
TH2D *h201ThVA0123 = new TH2D("h201ThVA0123","SiPM ; [CH 0123] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); Threshold (mV*ns)",1000,-5,20,20,0,20);
TH2D *h201ThVA45 = new TH2D("h201ThVA45","SiPM ; [CH 45] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); Threshold (mV*ns)",1000,-5,20,20,0,20);
TH2D *h201ThVA456 = new TH2D("h201ThVA456","SiPM ; [CH 456] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); Threshold (mV*ns)",1000,-5,20,20,0,20);
TH2D* h201A0V4[nThr];//Will only use 6
TH2D* h201A0V4HIT[nThr];
TH2D* h201A0V4HIT2[nThr];
TH2D* h201ThVAFB = new TH2D("h201ThVAFB","SiPM ; [CH 456] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); Threshold (mV*ns)",1000,-5,20,20,0,20);
TH2D *h201AW[7][nThr];
TH2D *h201AW1000[7][nThr];
TH2D *h201AW800[7][nThr];
TH2D *h201AW600[7][nThr];
TH2D *h201AW400[7][nThr];
TH2D *h201AW350[7][nThr];
TH2D *h201AW250[7][nThr];
TH2D* h301[7][nThr];
TCanvas* suncanvas[7][nThr];
TCanvas* AWComp[7][20];
      for(int j=0; j<nThr; j++){
	 Avtime[j] = new TCanvas();
      	
         h201A0[j] = new TH1D(Form("h201A0_%d",j), "SiPM ; [CH 0] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A0HIT[j] = new TH1D(Form("h201A0HIT_%d",j), "SiPM ; [CH 0] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A0MISS[j] = new TH1D(Form("h201A0MISS_%d",j), "SiPM ; [CH 0] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
          h201A1[j] = new TH1D(Form("h201A1_%d",j), "SiPM ; [CH 1] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A1HIT[j] = new TH1D(Form("h201A1HIT_%d",j), "SiPM ; [CH 1] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A1MISS[j] = new TH1D(Form("h201A1MISS_%d",j), "SiPM ; [CH 1] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A3[j] = new TH1D(Form("h201A3_%d",j), "SiPM ; [CH 3] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A3HIT[j] = new TH1D(Form("h201A3HIT_%d",j), "SiPM ; [CH 3] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A3MISS[j] = new TH1D(Form("h201A3MISS_%d",j), "SiPM ; [CH 3] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A4[j] = new TH1D(Form("h201A4_%d",j), "SiPM ; [CH 0] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A4HIT[j] = new TH1D(Form("h201A4HIT_%d",j), "SiPM ; [CH 4] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A4MISS[j] = new TH1D(Form("h201A4MISS_%d",j), "SiPM ; [CH 4] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
 	 h201A2[j]=  new TH1D(Form("h201A2_%d",j), "SiPM ; [CH 2] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A01[j] = new TH1D(Form("h201A01_%d",j), "SiPM ; Average [01] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A01HIT[j] = new TH1D(Form("h201A01HIT_%d",j), "SiPM ; Average [01] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A01MISS[j] = new TH1D(Form("h201A01MISS_%d",j), "SiPM ; Average [01] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20); 
         h201A012[j] = new TH1D(Form("h201A012_%d",j), "SiPM ; Average [012] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A012HIT[j] = new TH1D(Form("h201A012HIT_%d",j), "SiPM ; Average [012] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A012MISS[j] = new TH1D(Form("h201A012MISS_%d",j), "SiPM ; Average [012] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A0123[j] = new TH1D(Form("h201A0123_%d",j),"SiPM ; Average [0123] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A0123HIT[j] = new TH1D(Form("h201A0123HIT_%d",j),"SiPM ; Average [0123] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A0123MISS[j] = new TH1D(Form("h201A0123MISS_%d",j),"SiPM ; Average [0123] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20); 
	 h201A0V4[j]  = new TH2D(Form("h201A0V4_%d",j),"SiPM ; [CH 0] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns) ;[CH 4] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns); events", 80, -1, 1, 80, -1, 1);
	 h201A0V4HIT[j]  = new TH2D(Form("h201A0V4HIT_%d",j),"SiPM ; [CH 0] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns) ;[CH 4] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns); events", 80, -1, 1, 80, -1,1);
	 h201A0V4HIT2[j]  = new TH2D(Form("h201A0V4HIT2_%d",j),"SiPM ; [CH 0] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns) ;[CH 4] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns); events", 80, -1, 1, 80, -1, 1);
	 h201A45[j] = new TH1D(Form("h201A45_%d",j), "SiPM  ; Average [45] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A45HIT[j] = new TH1D(Form("h201A45HIT_%d",j), "SiPM  ; Average [45] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A45MISS[j] = new TH1D(Form("h201A45MISS_%d",j), "SiPM  ; Average [45] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A456[j] = new TH1D(Form("h201A456_%d",j),"SiPM ;  Average [456] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
         h201A456HIT[j] = new TH1D(Form("h201A456HIT_%d",j),"SiPM ;  Average [456] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201A456MISS[j] = new TH1D(Form("h201A456MISS_%d",j),"SiPM ;  Average [456] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201AFVB[j] = new TH2D(Form("h201AFVB_%d",j),"SiPM ; Average [Front] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns) ;Average [Back] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns); events", 80, -1, 1, 80, -1, 1);
	h201AFVBHIT[j] = new TH2D(Form("h201AFVBHIT_%d",j),"SiPM ; Average [Front] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns) ;Average [Back] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns); events", 80, -1, 1, 80, -1, 1);
	//h201AFVB[j] = new TH2D(Form("h201AFVB_%d",j),"SiPM ; Average [Front] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns) ;Average [Back] T_{Thr} - T_{MCP} - F(1 / A'_{max}) (ns); events", 80, -1, 1, 80, -1, 1);
	 h201Atotal[j] = new TH1D(Form("h201Atotal_%d",j),"SiPM ;  Average [All] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201AtotalHIT[j] = new TH1D(Form("h201AtotalHIT_%d",j),"SiPM ;  Average [All] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	 h201AtotalMISS[j] = new TH1D(Form("h201AtotalMISS_%d",j),"SiPM ;  Average [All] (T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns) ; events",1000,-5,20);
	   for(int i=0; i<7; i++){
               h201[i][j] = new TH2D(Form("h201_%d_%d",i,j), "SiPM ; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP}  (ns); events", 100,.01, 0.21, 1000,-5,20);//fix my range
		h201AW[i][j] = new TH2D(Form("h201AW_%d_%d",i,j), "SiPM ; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); events", 100,0.01, 0.21, 1000,-5,20);
      	        h201AW1000[i][j] = new TH2D(Form("h201AW1000_%d_%d",i,j), "SiPM 1k Bins ; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); events", 100,0.01, 0.21, 1000,-5,20);
		h201AW800[i][j] = new TH2D(Form("h201AW800_%d_%d",i,j), "SiPM 800 Bins ; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); events", 100,0.01, 0.21, 800,-5,20);
		h201AW600[i][j] = new TH2D(Form("h201AW600_%d_%d",i,j), "SiPM 600 Bins; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); events", 100,0.01, 0.21, 600,-5,20);
                h201AW400[i][j] = new TH2D(Form("h201AW400_%d_%d",i,j), "SiPM 400 Bins; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); events", 100,0.01, 0.21, 400,-5,20);
		h201AW350[i][j] = new TH2D(Form("h201AW350_%d_%d",i,j), "SiPM 350 Bins; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); events", 100,0.01, 0.21, 350,-5,20);
                h201AW250[i][j] = new TH2D(Form("h201AW250_%d_%d",i,j), "SiPM 250 Bins; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP} - F(1 / A'_{max})  (ns); events", 100,0.01, 0.21, 250,-5,20);	      
 h301[i][j] = new TH2D(Form("h301_%d_%d",i,j), "SiPM Peak Removed; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP}  (ns); events", 100, .01, 0.21, 1000, -5, 20);//This is for removing the peaks/"Sun"
	       AWComp[i][j] = new TCanvas(Form("Before and After AWC Ch%d,Thr%d",i,j),"Before and After AWC",1600,1600);
}
  }

//Gnote: TGraph for Singlepulses
TCanvas* singpul = new TCanvas();
TGraph* singpulT = new TGraph();
TGraph* timplots[7][7][2];
TCanvas* byamp[7][2]={ new TCanvas("Timing Resolution by Amp", "Timing Resolution by Amp",600,800)};
TCanvas* bych[7][2]={ new TCanvas("Timing Resolution by Channel", "Timing Resolution by Channel",600,800)};
TCanvas* timefuncplot = new TCanvas();
TGraph* meanvsamp = new TGraph(); 
double window = 2.0;
TF1* fitfunc;
TF1* timefunc;//good place for this?

  const int ngrWFMax = 10;
  TGraph *grWF[8][ngrWFMax];
  TGraph *grIF[7][ngrWFMax];
  int ngrWF = 0;
  
  Long64_t nentries = fChain->GetEntries();
  int nb, nbytes;
  int pul1 = 0;
  int pul2 = 0; 
  for (Long64_t jentry=0; jentry<nentries;jentry++) {//Gnote: this is where the loop over all events begins
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (jentry % 10000 == 0) cout << "Processing Event " << jentry << " of " << nentries << "\n";
      nb = fChain->GetEntry(jentry);   nbytes += nb;
//TGraph* amicrazy = new TGraph();
//amicrazy->SetName("crazy");
 if (jentry < 50){
for (int it= -100; it <-40; it++){
amicrazy->SetPoint(it+100,it,channel[0][it]);
}
trigger->cd();
amicrazy->SetTitle("PWOp Channel 0");
amicrazy->GetXaxis()->SetTitle("t_{i} (ns)");
amicrazy->GetYaxis()->SetTitle("amplitude (mV)");
amicrazy->Draw();

//trigger->Print(Form("triggers/trigger_%d.png",jentry));
} 
      bool isOK = true; // is it a good event?

      // ---------------------------
      //           Tracks
      // ---------------------------
       
      
      if(isOK){
        h001->Fill( ntracks );
      }
      isOK = isOK &&  ntracks == 1;//OG is 1

      if(isOK){
          h001b->Fill(xIntercept);
      }
      isOK = isOK && !(fabs(xIntercept)<1e-9 && fabs(yIntercept)<1e-9 && fabs(xSlope)<1e-9 && fabs(ySlope) < 1e-9);

      // calculate position of the track at Z=1.5m      
      double xtrk = xIntercept + 1560. * xSlope;
      double ytrk = yIntercept + 1560. * ySlope;
     

 double tmp2[7];

for(int ich = 0 ; ich < 7 ; ich++){//Seg?
tmp2[ich]= sqrt( pow(xtrk - x0center[ich],2) + pow(ytrk - y0center[ich],2) ); 
//std::cout<<"Tehee the tmp2 of Chan: "<< ich<<" is: "<<tmp2[ich]<<std::endl;
     }

	 if(isOK){
        h002->Fill( xIntercept, yIntercept );
        h002b->Fill( xtrk, ytrk );
        h003->Fill( xSlope, ySlope );
	h003a->Fill(xSlope);
        h003b->Fill(ySlope);
        h004->Fill( xIntercept, yIntercept, nplanes );
      }
      

      // ------------------------------
      //        MCP 
      // ------------------------------
      
      if(isOK){
        h005->Fill(baseline_RMS[7]);
      }
      isOK = isOK && baseline_RMS[7] < 2.0;
      
      if(isOK){
          h006->Fill( amp[7] );
          if(amp[7] < 500){
            h007a->Fill( xIntercept, yIntercept, amp[7] );
            h007b->Fill( xtrk, ytrk, amp[7] );
          }
      }
      isOK = isOK && amp[7] > 100. && amp[7] < 500.;

      if(isOK){
          h008a->Fill( (LP2_30mV[7] - LP2_50[7])*1e+9, (gaus_mean[7] - LP2_50[7])*1e+9 );
          h008c->Fill( (LP2_30mV[7] - LP2_50[7])*1e+9, (gaus_mean[7] - LP2_50[7])*1e+9 );
      }
      
      // MCP reconstruction

      double tMCP_mean = -1e+9;
      double tMCP_50   = -1e+9;
      double tMCP_30mV = -1e+9;
      double aMCP      = 0;
      if(isOK){
        int imax = 0;
        int amax = 0;
        for(int it=10; it<490; it++){
            double a = -1. * channel[7][it];
            if( amax < a ){
                imax = it;
                amax = a;
            }
        }
        if(imax>0){
            TGraphErrors *grMCP = new TGraphErrors();
            int npMCP = 0;
            for(int ip=imax-2; ip<=imax+1; ip++){
                grMCP->SetPoint( npMCP, times[0][ip],  -1. * channel[7][ip]);
                grMCP->SetPointError( npMCP, 0.5 * (times[0][ip] - times[0][ip-1]), baseline_RMS[7]  );
                npMCP++;
            }
            TF1 *fMCP = new TF1("fMCP","gaus",times[0][imax-2]-0.01e-9,times[0][imax+1]+0.01e-9);
            fMCP->SetParameters(amp[7], times[0][imax], times[0][imax]-times[0][imax-1]);
            fMCP->SetParLimits(0, 50, 650);
            fMCP->SetParLimits(1, times[0][imax-1], times[0][imax+1]);
            fMCP->FixParameter(2, 2.17e-10); // fix width to typical value for this MCP
            grMCP->Fit("fMCP","QWB");
            tMCP_mean = fMCP->GetParameter(1);
            aMCP = fMCP->Eval(tMCP_mean);
            double thr = -0.5 * aMCP;
            for(int ip=imax; ip>5; ip--){
                if(channel[7][ip] < thr && channel[7][ip-1] >= thr){
                    tMCP_50 = times[0][ip-1] + (times[0][ip] - times[0][ip-1]) * (channel[7][ip-1] - thr)/(channel[7][ip-1] - channel[7][ip]);
                    break;
                }
            }
            thr = -30.0;
            for(int ip=imax; ip>5; ip--){
                if(channel[7][ip] < thr && channel[7][ip-1] >= thr){
                    tMCP_30mV = times[0][ip-1] + (times[0][ip] - times[0][ip-1]) * (channel[7][ip-1] - thr)/(channel[7][ip-1] - channel[7][ip]);
                    break;
                }
            }
            delete fMCP;
            delete grMCP;
      // Takes up too much memory/time 		grMCP->Write();
        }
      }//isOK if statement close (below MCP Recon)
      if(isOK){
          h008b->Fill( (tMCP_30mV - tMCP_50)*1e+9, (tMCP_mean - tMCP_50)*1e+9 );
          h008d->Fill( (tMCP_30mV - tMCP_50)*1e+9, (tMCP_mean - tMCP_50)*1e+9 );
          h009a->Fill( aMCP);
          h009b->Fill( tMCP_mean );
      }
      isOK = isOK && tMCP_mean > -85e-9 && tMCP_mean < -65e-9;
      
      if(isOK){
          h009c->Fill( aMCP);
      }
      
      
      // ------------------------
      //        SiPMs 
      // ------------------------
      
      if(isOK){
          for(int ich=0; ich<7; ich++){
            h101[ich]->Fill(baseline_RMS[ich]);
          }
      }
      for(int i=0; i<7; i++){
          isOK = isOK && baseline_RMS[i] < 6.0;
      }
      
      if(isOK){
          for(int ich=0; ich<7; ich++){
              if(amp[ich]>1e-3){
                h102[ich]->Fill(log10(amp[ich]));
              }
          }
      }
      
      // find good range for integration of WF
      if(isOK){
          for(int ich=0; ich<7; ich++){
              double logA = log10(amp[ich]);
              for(int it=0; it<500; it++){
                  double x = (times[0][it] - tMCP_mean) * 1e+9;
                  double z = -1. * channel[ich][it] / amp[ich];
                  h103[ich]->Fill( x, logA, z);
              }
          }
      }
           
      // calculate integral of WFs     
      double intA[7];
      for(int ich=0; ich<7; ich++){
	intA[ich] = -999;
          if(isOK){
              intA[ich] = 0;
              for(int it=1; it<500; it++){
                  double x = times[0][it] - tMCP_mean;
                  double z = -1. * channel[ich][it];
                  if(x > -5e-9 && x< +20e-9){
                      intA[ich] += -1. * channel[ich][it] * (times[0][it] - times[0][it-1]) * 1e+9;
                  }
              }
          }
      }
      
      // integral vs amplitude
      if(isOK){
          for(int ich=0; ich<7; ich++){
              if(intA[ich]>1e-2){
                  double logI = log10(intA[ich]);
                  for(int it=0; it<500; it++){
                    double x = (times[0][it] - tMCP_mean) * 1e+9;
                    double z = -1. * channel[ich][it];
                    h104[ich]->Fill( x, logI, z);
                 }
              }
          }
      }
            
      double ampI[7];
      for(int i=0; i<7; i++){
          ampI[i] = intA[i] * int2amp[i];
      }
      
      if(isOK){
          for(int ich=0; ich<7; ich++){
	       if (ampI[ich] > 10){
              h105[ich]->Fill( ampI[ich] );
		}
	      h105C->Fill( log10(amp[ich]));//An extra 1D Histo of all events from all SiPMs
              if(ampI[ich] < 60){
                h106[ich]->Fill( xtrk, ytrk, ampI[ich] );
              }
          }
      }           
      
//std::cout<<"heyo"<<std::endl;
 if(isOK){
	

//	if (jentry >10){//edit later
	//for(int it =0; it < 500; it++){
//		std::cout<<times[0][499]-times[0][0]<<std::endl;
//}
//}//Most Recent SDL analysis [August 2024]: Goal was simply to devine the MPV peak of the event amplitude histograms to grab events around half-max width the MPV for an average pulse
		for (int ich = 0 ; ich <7; ich++){
		double MPVpeak=h105[ich]->GetMaximumBin();
		double MPValue = GetBinContent(MPVpeak);
		double MPVhalf = MPValue/2.0;
		int right_bin = MPVpeak;
		int left_bin = MPVpeak;
		while (left_bin > 0 && h105[ich]->GetBinContent(left_bin) > MPVhalf){
			left_bin--;
		}
		while (right_bin <= h105[ich]->GetNbinsX() && h105[ich]->GetBinContent(right_bin) < MPVhalf){
			right_bin++;
		}
		
		double left_value = h105[ich]->GetBinLowEdge(left_bin);
		double right_value = h105[ich]->GetBinLowEdge(right_bin);
		if (ampI[ich] < right_value && ampI[ich] > left_value){	
			
		//	for (int del = 0; del < 10; del++){
//				avgSDL[ich][del] = new TProfile();
//				avgSDL[ich][del]->SetTitle(Form("Average SDL Ch%d del%d_ns",ich,Delays[del]));
			//	if(ampI[ich] > 100 ){
				//	TCanvas* singlec1 = new TCanvas();
				//	TCanvas* singleSDLc1 =new TCanvas();
					for (int it=0;it<500;it++){
	 					double x = (times[0][it]-tMCP_mean) * 1e+9;
         					double z = -1. * channel[ich][it];
	 					singlepulse->SetPoint(it,x,z);
						AvgPulseAug[ich]->SetPoint(it,x,z/MPVpeak);	//normalized
					//	if(it >= Delays[del]){
         							SDLpulse[ich]->SetPoint(it,x, (-1*(channel[ich][it]-channel[ich][it-(Delays[0])]))/MPVpeak);//SDL Pulse normalized
								avgSDL[ich][0]->Fill(x, -1*(channel[ich][it]-channel[ich][it-(Delays[0])]));
						//	}
					}
				//	singlec1->cd();
				//	singlepulse->Draw();
					//singlec1->SetTitle(Form("SP%d",jentry));
				//	singlec1->Write();
					//singlec1->SaveAs(Form("SDL/SinglePulse_num%d_Ch%d.png",jentry,ich));
				//	singleSDLc1->cd();
				//	SDLpulse->Draw();	
				//	singleSDLc1->SaveAs(Form("SDL/SDLPulse_num%d_Ch%d_Del%d.png",jentry,ich,del));
				//	singlepulse->Clear();
				//	SDLpulse->Clear();
			//	}
//			}
		}	 
SDLpulse[ich]->Write();
AvgPulseAug[ich]->Write();
}
//	}
//std::cout<<"hey"<<std::endl;
if (jentry == 281){// && ampI[0] > 10){
TGraph* mippulse = new TGraph();
TCanvas* mippulseC = new TCanvas();
for (int it=0;it<500;it++){
    double x = (times[0][it]-tMCP_mean) * 1e+9;
    double z = channel[0][it];
    mippulse->SetPoint(it,x,z);
}
mippulseC->cd();
mippulse->GetXaxis()->SetTitle("t_{i}-t_{MCP} (ns)");
mippulse->GetYaxis()->SetTitle("Amplitude (mV)");
mippulse->SetTitle("PWOp Sample Waveform (Channel 0)");
mippulse->Draw();
//mippulseC->SaveAs(Form("../ForBob/MipPulse%d.C",jentry));
delete mippulseC;
delete mippulse;
}



          for(int ich=0; ich<7; ich++){
              double tmp = sqrt( pow(xtrk - x0center[ich],2) + pow(ytrk - y0center[ich],2) );
              //if(tmp < dR[0]){
                  h111[ich]->Fill( ampI[ich] );
                  if(ampI[ich] < 80){
                    for(int it=0; it<500; it++){
                        double x = (times[0][it]-tMCP_mean) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h121[ich]->Fill( x, z);
			h221[ich]->Fill(ampI[ich]);//Fill the CM Histograms 
                  //  }
                  }

              }else if(tmp < dR[1]){
                h112[ich]->Fill( ampI[ich] );
               if(ampI[ich] < 80){
               for(int it=0; it<500; it++){
                 double x = (times[0][it]) * 1e+9;
                 double z = -1. * channel[ich][it];
                h122[ich]->Fill( x, z);
		h222[ich]->Fill(ampI[ich]);
                    }
                }
           }else if(tmp < dR[2]){
              h113[ich]->Fill( ampI[ich] );
             if(ampI[ich] < 80){
             for(int it=0; it<500; it++){
             double x = (times[0][it]) * 1e+9;
             double z = -1. * channel[ich][it];
              h123[ich]->Fill( x, z);
			h223[ich]->Fill(ampI[ich]);
                    }
                 }
           }else if(tmp < dR[3]){
              h114[ich]->Fill( ampI[ich] );
             if(ampI[ich] < 80){
              for(int it=0; it<500; it++){
             double x = (times[0][it]) * 1e+9;
                double z = -1. * channel[ich][it];
                h124[ich]->Fill( x, z);
		h224[ich]->Fill(ampI[ich]);
                  }
                }
             }else{ 
                  h115[ich]->Fill( ampI[ich] );
                  if(ampI[ich] < 80){
                    for(int it=0; it<500; it++){
                        double x = (times[0][it]) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h125[ich]->Fill( x, z);
			h225[ich]->Fill(ampI[ich]);
                    }
                  }
             }
          }
      }//Closes isOK if statement wayyy above


      // ----------------------------
      //    SiPM timing
      // ---------------------------
double m = 47.2;
double b = -2.5;
   if(isOK){
	double tsThr[7][nThr];
          for(int ich=0; ich<7; ich++){
              if(ampI[ich] < 5. || ampI[ich] > 2000.) continue;
              
              // make integrated waveform
              std::vector<double> xv;
              std::vector<double> yv;
              double sum = 0;
              for(int it=1; it<500; it++){
                  double x = (times[0][it] - tMCP_mean) * 1e+9;
                  if(x > -5. && x < 20.){
                      xv.push_back(x);
                      yv.push_back(sum);
                      sum += -1. * channel[ich][it] * (times[0][it] - times[0][it-1]) * 1e+9;
                  }//integration range
                  if(x>20) break;
              }//time iteration
              
              //double tsThr[nThr];//Gnote: This is where tsThr is defined. Update: I must expand the dimension of this array to house the information for all channels and thresholds so that I may average the timestamps across multiple channels
              for(int ithr=0; ithr<nThr; ithr++){
                  tsThr[ich][ithr] = -999;
                  if(intThr[ithr] > 0.95 * intA[ich]) continue;
                  for(unsigned int it=xv.size()-1; it>1; it--){
                      if( yv[it-1] <= intThr[ithr] && yv[it] > intThr[ithr] ){
                          tsThr[ich][ithr] = xv[it-1] + (intThr[ithr] - yv[it-1])/(yv[it] - yv[it-1])*(xv[it] - xv[it-1]);
                          break;
                      }
                  }
              }

//CM fills single pulses (original single pulse study from November 2023)
//if (ich == 0 ){
//	//fill single pulse
//for(int it=0; it<500; it++){
//           double x = (times[0][it]-tMCP_mean) * 1e+9;
//           double z = -1. * channel[0][it];
////	singpulT->SetPoint(it,x,z);
//}
////	singpul->cd();
////	singpulT->Draw();
////singpulT->GetXaxis()->SetTitle("t_{i} - t_{MCP} (ns)");
////singpulT->GetYaxis()->SetTitle("average a_{ i} (mV)");
//	//check if the event is above the line
////double cmamp = 1./ampI[0];
////double cmtime = tsThr[10];
////if (tmp < dR[0]){//Cloud conditions go here now (amplitude and deltaT)
//double tmp = sqrt( pow(xtrk - x0center[0],2) + pow(ytrk - y0center[0],2) );
//if (tmp < dR[0] && ampI[0] == 10 && tsThr[10] == 10){
//
////singpulT->SetTitle(Form("PWO_CH0__N2R1_Centerpulse_%d",jentry));
////singpul->Print(Form("trackstudies/PWO_CH0_N1R1_Centerpulse_%d.png",jentry));
//
//}if (tmp < dR[2]&& ampI[0] > 10.5 && ampI[0] < 11 && tsThr[10] > -0.6 && tsThr[10] < -0.1){
////singpulT->SetTitle(Form("PWO_CH0__N2R6_Rightpulse_%d",jentry));
////singpul->Print(Form("trackstudies/PWO_CH0_N2R6_Rightpulse_%d.png",jentry));
//
//}if (tmp > dR[2] && ampI[0] > 5 && ampI[0] < 5.3  && tsThr[10] > 3.2 && tsThr[10] < 3.7 ){
////singpulT->SetTitle(Form("PWO_CH0__N2R7_Rightpulse_%d",jentry));
////singpul->Print(Form("trackstudies/PWO_CH0_N2R7_Rightpulse_%d.png",jentry));
//
//}if ( tmp < dR[2] && ampI[0] < 15 && ampI[0] < 10 && tsThr[10] < -0.6 && tsThr[10] > -1){
////singpulT->SetTitle(Form("PWO_CH0__N2R6_Centerpulse_%d",jentry));
////singpul->Print(Form("trackstudies/PWO_CH0_N2R6_Centerpulse_%d.png",jentry));
//
//}if(tmp > dR[2] && ampI[0] < 7 && ampI[0] > 6.6 && tsThr[10] > 2.4 && tsThr[10] < 2.8){
////singpulT->SetTitle(Form("PWO_CH0__N2R7_Centerpulse_%d",jentry));
////singpul->Print(Form("trackstudies/PWO_CH0_N2R7_Centerpulse_%d.png",jentry));
//
//}if (tmp < dR[2] && ampI[0] > 17 && ampI[0] < 17.3 && tsThr[10] > -1.3 && tsThr[10] < -1){
////singpulT->SetTitle(Form("PWO_CH0__N2R6_Leftpulse_%d",jentry));
////singpul->Print(Form("trackstudies/PWO_CH0_N2R6_Leftpulse_%d.png",jentry));
//
//}if (tmp > dR[2] && ampI[0] < 9.5 && ampI[0] > 9 && tsThr[10] > 1.2 && tsThr[10] < 1.5){
////singpulT->SetTitle(Form("PWO_CH0__N2R7_Leftpulse_%d",jentry));
////singpul->Print(Form("trackstudies/PWO_CH0_N2R7_Leftpulse_%d.png",jentry));
//}
////}//else{
////singpulT->SetTitle(Form("BGO_Below_pulse_%d",jentry));
////singpulT->GetXaxis()->SetTitle("t_{i} - t_{MCP} (ns)");
////singpulT->GetYaxis()->SetTitle("average a_{ i} (mV)");
////singpul->Print(Form("pulses/Below/BGO_Below_pulse_%d.png",jentry));
//
//
//
////}
//
//}//Close Single Pulse stuff
////delete singpulT;
////Angular Study 
double delta = sqrt(pow(xIntercept-xtrk,2) +pow(yIntercept- ytrk,2) + pow(1560,2));//Total displacement of the MIP from beam origin to SiPM
double angle = acos(1560/delta);//Angle made between the xy plane and the track of the incoming MIP (likely in radians)
double an[] = {0,15,30,45,60,90};//some random angles to play with
double pi = TMath::Pi();//include the correct library
double conv = pi/180;
double plmi = 10*conv;//just a window of radians (10 degrees)
 if ( sqrt( pow(xtrk - x0center[4],2) + pow(ytrk - y0center[4],2)) < sqrt(18) ){
//h202a->Fill(angle/conv);
}if ( sqrt( pow(xtrk - x0center[5],2) + pow(ytrk - y0center[5],2)) < sqrt(18) ){
//h202b->Fill(angle/conv);
}if ( sqrt( pow(xtrk - x0center[6],2) + pow(ytrk - y0center[6],2)) < sqrt(18) ){
//h202c->Fill(angle/conv);
}if ( sqrt( pow(xtrk - x0center[3],2) + pow(ytrk - y0center[3],2)) < sqrt(18) ){
//h202d->Fill(angle/conv);
}


 if (ich == 0 ){
for (int ithr = 0; ithr < nThr; ithr++){ 
int ampbin = fitfunc2[ich][ithr]->GetXaxis()->FindBin(1./ampI[ich]);
double comp = fitfunc2[ich][ithr]->GetBinContent(ampbin);
//double cmamp = 1./ampI[0];
//double cmtime = tsThr[11];
double tmp = sqrt( pow(xtrk - x0center[0],2) + pow(ytrk - y0center[0],2) );
double delta = sqrt(pow(xIntercept-xtrk,2) +pow(yIntercept- ytrk,2) + pow(1560,2));//Total displacement of the MIP from beam origin to SiPM
double angle = acos(1560/delta);//Angle made between the xy plane and the track of the incoming MIP (likely in radians)
double an[] = {0,15,30,45,60,90};//some random angles to play with
double pi = TMath::Pi();//include the correct library
double conv = pi/180;
double plmi = 10*conv;//just a window of radians (10 degrees)
//h202->Fill(angle);
//I will have several regions of interest:
//a broad set of (degrees): 90-60 , 60-30, 30-0
//A more narrow set of each angle of angarr plus or minus 10 degrees
//cout<<"X intercept:  "<<xIntercept<<"x track:  "<<xtrk<<endl;
//cout<<"x slope:  "<<xSlope<<"plusminus check is:"<<plmi<<endl;
//cout<<"The angle is:  "<<angle<<endl;
//cout<<"The Delta is:  "<<delta<<endl;
if ( tmp < dR[0] ){//Ring 1

        if(tsThr[ich][ithr] > -5){
           h201R[0][ithr]->Fill(1./ampI[ich],tsThr[ich][ithr]-comp );
        }

}if (tmp > dR[0] && tmp < dR[1] ){//Ring 2

        if(tsThr[ich][ithr] > -5){
           h201R[1][ithr]->Fill(1./ampI[ich],tsThr[ich][ithr]-comp );
                }


}if ( tmp > dR[1] && tmp < dR[2] ){//Ring 3

	if(tsThr[ich][ithr] > -5){
	h201R[2][ithr]->Fill(1./ampI[ich],tsThr[ich][ithr]-comp );
		}	 

}if(tmp > dR[2] && tmp < dR[3] ){//Ring 4
 if(tsThr[ich][ithr] > -5){
        h201R[3][ithr]->Fill(1./ampI[ich],tsThr[ich][ithr]-comp );
                }


}if (tmp > dR[3]){//Ring 5
 if(tsThr[ich][ithr] > -5){
       h201R[4][ithr]->Fill(1./ampI[ich],tsThr[ich][ithr]-comp );
                }

}if (tmp < 8){
 if(tsThr[ich][ithr] > -5){//R < 8 mm
        h201R[5][ithr]->Fill(1./ampI[ich],tsThr[ich][ithr]-comp );
                }

}if (tmp > 8){//R > 8 mm
 if(tsThr[ich][ithr] > -5){
        h201R[6][ithr]->Fill(1./ampI[ich],tsThr[ich][ithr]-comp );
                }

}
}
}
double tmp = sqrt( pow(xtrk - x0center[ich],2) + pow(ytrk - y0center[ich],2) ); 
for(int ithr=0; ithr<nThr; ithr++){
       
	  if(tsThr[ich][ithr] > -5 ){ //&& tmp > dR[2])CM bounds the radius on h201 to explore strange fit behavior 
        int ampbin = fitfunc2[ich][ithr]->GetXaxis()->FindBin(1./ampI[ich]); 
	double comp = fitfunc2[ich][ithr]->GetBinContent(ampbin);
	 h201[ich][ithr]->Fill( 1./ ampI[ich], tsThr[ich][ithr] );//h201 get's filled on an event by event basis!
         h201AW[ich][ithr]->Fill(1./ampI[ich], tsThr[ich][ithr] - comp);
	h201AW1000[ich][ithr]->Fill(1./ampI[ich], tsThr[ich][ithr] - comp);//This should correct for Amplitude walk
	h201AW800[ich][ithr]->Fill(1./ampI[ich], tsThr[ich][ithr] - comp);
	h201AW600[ich][ithr]->Fill(1./ampI[ich], tsThr[ich][ithr] - comp);
	h201AW400[ich][ithr]->Fill(1./ampI[ich], tsThr[ich][ithr] - comp);
	h201AW350[ich][ithr]->Fill(1./ampI[ich], tsThr[ich][ithr] - comp);
	h201AW250[ich][ithr]->Fill(1./ampI[ich], tsThr[ich][ithr] - comp);
	 }//ifsThr


}//for ithr

//Gnote: Above we fill h201 (the heatmap we always stare at) And below I fill the average WF and the single event pulses we are currently working on.
//Below is the amp and timing cut for Channel 0 Region 4 in use
//For Channel 4 Central peak:  if( ich == 4 && 1./ampI[4] > 0.0600  && 1./ampI[4] < 0.0625 && tsThr[10] > 0.25 && tsThr[10] < 0.50 && pul < 10 )(dont forget bracket here)
				//BGO Region 7....Run this twice, the first for samples of Waveforms, the second without the pul requirment to make an average
				if( ich == 2 && 1./ampI[2] > 0.0150  && 1./ampI[2] < 0.0175 && tsThr[2][0] > -2 && tsThr[2][0] < -1.75 && pul1 < 15 ){//pul just ensures we sample 10 events
					for(int it=0; it <500; it++){
					
					double x = (times[2][it]-tMCP_mean)*1e+9;//be sure to change the time and channel info for the relevant channel
					double z = -1*channel[2][it];
					h126->Fill(x,z);
					singpulT->SetPoint(it,x,z);
			//		if (z > maxZ){
			//		 maxZ = z;
			//		std::cout<<"maxZ is: "<< maxZ<<std::endl;
			//		}
				
				}
					singpulT->SetTitle(Form("BGO Channel 0: R7 Single Pulse_%d",jentry));
					singpulT->GetXaxis()->SetTitle("t_{i} - t_{MCP} (ns)");
                                        singpulT->GetYaxis()->SetTitle("average a_{ i} (mV)");
					singpul->cd();
					singpulT->Draw();
		//			singpul->Print(Form("plots/BGO/singlepulses/BGOparFilter_Ch0_Thr10_Region7_event%d.png",jentry));
					singpulT->Set(0);//Cleans the TGraph
		//			pul1++;
				} 
			        
			//BGO Region 8
			if( ich == 0 && 1./ampI[0] > 0.09  && 1./ampI[0] < 0.0925 && tsThr[ich][10] > -1 && tsThr[ich][10] < -0.75 && pul2 < 15 ){//pul just ensures we sample 10 events
					for(int it=0; it <500; it++){
                                        double x = (times[0][it]-tMCP_mean)*1e+9;//be sure to change the time and channel info for the relevant channel
                                        double z = -1*channel[0][it];
                                        h127->Fill(x,z);
                                        singpulT->SetPoint(it,x,z);

			//		if (z > maxZ2){
			//			 maxZ2 = z;
			//		}
                                }
                                        singpulT->SetTitle(Form("BGO Channel 0: R8 Single Pulse_%d",jentry));
                                        singpulT->GetXaxis()->SetTitle("t_{i} - t_{MCP} (ns)");
                                        singpulT->GetYaxis()->SetTitle("average a_{ i} (mV)");
                                        singpul->cd();
                                        singpulT->Draw();
                  //                      singpul->Print(Form("plots/BGO/singlepulses/BGOparFilter_Ch0_Thr10_Region8_event%d.png",jentry));
                                        singpulT->Set(0);//Cleans the TGraph
              //                          pul2++;
                                }
				
				


//Here is where I will utilize the Amplitude Walk Method for timing resolution:
//First, within each nominal amplitude bin, I will find the mean delta T
//Then, plot the mean delta T compared to inverse amplitude and fit some function F
//Then replot h201 bith correction to delta T (delta T - F)
//once again take the projections and find the width just as last time
for (int Ampindex = 0; Ampindex <7; Ampindex++){



   }//Amplitude loop
  }    //chindex

double SiPMsize = sqrt(18);
for(int ithr=0; ithr<nThr; ithr++){
    if(tsThr[0][ithr] > -5 && ampI[0] < 80 ){
        double T0=(tsThr[0][ithr] - fitfunc2[0][ithr]->GetBinContent(fitfunc2[0][ithr]->GetXaxis()->FindBin(1./ampI[0])));
         h201ThVA0->Fill(T0,ithr);
         h201A0[ithr]->Fill(T0);
	     if(tmp2[0] < SiPMsize){
                h201A0HIT[ithr]->Fill(T0);
             }
             if(tmp2[0] > SiPMsize){
                h201A0MISS[ithr]->Fill(T0);
             }

    }

if(tsThr[1][ithr] > -5 && ampI[1] < 80 ){
        double T1=(tsThr[1][ithr] - fitfunc2[1][ithr]->GetBinContent(fitfunc2[1][ithr]->GetXaxis()->FindBin(1./ampI[1])));
      //   h201ThVA0->Fill(T0,ithr);
         h201A1[ithr]->Fill(T1);
             if(tmp2[1] < SiPMsize){
                h201A1HIT[ithr]->Fill(T1);
             }
             if(tmp2[1] > SiPMsize){
                h201A1MISS[ithr]->Fill(T1);
             }

    } 

    if(tsThr[2][ithr] > -5 && ampI[2] < 80 ){
        double T2=(tsThr[2][ithr] - fitfunc2[2][ithr]->GetBinContent(fitfunc2[2][ithr]->GetXaxis()->FindBin(1./ampI[2])));
        h201A2[ithr]->Fill(T2);
         if(tmp2[2] < SiPMsize){
        //        h201A2HIT[ithr]->Fill(T2);
         }
         if(tmp2[2] > SiPMsize){
        //        h201A2MISS[ithr]->Fill(T2);
         }

    }

if(tsThr[3][ithr] > -5 && ampI[3] < 80 ){
        double T3=(tsThr[3][ithr] - fitfunc2[3][ithr]->GetBinContent(fitfunc2[3][ithr]->GetXaxis()->FindBin(1./ampI[3])));
        // h201ThVA0->Fill(T0,ithr);
         h201A3[ithr]->Fill(T3);
             if(tmp2[3] < SiPMsize){
                h201A3HIT[ithr]->Fill(T3);
             }
             if(tmp2[3] > SiPMsize){
                h201A3MISS[ithr]->Fill(T3);
             }

    }

    if(tsThr[4][ithr] > -5 && ampI[4] < 80 ){
        double T4=(tsThr[4][ithr] - fitfunc2[4][ithr]->GetBinContent(fitfunc2[4][ithr]->GetXaxis()->FindBin(1./ampI[4])));
         h201A4[ithr]->Fill(T4);
             if(tmp2[0] < SiPMsize){
                h201A4HIT[ithr]->Fill(T4);
             }
             if(tmp2[0] > SiPMsize){
                h201A4MISS[ithr]->Fill(T4);
             }

    }

    if(tsThr[0][ithr] > -5 && tsThr[4][ithr] > -5 && ampI[0] < 80 && ampI[4] < 80 ){
        double T0=(tsThr[0][ithr] - fitfunc2[0][ithr]->GetBinContent(fitfunc2[0][ithr]->GetXaxis()->FindBin(1./ampI[0])));
        double T4=(tsThr[4][ithr] - fitfunc2[4][ithr]->GetBinContent(fitfunc2[4][ithr]->GetXaxis()->FindBin(1./ampI[4]))); 
         h201A0V4[ithr]->Fill(T0,T4);
             if(tmp2[0] < SiPMsize || tmp2[4] < SiPMsize){
                h201A0V4HIT[ithr]->Fill(T0,T4);
             }
             if(tmp2[0] < SiPMsize && tmp2[4] < SiPMsize){
                h201A0V4HIT2[ithr]->Fill(T0,T4);
             }

    } 
    
    if(tsThr[0][ithr] > -5 && tsThr[1][ithr] > -5 && ampI[0] < 80 && ampI[1] < 80 ){
        //std::cout<<"Test 1.5"<<std::endl;
	double T0=(tsThr[0][ithr] - fitfunc2[0][ithr]->GetBinContent(fitfunc2[0][ithr]->GetXaxis()->FindBin(1./ampI[0])));
        double T1=(tsThr[1][ithr] - fitfunc2[1][ithr]->GetBinContent(fitfunc2[1][ithr]->GetXaxis()->FindBin(1./ampI[1])));
        double A01 = (T0+T1)/2;	
	h201A01[ithr]->Fill(A01);
      //  h201ThVA0->Fill(T0,ithr);
        h201ThVA01->Fill(A01,ithr);
	    if(tmp2[0] < SiPMsize || tmp2[1] < SiPMsize){
	        h201A01HIT[ithr]->Fill(A01);
	    }
	    if(tmp2[0] > SiPMsize && tmp2[1] > SiPMsize){
	        h201A01MISS[ithr]->Fill(A01);
	    }
    }
	
    if(tsThr[0][ithr] > -5 && tsThr[1][ithr] > -5 && tsThr[2][ithr] > -5 && ampI[0] < 80 && ampI[1] < 80 && ampI[2] < 80){
	  
        double T0=(tsThr[0][ithr] - fitfunc2[0][ithr]->GetBinContent(fitfunc2[0][ithr]->GetXaxis()->FindBin(1./ampI[0])));
        double T1=(tsThr[1][ithr] - fitfunc2[1][ithr]->GetBinContent(fitfunc2[1][ithr]->GetXaxis()->FindBin(1./ampI[1])));
	double T2=(tsThr[2][ithr] - fitfunc2[2][ithr]->GetBinContent(fitfunc2[2][ithr]->GetXaxis()->FindBin(1./ampI[2])));
	double A012 = (T0+T1+T2)/3;
	   h201A2[ithr]->Fill(T2); 
	h201A012[ithr]->Fill(A012);
	h201ThVA012->Fill(A012,ithr);
	    
            if(tmp2[0] < SiPMsize || tmp2[1] < SiPMsize || tmp2[2] < SiPMsize){
	        h201A012HIT[ithr]->Fill(A012);
            }
            if(tmp2[0] > SiPMsize && tmp2[1] > SiPMsize && tmp2[2] > SiPMsize){
	        h201A012MISS[ithr]->Fill(A012);
            }
    }
//std::cout<<"Test 4"<<std::endl;
    if(tsThr[0][ithr] > -5 && tsThr[1][ithr] > -5 && tsThr[2][ithr] > -5 && tsThr[3][ithr] > -5 && ampI[0] < 80 && ampI[1] < 80 && ampI[2] < 80 && ampI[3] < 80){
        
	double T0=(tsThr[0][ithr] - fitfunc2[0][ithr]->GetBinContent(fitfunc2[0][ithr]->GetXaxis()->FindBin(1./ampI[0])));
        double T1=(tsThr[1][ithr] - fitfunc2[1][ithr]->GetBinContent(fitfunc2[1][ithr]->GetXaxis()->FindBin(1./ampI[1])));
        double T2=(tsThr[2][ithr] - fitfunc2[2][ithr]->GetBinContent(fitfunc2[2][ithr]->GetXaxis()->FindBin(1./ampI[2])));
	double T3=(tsThr[3][ithr] - fitfunc2[3][ithr]->GetBinContent(fitfunc2[3][ithr]->GetXaxis()->FindBin(1./ampI[3])));
	double A0123 = (T0+T1+T2+T3)/4;
	
	h201A0123[ithr]->Fill(A0123);
	h201ThVA0123->Fill(A0123,ithr);
        
	    if(tmp2[0] < SiPMsize || tmp2[1] < SiPMsize || tmp2[2] < SiPMsize || tmp2[3] < SiPMsize){
	        h201A0123HIT[ithr]->Fill(A0123);
            }
            if(tmp2[0] > SiPMsize && tmp2[1] > SiPMsize && tmp2[2] > SiPMsize && tmp2[3] > SiPMsize){
	        h201A0123MISS[ithr]->Fill(A0123);
            }
    }
    
    if(tsThr[4][ithr] > -5 && tsThr[5][ithr] > -5 && ampI[4] < 80 && ampI[5] < 80 ){

        double T4=(tsThr[4][ithr] - fitfunc2[4][ithr]->GetBinContent(fitfunc2[4][ithr]->GetXaxis()->FindBin(1./ampI[4])));
	double T5=(tsThr[5][ithr] - fitfunc2[5][ithr]->GetBinContent(fitfunc2[5][ithr]->GetXaxis()->FindBin(1./ampI[5])));
	double A45 = (T4+T5)/2;
	
	h201A45[ithr]->Fill(A45);
	h201ThVA45->Fill(A45,ithr);
	
	    if(tmp2[4] < SiPMsize || tmp2[4] < SiPMsize){
	        h201A45HIT[ithr]->Fill(A45);
            }
            if(tmp2[5] > SiPMsize && tmp2[5] > SiPMsize){
	        h201A45MISS[ithr]->Fill(A45);
            }
    }
//std::cout<<"Test 5"<<std::endl;	

    if(tsThr[4][ithr] > -5 && tsThr[5][ithr] > -5 && tsThr[6][ithr] > -5 & ampI[4] < 80 && ampI[5] < 80 && ampI[6] < 80){

	double T4=(tsThr[4][ithr] - fitfunc2[4][ithr]->GetBinContent(fitfunc2[4][ithr]->GetXaxis()->FindBin(1./ampI[4])));
        double T5=(tsThr[5][ithr] - fitfunc2[5][ithr]->GetBinContent(fitfunc2[5][ithr]->GetXaxis()->FindBin(1./ampI[5])));
	double T6=(tsThr[6][ithr] - fitfunc2[6][ithr]->GetBinContent(fitfunc2[6][ithr]->GetXaxis()->FindBin(1./ampI[6])));
        double A456 = (T4+T5+T6)/3;

	h201A456[ithr]->Fill(A456);
	h201ThVA456->Fill(A456,ithr);

            if(tmp2[4] < SiPMsize || tmp2[5] < SiPMsize || tmp2[6] < SiPMsize){
	        h201A456HIT[ithr]->Fill(A456);
            }
            if(tmp2[4] > SiPMsize && tmp2[5] > SiPMsize && tmp2[6] > SiPMsize){
	        h201A456MISS[ithr]->Fill(A456);
            }
    }
//std::cout<<"Test 6"<<std::endl;
    if(tsThr[0][ithr] > -5 && tsThr[1][ithr] > -5 && tsThr[2][ithr] > -5 && tsThr[3][ithr] > -5 && tsThr[4][ithr] > -5 && tsThr[5][ithr] > -5 && tsThr[6][ithr] > -5 && ampI[0] < 80 && ampI[1] < 80 && ampI[2] < 80 && ampI[3] < 80 && ampI[4] < 80 && ampI[5] < 80 && ampI[6] < 80){
	
        double T0=(tsThr[0][ithr] - fitfunc2[0][ithr]->GetBinContent(fitfunc2[0][ithr]->GetXaxis()->FindBin(1./ampI[0])));
        double T1=(tsThr[1][ithr] - fitfunc2[1][ithr]->GetBinContent(fitfunc2[1][ithr]->GetXaxis()->FindBin(1./ampI[1])));
        double T2=(tsThr[2][ithr] - fitfunc2[2][ithr]->GetBinContent(fitfunc2[2][ithr]->GetXaxis()->FindBin(1./ampI[2])));
        double T3=(tsThr[3][ithr] - fitfunc2[3][ithr]->GetBinContent(fitfunc2[3][ithr]->GetXaxis()->FindBin(1./ampI[3])));
	double T4=(tsThr[4][ithr] - fitfunc2[4][ithr]->GetBinContent(fitfunc2[4][ithr]->GetXaxis()->FindBin(1./ampI[4])));
        double T5=(tsThr[5][ithr] - fitfunc2[5][ithr]->GetBinContent(fitfunc2[5][ithr]->GetXaxis()->FindBin(1./ampI[5])));
        double T6=(tsThr[6][ithr] - fitfunc2[6][ithr]->GetBinContent(fitfunc2[6][ithr]->GetXaxis()->FindBin(1./ampI[6])));
        double A0123 = (T0+T1+T2+T3)/4;
        double A456 = (T4+T5+T6)/3;
        double Atotal=(T0+T1+T2+T3+T4+T5+T6)/7;
  //  std::cout<<"Test 7"<<std::endl;   
	h201AFVB[ithr]->Fill(A456,A0123);
//std::cout<<"Test 8"<<std::endl;
       // h201A0V4[ithr]->Fill(T4,T0);
//std::cout<<"Test 9"<<std::endl;
        h201ThVAFB->Fill(Atotal,ithr);
//std::cout<<"Test 10"<<std::endl;
	h201Atotal[ithr]->Fill(Atotal);
//std::cout<<"Test 11"<<std::endl;
      
            if(tmp2[0] < SiPMsize || tmp2[4] < SiPMsize){
//std::cout<<"Test 12"<<std::endl;
               // h201A0V4HIT[ithr]->Fill(T4,T0);
            }
            if (tmp2[0] < SiPMsize && tmp2[4] < SiPMsize){
//std::cout<<"Test 13"<<std::endl;
	     //   h201A0V4HIT2[ithr]->Fill(T4,T0);
            }
	    if (tmp2[0] < SiPMsize || tmp2[1] < SiPMsize|| tmp2[2] < SiPMsize|| tmp2[3] < SiPMsize|| tmp2[4] < SiPMsize|| tmp2[5] < SiPMsize|| tmp2[6] < SiPMsize){
//std::cout<<"Test 14"<<std::endl;             
   h201AFVBHIT[ithr]->Fill(A456,A0123);
	    }
	    if(tmp2[0] < SiPMsize || tmp2[1] < SiPMsize || tmp2[2] < SiPMsize || tmp2[3] < SiPMsize || tmp2[4] < SiPMsize || tmp2[5] < SiPMsize || tmp2[6] < SiPMsize){
//std::cout<<"Test 15"<<std::endl;	     
   h201AtotalHIT[ithr]->Fill(Atotal);
            }
            if(tmp2[0] > SiPMsize && tmp2[1] > SiPMsize && tmp2[2] > SiPMsize && tmp2[3] > SiPMsize && tmp2[4] > SiPMsize && tmp2[5] > SiPMsize && tmp2[6] > SiPMsize){
//std::cout<<"Test 16"<<std::endl;	     
   h201AtotalMISS[ithr]->Fill(Atotal);
            }
    }
//	double T0=(tsThr[0][ithr] - fitfunc2[0][ithr]->GetBinContent(fitfunc2[0][ithr]->GetXaxis()->FindBin(1./ampI[0])));
//        double T1=(tsThr[1][ithr] - fitfunc2[1][ithr]->GetBinContent(fitfunc2[1][ithr]->GetXaxis()->FindBin(1./ampI[1])));
//        double T2=(tsThr[2][ithr] - fitfunc2[2][ithr]->GetBinContent(fitfunc2[2][ithr]->GetXaxis()->FindBin(1./ampI[2])));
//        double T3=(tsThr[3][ithr] - fitfunc2[3][ithr]->GetBinContent(fitfunc2[3][ithr]->GetXaxis()->FindBin(1./ampI[3])));
//        double T4=(tsThr[4][ithr] - fitfunc2[4][ithr]->GetBinContent(fitfunc2[4][ithr]->GetXaxis()->FindBin(1./ampI[4])));
//        double T5=(tsThr[5][ithr] - fitfunc2[5][ithr]->GetBinContent(fitfunc2[5][ithr]->GetXaxis()->FindBin(1./ampI[5])));
//        double T6=(tsThr[6][ithr] - fitfunc2[6][ithr]->GetBinContent(fitfunc2[6][ithr]->GetXaxis()->FindBin(1./ampI[6])));
//	double A01 = (T0+T1)/2;
//	double A012 = (T0+T1+T2)/3;
//	double A0123 = (T0+T1+T2+T3)/4;
//	double A45 = (T4+T5)/2;
//	double A456 = (T4+T5+T6)/3;
//	h201A01[ithr]->Fill(A01);
//        h201A012[ithr]->Fill(A012);
//	h201A0123[ithr]->Fill(A0123);
//        h201A45[ithr]->Fill(A45);
//        h201A456[ithr]->Fill(A456);
//	h201AFVB[ithr]->Fill(A456,A0123);
//

}//for ithr



 }//isOkay
      bool isInteresting = isOK;
      
//      isInteresting = isInteresting && log10(amp[1])>1.45 && log10(amp[1])<1.50;
      isInteresting = isInteresting && log10(ampI[1])>1 && log10(amp[1])<2.0;
      
      if(isInteresting && ngrWF < ngrWFMax){
          for(int ich=0; ich<8; ich++){
              grWF[ich][ngrWF] = new TGraph();
              for(int it=0; it<500; it++){ // how many samples?
                  double x = (times[0][it] - tMCP_mean) * 1e+9;
//                  grWF[ich][ngrWF]->SetPoint(it, times[0][it], channel[ich][it]);
                  grWF[ich][ngrWF]->SetPoint(it, x, channel[ich][it]);
              }
          }
          for(int ich=0; ich<7; ich++){
              grIF[ich][ngrWF] = new TGraph();
              int np = 0;
              double sum = 0;
              for(int it=0; it<500; it++){
                  double x = (times[0][it] - tMCP_mean) * 1e+9;
                  if(x > -5. && x < 20.){
                      grIF[ich][ngrWF]->SetPoint(np, x, sum );
//                      grIF[ich][ngrWF]->SetPoint(np, x, sum / intA[ich]);
                      sum += -1. * channel[ich][it] * (times[0][it] - times[0][it-1]) * 1e+9;
                      np++;
                  }
              }
          }
          ngrWF++;
      }
      
  }//Gnote: End of Loop through all events     


TFile* SDLfile = new TFile("smallSDLTGraphs.root", "UPDATE");
    if (!SDLfile->IsOpen()) {
        std::cerr << "Error: Could not open output.root file!" << std::endl;
        return;
    }//SDL Stuff
SDLfile->cd();
for (int ich = 0; ich <7; ich++){
for (int del =0 ; del <10; del++){
TGraph* tempgraf = new TGraph();
tempgraf->SetName(Form("SDL_Ch%d_del%d_F",ich,del));
for(int i =1; i <=avgSDL[ich][del]->GetNbinsX();i++){
tempgraf->SetPoint(i-1,avgSDL[ich][del]->GetBinCenter(i),avgSDL[ich][del]->GetBinContent(i));
}
avgSDLc1->cd();
tempgraf->Draw();
tempgraf->Write();
//avgSDLc1->Print(Form("SDL/AvgUnfil/Ch%dDel%d.png",ich,del));
avgSDLc1->Clear();
}
}
SDLfile->Close();















//Here, I take the Bin contents of h201 and transfer them to h301 as long as the maximum is blocked out (sunplots)
for(int ich=0; ich<7 ; ich++){
 for(int ithr=0; ithr<nThr; ithr++){
	int brightspot =h201[ich][ithr]-> GetMaximumBin();
	double brightness =h201[ich][ithr]->GetBinContent(brightspot);
	double trim = brightness-brightness*0.50;

	int maxX = h201[ich][ithr]->GetNbinsX();
	int maxY = h201[ich][ithr]->GetNbinsY();



  AWComp[ich][ithr]->Divide(2,1);
	AWComp[ich][ithr]->cd(1)->SetPad(0.01, 0.01, 0.49, 0.99);
                AWComp[ich][ithr]->cd(1);
                h201[ich][ithr]->Draw("COLZ");
                  AWComp[ich][ithr]->cd(2)->SetPad(0.51, 0.01, 0.99, 0.99);
		AWComp[ich][ithr]->cd(2);
                h201AW[ich][ithr]->Draw("COLZ");
//AWComp[ich][ithr]->Print(Form("plots/corrcompare/BGOpF_AWComp_Ch%d_Thr%d.png",ich,ithr));
TCanvas* AWSep = new TCanvas();
AWSep->cd();
h201[ich][ithr]->Draw("COLZ");
//AWSep->Print(Form("plots/AWCs/BGOpF_Ch%d_Thr%d_OG.png",ich,ithr));
AWSep->Clear();
AWSep->Update();
AWSep->cd();
h201AW[ich][ithr]->Draw("COLZ");
//AWSep->Print(Form("plots/AWCs/BGOpF_Ch%d_Thr%d_PC.png",ich,ithr));





for(int i=1; i <= maxX; i++){
  for(int j =1; j<= maxY; j++){
	double binContent = h201[ich][ithr]->GetBinContent(i,j);
	 if(binContent < trim){

		h301[ich][ithr]->SetBinContent(i,j,binContent);

	}else{
	 	h301[ich][ithr]->SetBinContent(i,j,1.0);
		
}//else
}
}
 }//ithr
  }//ich 
 
//next to paste h201 and 301 together, play with the trim amount, maybe increments of 10, turn into a gif.
for(int ich=0; ich<7 ; ich++){
 for(int ithr=0; ithr<nThr; ithr++){
suncanvas[ich][ithr] = new TCanvas();
suncanvas[ich][ithr]->Divide(2,1);
suncanvas[ich][ithr]->cd(1);
gPad->SetLogz();
//h301[ich][ithr]->SetLogz();
h201[ich][ithr]->Draw("COLZ");
suncanvas[ich][ithr]->cd(2);
gPad->SetLogz();
h301[ich][ithr]->Draw("COLZ");
suncanvas[ich][ithr]->Update();
//suncanvas[ich][ithr]->Print(Form("sunplotsblue50/logzTimingDistribution(un)blocked_Chan%d_Thr%d.png",ich,ithr));
}
}


//Angular Study plotting

//TH1D* proja = (TH1D*)h201a->ProjectionY("Projection",h201a->GetXaxis()->FindBin(1/18),h201a->GetXaxis()->FindBin(1/14));
//  TCanvas* Angulara = new TCanvas();
//  Angulara->Divide(2,1);
//  Angulara->cd(1);
//  h201a->Draw("COLZ");
//  Angulara->cd(2);
//  proja->Draw();
//  //Angulara->Print("Angstudy/PWOChannel3HIT.png");
//
//  TH1D* projb = (TH1D*)h201b->ProjectionY("Projection",h201b->GetXaxis()->FindBin(1/18),h201b->GetXaxis()->FindBin(1/14));
//  TCanvas* Angularb = new TCanvas();
//  Angularb->Divide(2,1);
//  Angularb->cd(1);
//  h201b->Draw("COLZ");
//  Angularb->cd(2);
//  projb->Draw();
// //Angularb->Print("Angstudy/PWOChannel3NOHIT.png");
// 
//TH1D* projc = (TH1D*)h201c->ProjectionY("Projection",h201c->GetXaxis()->FindBin(1/18),h201c->GetXaxis()->FindBin(1/14));
//  TCanvas* Angularc = new TCanvas();
//  Angularc->Divide(2,1);
//  Angularc->cd(1);
//  h201c->Draw("COLZ");
//  Angularc->cd(2);
//  projc->Draw();
//  //Angularc->Print("Angstudy/PWOChannel3NARROW.png");
// 
//TH1D* projd = (TH1D*)h201d->ProjectionY("Projection",h201d->GetXaxis()->FindBin(1/18),h201d->GetXaxis()->FindBin(1/14));
//  TCanvas* Angulard = new TCanvas();
//  Angulard->Divide(2,1);
//  Angulard->cd(1);
//  h201d->Draw("COLZ");
//  Angulard->cd(2);
//  projd->Draw();
//  //Angulard->Print("Angstudy/PWOChannel3WIDE.png");
//
//  TH1D* proje = (TH1D*)h201e->ProjectionY("Projection",h201e->GetXaxis()->FindBin(1/18),h201e->GetXaxis()->FindBin(1/14));
//  TCanvas* Angulare = new TCanvas();
//  Angulare->Divide(2,1);
//  Angulare->cd(1);
//  h201e->Draw("COLZ");
//  Angulare->cd(2);
//  proje->Draw();
////Angulare->Print("Angstudy/PWOChannel3Angle_1.png");
// 
//TH1D* projf = (TH1D*)h201f->ProjectionY("Projection",h201f->GetXaxis()->FindBin(1/18),h201f->GetXaxis()->FindBin(1/14));
//  TCanvas* Angularf = new TCanvas();
//  Angularf->Divide(2,1);
//  Angularf->cd(1);
//  h201f->Draw("COLZ");
//  Angularf->cd(2);
//  projf->Draw();
//// Angularf->Print("Angstudy/PWOChannel3Angle_2.png");
//
//TH1D* projg = (TH1D*)h201g->ProjectionY("Projection",h201g->GetXaxis()->FindBin(1/18),h201g->GetXaxis()->FindBin(1/14));
//  TCanvas* Angularg = new TCanvas();
//  Angularg->Divide(2,1);
//  Angularg->cd(1);
//  h201g->Draw("COLZ");
//  Angularg->cd(2);
//  projg->Draw();
//  //Angularg->Print("Angstudy/PWOChannel3Angle_3.png");
//
  fout->cd();
  fout->Write();
  for(int ig=0; ig<ngrWF; ig++){
      for(int ich=0; ich<8; ich++){
          grWF[ich][ig]->SetName(Form("grWF_%d_%d",ich,ig));
          grWF[ich][ig]->Write();
      }
  }
  for(int ig=0; ig<ngrWF; ig++){
      for(int ich=0; ich<7; ich++){
          grIF[ich][ig]->SetName(Form("grIF_%d_%d",ich,ig));
          grIF[ich][ig]->Write();
      }
  }
 // fout->Close();
//for (int step = 0; step < 500; ++step) {
    double maxZ = h126->Integral(h126->FindBin(-5.0), h126->FindBin(30.0), "width");
    double maxZ2 =  h127->Integral(h126->FindBin(-5.0), h126->FindBin(30.0), "width");
    // Update maxZ if avgValue is greater
  //       if (intValue > maxZ) {
    //             maxZ = intValue;
      //               }
//	if (intValue2 > maxZ2){
//	maxZ2 = intValue2;
//	}                    

// }




h126->SetMarkerColor(kRed);
h126->SetLineColor(kRed);
h127->SetMarkerColor(kGreen);
h127->SetLineColor(kGreen);
TCanvas* avgcanv = new TCanvas("avgcanv","Avg WFs in Each Regime",800,600);
avgcanv->Clear();
avgcanv->cd();
h126->Scale(1.0/maxZ);
h126->GetYaxis()->SetRangeUser(0, 0.1);
h126->Draw();
h127->Scale(1.0/maxZ2);
h127->GetYaxis()->SetRangeUser(0, 0.1);
h127->Draw("SAME");
TLegend* avglegend = new TLegend(0.7,0.7,0.9,0.9);
avglegend->AddEntry(h126, "Region 7", "lep");
avglegend->AddEntry(h127, "Region 8","lep");
avglegend->Draw();
avgcanv->Update();
//avgcanv->Print("plots/BGO/AverageWF/BGOparFilter_Ch0_Thr10_Region7and8.png");
avgcanv->Clear();
h126->Draw();
//avgcanv->Print(Form("plots/BGO/AverageWF/BGOparFilter_Ch0_Thr10_Region7.png"));
avgcanv->Clear();
h127->Draw();
//avgcanv->Print(Form("plots/BGO/AverageWF/BGOparFilter_Ch0_Thr10_Region8.png"));


for (int i = 0; i < nThr; i++){
// Avtime[i]->cd();
//	h201A0[i]->Draw();
//	Avtime[i]->Update();
//        Avtime[i]->Print(Form("plots/Avgtime/MIP/PWOpF/AvgtimeCh0_thr_%d.png",i));
//        Avtime[i]->Clear();
//        h201A0V4[i]->Draw("COLZ");
//        Avtime[i]->Update();
//        Avtime[i]->Print(Form("plots/Avgtime/MIP/PWOpF/Corr0V4_thr_%d.png",i));
//        Avtime[i]->Clear();
//        Avtime[i]->Update();
//        h201A0V4HIT[i]->Draw("COLZ");
//        Avtime[i]->Update();
//        Avtime[i]->Print(Form("plots/Avgtime/MIP/PWOpF/Corr0V4HIT_thr_%d.png",i));
//        Avtime[i]->Clear();
//        Avtime[i]->Update();
//        h201A0V4HIT2[i]->Draw("COLZ");
//        Avtime[i]->Update();
//        Avtime[i]->Print(Form("plots/Avgtime/MIP/PWOpF/Corr0V4HIT2_thr_%d.png",i));
//        Avtime[i]->Clear();
//        Avtime[i]->Update();
//        h201A45[i]->Draw();
//        Avtime[i]->Update();
//        Avtime[i]->Print(Form("plots/Avgtime/MIP/PWOpF/AvgtimeCh45_thr_%d.png",i));
//        Avtime[i]->Clear();
//        Avtime[i]->Update();
//        h201A456[i]->Draw();
//        Avtime[i]->Update();
//        Avtime[i]->Print(Form("plots/Avgtime/MIP/PWOpF/AvgtimeCh456_thr_%d.png",i));
//        Avtime[i]->Clear();
//        Avtime[i]->Update();
//        h201AFVB[i]->Draw("COLZ");
//        Avtime[i]->Update();
//        Avtime[i]->Print(Form("plots/Avgtime/MIP/PWOpF/AvgtimeFVB_thr_%d.png",i));
//        Avtime[i]->Clear();
//        Avtime[i]->Update();
}


fitsFile->Close();
fout->Close();
}



