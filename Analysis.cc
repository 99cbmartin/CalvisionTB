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

  
  double int2amp[] = { 7.31775e-02, 7.31946e-02, 7.34309e-02, 7.30913e-02, 7.79333e-02, 7.41691e-02, 7.59032e-02 };
  int chPair[3][2] = {
      {0, 4},
      {1, 6},
      {3, 5}
  };
  double x0center[] = { -1.0, -10.0, -1.6, -10.3, -2.2, -12.0, -11.4};  
  double y0center[] = { -1.8,  -2.1, +7.3,  +7.1, -1.8,  +6.9,  -2.1};  
  double dR[] = { 2.0, 4.0, 8.0, 12.0 };
    
  const int nThr = 20;
  double intThr[nThr];
  for(int i=0; i<nThr; i++){
      intThr[i] = pow(10, 0.4 + 0.1 * i);
  }
  
  TFile *fout = new TFile("PWOparFilterAvgwfFeb1_Ch4.root","recreate");

  
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
  TH1D *h009a = new TH1D("h009a", "MCP ;aMCP (mV); events", 200, 0, 600);
  TH1D *h009b = new TH1D("h009b", "MCP ;tMCP_mean (s); events", 200, -90e-9, -40e-9);
  TH1D *h009c = new TH1D("h009c", "MCP ;aMCP (mV); events", 200, 0, 600);
  
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
  TProfile *h122[7];
  TProfile *h123[7];
  TProfile *h124[7];
  TProfile *h125[7];
  for(int i=0; i<7; i++){
      h101[i] = new TH1D(Form("h101_%d",i),Form("SiPM ;baseline_RMS[%d] (mV)",i),200, 0, 20);
      h102[i] = new TH1D(Form("h102_%d",i),Form("SiPM ;log10 amp[%d] (mV) ;events",i),200, 0, 4);
      h103[i] = new TProfile2D(Form("h103_%d",i),";t_{ i} - t_{ MCP} (ns) ;log10 A_{ max}  (mV); avg a_{ i} / A_{ max}", 1000, -50, 50, 100, 0, 3.5);
      h104[i] = new TProfile2D(Form("h104_%d",i),";t_{ i} - t_{ MCP} (ns)  ;log I  (mV*ns); avg a_{ i} (mV)", 1000, -10, 40, 100, 0, 5);      
      h105[i] = new TH1D(Form("h105_%d",i),";A'_{max} (mV); events", 200,-20,100);//Was -20-100 x domain
      h106[i] = new TProfile2D(Form("h106_%d",i), ";x_{ trk} (mm);y_{ trk} (mm); avg A'_{ max} (mV)", nbXY, xMin, xMax, nbXY, yMin, yMax);
      h111[i] = new TH1D(Form("h111_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h112[i] = new TH1D(Form("h112_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h113[i] = new TH1D(Form("h113_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h114[i] = new TH1D(Form("h114_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h115[i] = new TH1D(Form("h115_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h121[i] = new TProfile(Form("h121_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      h122[i] = new TProfile(Form("h122_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      h123[i] = new TProfile(Form("h123_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      h124[i] = new TProfile(Form("h124_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
      h125[i] = new TProfile(Form("h125_%d",i), ";t_{i} (ns); average a_{ i} (mV)", 1000, -90, -40);
     
//CM Inserts some more Histograms for Analysis of Avg. Pulse Shape:
     h221[i] = new TH1D(Form("h221_%d",i),";A'_{max} (mV); events", 200, -20, 100);//Designated One histogram for each Ring (Distance from the Center of the crystal)
     h222[i] = new TH1D(Form("h222_%d",i),";A'_{max} (mV); events", 200, -20, 100);
     h223[i] = new TH1D(Form("h223_%d",i),";A'_{max} (mV); events", 200, -20, 100);
     h224[i] = new TH1D(Form("h224_%d",i),";A'_{max} (mV); events", 200, -20, 100);
     h225[i] = new TH1D(Form("h225_%d",i),";A'_{max} (mV); events", 200, -20, 100);//I set the lower Bin starting at 1 to avoid the extreme peak around zero
  }//Lazy TH2D construction below, should have looped (embarassing)
  TH1D* h105C = new TH1D("h105C","Amplitude of All Events;log10 amp (mV); events", 200, 0,5);
  TH2D* h201a = new TH2D("h201_a","Channel 3: Hits SiPM ; 1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, .02, 0.21, 1000, -5, 20); 
  TH2D* h201b = new TH2D("h201_b","Channel 3: Misses SiPM; 1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, 0.02, 0.21, 1000, -5, 20);
  TH2D* h201c = new TH2D("h201_c","Channel 3: Narrow-Peak; 1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, .02, 0.21, 1000, -5, 20);
  TH2D* h201d = new TH2D("h201_d","Channel 3: Wide-Peak; 1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, .02, 0.21, 1000, -5, 20);
  TH2D* h201e = new TH2D("h201_e","Channel 3: θ > 0.02; 1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, .02, 0.21, 1000, -5, 20);
  TH2D* h201f = new TH2D("h201_f","Channel 3: θ < 0.02; 1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, .02, 0.21, 1000, -5, 20);
  TH2D* h201g = new TH2D("h201_g","Channel 3: 0.01 < θ < 0.02; 1/ A'_{max} (/mV) ;T_{Thr} - T_{MCP} (ns); events", 100, 0, 0.21, 1000, -5, 20);
  TH1D* h202a = new TH1D("h202_0","Angle of Hits CH4; Angles (Degrees); events",300,-1.5,1.5);
  TH1D* h202b = new TH1D("h202_1","Angle of Hits CH5; Angles (Degrees); events",300,-1.5,1.5);
  TH1D* h202c = new TH1D("h202_2","Angle of Hits CH6; Angles (Degrees); events",300,-1.5,1.5);
  TH1D* h202d = new TH1D("h202_3","Angle of Hits CH3; Angles (Degrees); events",300,-1.5,1.5);
  //Going to use the two below to see the average waveforms of each regime in similar amplitudes Gnote: TProfile Construction
  TProfile* h126 = new TProfile("h126",";t_{i} (ns); average a_{ i} (mV)", 1000,- 70,100);
  TProfile* h127 = new TProfile("h127",";t_{i} (ns); average a_{ i} (mV)", 1000,- 70, 100); 
 //TH1D* h202e = new TH1D("h202","Some Angles",100,0,2);
  //TH1D* h202f = new TH1D("h202","Some Angles",100,0,2);
  //TH1D* h202g = new TH1D("h202","Some Angles",100,0,2);
TH2D *h201[7][nThr];
TH2D* h301[7][nThr];
TCanvas* suncanvas[7][nThr];
  for(int i=0; i<7; i++){
      for(int j=0; j<nThr; j++){
          h201[i][j] = new TH2D(Form("h201_%d_%d",i,j), "SiPM ; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP}  (ns); events", 100,.02, 0.21, 1000,-5,20);//fix my range
      	  h301[i][j] = new TH2D(Form("h301_%d_%d",i,j), "SiPM Peak Removed; 1 / A'_{max} (/mV) ; T_{Thr} - T_{MCP}  (ns); events", 100, .02, 0.21, 1000, -5, 20);//This is for removing the peaks/"Sun"
	}
  }
//Gnote: TGraph for Singlepulses
TCanvas* singpul = new TCanvas();
TGraph* singpulT = new TGraph();

  const int ngrWFMax = 10;
  TGraph *grWF[8][ngrWFMax];
  TGraph *grIF[7][ngrWFMax];
  int ngrWF = 0;
  
  Long64_t nentries = fChain->GetEntries();
  int nb, nbytes;
  int pul = 0; 
  for (Long64_t jentry=0; jentry<nentries;jentry++) {//Gnote: this is where the loop over all events begins
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (jentry % 10000 == 0) cout << "Processing Event " << jentry << " of " << nentries << "\n";
      nb = fChain->GetEntry(jentry);   nbytes += nb;
  
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
              h105[ich]->Fill( ampI[ich] );
	      h105C->Fill( log10(amp[ich]));//An extra 1D Histo of all events from all SiPMs
              if(ampI[ich] < 60){
                h106[ich]->Fill( xtrk, ytrk, ampI[ich] );
              }
          }
      }           
      

 if(isOK){
          for(int ich=0; ich<7; ich++){
              double tmp = sqrt( pow(xtrk - x0center[ich],2) + pow(ytrk - y0center[ich],2) );
              if(tmp < dR[0]){
                  h111[ich]->Fill( ampI[ich] );
                  if(ampI[ich] < 80){
                    for(int it=0; it<500; it++){
                        double x = (times[0][it]-tMCP_mean) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h121[ich]->Fill( x, z);
			h221[ich]->Fill(ampI[ich]);//Fill the CM Histograms 
                    }
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
              
              double tsThr[nThr];//Gnote: This is where tsThr is defined
              for(int ithr=0; ithr<nThr; ithr++){
                  tsThr[ithr] = -999;
                  if(intThr[ithr] > 0.95 * intA[ich]) continue;
                  for(unsigned int it=xv.size()-1; it>1; it--){
                      if( yv[it-1] <= intThr[ithr] && yv[it] > intThr[ithr] ){
                          tsThr[ithr] = xv[it-1] + (intThr[ithr] - yv[it-1])/(yv[it] - yv[it-1])*(xv[it] - xv[it-1]);
                          break;
                      }
                  }
              }

//CM fills single pulses (original single pulse study from November 2023)
if (ich == 0 ){
	//fill single pulse
for(int it=0; it<500; it++){
           double x = (times[0][it]-tMCP_mean) * 1e+9;
           double z = -1. * channel[0][it];
//	singpulT->SetPoint(it,x,z);
}
//	singpul->cd();
//	singpulT->Draw();
//singpulT->GetXaxis()->SetTitle("t_{i} - t_{MCP} (ns)");
//singpulT->GetYaxis()->SetTitle("average a_{ i} (mV)");
	//check if the event is above the line
//double cmamp = 1./ampI[0];
//double cmtime = tsThr[10];
//if (tmp < dR[0]){//Cloud conditions go here now (amplitude and deltaT)
double tmp = sqrt( pow(xtrk - x0center[0],2) + pow(ytrk - y0center[0],2) );
if (tmp < dR[0] && ampI[0] == 10 && tsThr[10] == 10){

//singpulT->SetTitle(Form("PWO_CH0__N2R1_Centerpulse_%d",jentry));
//singpul->Print(Form("trackstudies/PWO_CH0_N1R1_Centerpulse_%d.png",jentry));

}if (tmp < dR[2]&& ampI[0] > 10.5 && ampI[0] < 11 && tsThr[10] > -0.6 && tsThr[10] < -0.1){
//singpulT->SetTitle(Form("PWO_CH0__N2R6_Rightpulse_%d",jentry));
//singpul->Print(Form("trackstudies/PWO_CH0_N2R6_Rightpulse_%d.png",jentry));

}if (tmp > dR[2] && ampI[0] > 5 && ampI[0] < 5.3  && tsThr[10] > 3.2 && tsThr[10] < 3.7 ){
//singpulT->SetTitle(Form("PWO_CH0__N2R7_Rightpulse_%d",jentry));
//singpul->Print(Form("trackstudies/PWO_CH0_N2R7_Rightpulse_%d.png",jentry));

}if ( tmp < dR[2] && ampI[0] < 15 && ampI[0] < 10 && tsThr[10] < -0.6 && tsThr[10] > -1){
//singpulT->SetTitle(Form("PWO_CH0__N2R6_Centerpulse_%d",jentry));
//singpul->Print(Form("trackstudies/PWO_CH0_N2R6_Centerpulse_%d.png",jentry));

}if(tmp > dR[2] && ampI[0] < 7 && ampI[0] > 6.6 && tsThr[10] > 2.4 && tsThr[10] < 2.8){
//singpulT->SetTitle(Form("PWO_CH0__N2R7_Centerpulse_%d",jentry));
//singpul->Print(Form("trackstudies/PWO_CH0_N2R7_Centerpulse_%d.png",jentry));

}if (tmp < dR[2] && ampI[0] > 17 && ampI[0] < 17.3 && tsThr[10] > -1.3 && tsThr[10] < -1){
//singpulT->SetTitle(Form("PWO_CH0__N2R6_Leftpulse_%d",jentry));
//singpul->Print(Form("trackstudies/PWO_CH0_N2R6_Leftpulse_%d.png",jentry));

}if (tmp > dR[2] && ampI[0] < 9.5 && ampI[0] > 9 && tsThr[10] > 1.2 && tsThr[10] < 1.5){
//singpulT->SetTitle(Form("PWO_CH0__N2R7_Leftpulse_%d",jentry));
//singpul->Print(Form("trackstudies/PWO_CH0_N2R7_Leftpulse_%d.png",jentry));
}
//}//else{
//singpulT->SetTitle(Form("BGO_Below_pulse_%d",jentry));
//singpulT->GetXaxis()->SetTitle("t_{i} - t_{MCP} (ns)");
//singpulT->GetYaxis()->SetTitle("average a_{ i} (mV)");
//singpul->Print(Form("pulses/Below/BGO_Below_pulse_%d.png",jentry));



//}

}//Close Single Pulse stuff
//delete singpulT;
//Angular Study 
double delta = sqrt(pow(xIntercept-xtrk,2) +pow(yIntercept- ytrk,2) + pow(1560,2));//Total displacement of the MIP from beam origin to SiPM
double angle = acos(1560/delta);//Angle made between the xy plane and the track of the incoming MIP (likely in radians)
double an[] = {0,15,30,45,60,90};//some random angles to play with
double pi = TMath::Pi();//include the correct library
double conv = pi/180;
double plmi = 10*conv;//just a window of radians (10 degrees)
 if ( sqrt( pow(xtrk - x0center[4],2) + pow(ytrk - y0center[4],2)) < sqrt(18) ){
h202a->Fill(angle/conv);
}if ( sqrt( pow(xtrk - x0center[5],2) + pow(ytrk - y0center[5],2)) < sqrt(18) ){
h202b->Fill(angle/conv);
}if ( sqrt( pow(xtrk - x0center[6],2) + pow(ytrk - y0center[6],2)) < sqrt(18) ){
h202c->Fill(angle/conv);
}if ( sqrt( pow(xtrk - x0center[3],2) + pow(ytrk - y0center[3],2)) < sqrt(18) ){
h202d->Fill(angle/conv);
}


 if (ich == 3 ){
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
//if (tmp > sqrt(18)){//Narrow Region
if ( tmp < sqrt(18)){//Anything more than 89 degrees

        if(tsThr[10] > -5){
           h201a->Fill(1./ampI[3],tsThr[10] );
        }

}if (tmp > sqrt(18)  ){//90-0.65 and up

        if(tsThr[10] > -5){
           h201b->Fill(1./ampI[3],tsThr[10] );
                }


}if ( tmp < dR[2]){//exactly 90 degrees

	if(tsThr[10] > -5){
	h201c->Fill(1./ampI[3],tsThr[10] );
		}	 

}if(tmp > dR[2]){//less than 89
 if(tsThr[10] > -5){
        h201d->Fill(1./ampI[3],tsThr[10] );
                }


}if (angle > 0.02*conv){//narrow 60
 if(tsThr[10] > -5){
       h201e->Fill(1./ampI[3],tsThr[10] );
                }

}if (angle < 0.02*conv){
 if(tsThr[10] > -5){//narrow 45
        h201f->Fill(1./ampI[3],tsThr[10] );
                }

}if (angle < 0.02*conv && angle > 0.01*conv){//narrow 15
 if(tsThr[10] > -5){
        h201g->Fill(1./ampI[3],tsThr[10] );
                }

}
//}//for the hit/no hit condition 
}//else{continue;}//End of Angular study

              double tmp = sqrt( pow(xtrk - x0center[ich],2) + pow(ytrk - y0center[ich],2) ); 
              for(int ithr=0; ithr<nThr; ithr++){
                  if(tsThr[ithr] > -5 ){ //&& tmp > dR[2])CM bounds the radius on h201 to explore strange fit behavior 
                    h201[ich][ithr]->Fill( 1./ ampI[ich], tsThr[ithr] );//h201 get's filled on an event by event basis!
		}//if tsThr			
	}//for ithr

//Gnote: Above we fill h201 (the heatmap we always stare at) And below I fill the average WF and the single event pulses we are currently working on.
//Below is the amp and timing cut for Channel 0 Region 4 in use
//For Channel 4 Central peak:  if( ich == 4 && 1./ampI[4] > 0.0600  && 1./ampI[4] < 0.0625 && tsThr[10] > 0.25 && tsThr[10] < 0.50 && pul < 10 ){
				if( ich == 0 && 1./ampI[0] > 0.07  && 1./ampI[0] < 0.075 && tsThr[10] > -1.25 && tsThr[10] < -0.25 && pul < 10 ){//pul just ensures we sample 10 events
					for(int it=0; it <500; it++){
					double x = (times[0][it]-tMCP_mean)*1e+9;//be sure to change the time and channel info for the relevant channel
					double z = -1*channel[0][it];
					h126->Fill(x,z);
					singpulT->SetPoint(it,x,z);
				
				}
					singpulT->SetTitle(Form("Channel 0: Single Pulse_%d",jentry));
					singpulT->GetXaxis()->SetTitle("t_{i} - t_{MCP} (ns)");
                                        singpulT->GetYaxis()->SetTitle("average a_{ i} (mV)");
					singpul->cd();
					singpulT->Draw();
					singpul->Print(Form("Ch0SinglePulses/PWOparFilter_Ch0_%d.png",jentry));
					singpulT->Set(0);//Cleans the TGraph
					pul++;
				}else{ 
			        	continue;
			      	}	
  }    //chindex
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


//Here, I take the Bin contents of h201 and transfer them to h301 as long as the maximum is blocked out (sunplots)
for(int ich=0; ich<7 ; ich++){
 for(int ithr=0; ithr<nThr; ithr++){
	int brightspot =h201[ich][ithr]-> GetMaximumBin();
	double brightness =h201[ich][ithr]->GetBinContent(brightspot);
	double trim = brightness-brightness*0.50;

	int maxX = h201[ich][ithr]->GetNbinsX();
	int maxY = h201[ich][ithr]->GetNbinsY();

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

TH1D* proja = (TH1D*)h201a->ProjectionY("Projection",h201a->GetXaxis()->FindBin(1/18),h201a->GetXaxis()->FindBin(1/14));
  TCanvas* Angulara = new TCanvas();
  Angulara->Divide(2,1);
  Angulara->cd(1);
  h201a->Draw("COLZ");
  Angulara->cd(2);
  proja->Draw();
  //Angulara->Print("Angstudy/PWOChannel3HIT.png");

  TH1D* projb = (TH1D*)h201b->ProjectionY("Projection",h201b->GetXaxis()->FindBin(1/18),h201b->GetXaxis()->FindBin(1/14));
  TCanvas* Angularb = new TCanvas();
  Angularb->Divide(2,1);
  Angularb->cd(1);
  h201b->Draw("COLZ");
  Angularb->cd(2);
  projb->Draw();
 //Angularb->Print("Angstudy/PWOChannel3NOHIT.png");
 
TH1D* projc = (TH1D*)h201c->ProjectionY("Projection",h201c->GetXaxis()->FindBin(1/18),h201c->GetXaxis()->FindBin(1/14));
  TCanvas* Angularc = new TCanvas();
  Angularc->Divide(2,1);
  Angularc->cd(1);
  h201c->Draw("COLZ");
  Angularc->cd(2);
  projc->Draw();
  //Angularc->Print("Angstudy/PWOChannel3NARROW.png");
 
TH1D* projd = (TH1D*)h201d->ProjectionY("Projection",h201d->GetXaxis()->FindBin(1/18),h201d->GetXaxis()->FindBin(1/14));
  TCanvas* Angulard = new TCanvas();
  Angulard->Divide(2,1);
  Angulard->cd(1);
  h201d->Draw("COLZ");
  Angulard->cd(2);
  projd->Draw();
  //Angulard->Print("Angstudy/PWOChannel3WIDE.png");

  TH1D* proje = (TH1D*)h201e->ProjectionY("Projection",h201e->GetXaxis()->FindBin(1/18),h201e->GetXaxis()->FindBin(1/14));
  TCanvas* Angulare = new TCanvas();
  Angulare->Divide(2,1);
  Angulare->cd(1);
  h201e->Draw("COLZ");
  Angulare->cd(2);
  proje->Draw();
//Angulare->Print("Angstudy/PWOChannel3Angle_1.png");
 
TH1D* projf = (TH1D*)h201f->ProjectionY("Projection",h201f->GetXaxis()->FindBin(1/18),h201f->GetXaxis()->FindBin(1/14));
  TCanvas* Angularf = new TCanvas();
  Angularf->Divide(2,1);
  Angularf->cd(1);
  h201f->Draw("COLZ");
  Angularf->cd(2);
  projf->Draw();
// Angularf->Print("Angstudy/PWOChannel3Angle_2.png");

TH1D* projg = (TH1D*)h201g->ProjectionY("Projection",h201g->GetXaxis()->FindBin(1/18),h201g->GetXaxis()->FindBin(1/14));
  TCanvas* Angularg = new TCanvas();
  Angularg->Divide(2,1);
  Angularg->cd(1);
  h201g->Draw("COLZ");
  Angularg->cd(2);
  projg->Draw();
  //Angularg->Print("Angstudy/PWOChannel3Angle_3.png");

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
//  fout->Close();
h126->SetMarkerColor(kRed);
h126->SetLineColor(kRed);
h127->SetMarkerColor(kGreen);
h127->SetLineColor(kGreen);
TCanvas* avgcanv = new TCanvas("avgcanv","Avg WFs in Each Regime",800,600);
avgcanv->Clear();
avgcanv->cd();
h126->Draw();
h127->Draw("SAME");
TLegend* avglegend = new TLegend(0.7,0.7,0.9,0.9);
avglegend->AddEntry(h126, "Region 4", "lep");
avglegend->AddEntry(h127, "Region 5","lep");
avglegend->Draw();
avgcanv->Update();
//avgcanv->Print("AvgWF_Distributions_4_10_Region4and5.png");
avgcanv->Clear();
h126->Draw();
//avgcanv->Print("AvgWF_Distribution_0_10_Region4.png");//Gnote: output of the average waveforms we've been talking about
avgcanv->Clear();
h127->Draw();
//avgcanv->Print("AvgWF_Distribution_4_10_Region5.png");

fout->Close();
}



