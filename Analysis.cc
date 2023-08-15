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
  
  TFile *fout = new TFile("output.root","recreate");
  
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
      h105[i] = new TH1D(Form("h105_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h106[i] = new TProfile2D(Form("h106_%d",i), ";x_{ trk} (mm);y_{ trk} (mm); avg A'_{ max} (mV)", nbXY, xMin, xMax, nbXY, yMin, yMax);
      h111[i] = new TH1D(Form("h111_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h112[i] = new TH1D(Form("h112_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h113[i] = new TH1D(Form("h113_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h114[i] = new TH1D(Form("h114_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h115[i] = new TH1D(Form("h115_%d",i),";A'_{max} (mV); events", 200, -20, 100);
      h121[i] = new TProfile(Form("h121_%d",i), ";t_{i} - t_{ MCP} (ns); average a_{ i} (mV)", 1000, -10, 40);
      h122[i] = new TProfile(Form("h122_%d",i), ";t_{i} - t_{ MCP} (ns); average a_{ i} (mV)", 1000, -10, 40);
      h123[i] = new TProfile(Form("h123_%d",i), ";t_{i} - t_{ MCP} (ns); average a_{ i} (mV)", 1000, -10, 40);
      h124[i] = new TProfile(Form("h124_%d",i), ";t_{i} - t_{ MCP} (ns); average a_{ i} (mV)", 1000, -10, 40);
      h125[i] = new TProfile(Form("h125_%d",i), ";t_{i} - t_{ MCP} (ns); average a_{ i} (mV)", 1000, -10, 40);
  }
  
  TH2D *h201[7][nThr];
  for(int i=0; i<7; i++){
      for(int j=0; j<nThr; j++){
          h201[i][j] = new TH2D(Form("h201_%d_%d",i,j), "SiPM ; 1 / A'_{max} (/mV) ; T_{thr} - T_{MCP} (ns); events", 100, 0, 0.21, 1000, -5, 20);
      }
  }
  
  const int ngrWFMax = 10;
  TGraph *grWF[8][ngrWFMax];
  TGraph *grIF[7][ngrWFMax];
  int ngrWF = 0;
  
  Long64_t nentries = fChain->GetEntries();
  int nb, nbytes;
  
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (jentry % 100 == 0) cout << "Processing Event " << jentry << " of " << nentries << "\n";
      nb = fChain->GetEntry(jentry);   nbytes += nb;
  
      bool isOK = true; // is it a good event?

      
      // ---------------------------
      //           Tracks
      // ---------------------------
       
      
      if(isOK){
        h001->Fill( ntracks );
      }
      isOK = isOK && ntracks == 1;

      if(isOK){
          h001b->Fill(xIntercept);
      }
      isOK = isOK && !(fabs(xIntercept)<1e-9 && fabs(yIntercept)<1e-9 && fabs(xSlope)<1e-9 && fabs(ySlope) < 1e-9);

      // calculate position of the track at Z=1.5m      
      double xtrk = xIntercept + 1500. * xSlope;
      double ytrk = yIntercept + 1500. * ySlope;
      
      if(isOK){
        h002->Fill( xIntercept, yIntercept );
        h002b->Fill( xtrk, ytrk );
        h003->Fill( xSlope, ySlope );
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
        }
      }

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
                        double x = (times[0][it] - tMCP_mean) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h121[ich]->Fill( x, z);
                    }
                  }
              }else if(tmp < dR[1]){
                  h112[ich]->Fill( ampI[ich] );
                  if(ampI[ich] < 80){
                    for(int it=0; it<500; it++){
                        double x = (times[0][it] - tMCP_mean) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h122[ich]->Fill( x, z);
                    }
                  }
              }else if(tmp < dR[2]){
                  h113[ich]->Fill( ampI[ich] );
                  if(ampI[ich] < 80){
                    for(int it=0; it<500; it++){
                        double x = (times[0][it] - tMCP_mean) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h123[ich]->Fill( x, z);
                    }
                  }
              }else if(tmp < dR[3]){
                  h114[ich]->Fill( ampI[ich] );
                  if(ampI[ich] < 80){
                    for(int it=0; it<500; it++){
                        double x = (times[0][it] - tMCP_mean) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h124[ich]->Fill( x, z);
                    }
                  }
              }else{
                  h115[ich]->Fill( ampI[ich] );
                  if(ampI[ich] < 80){
                    for(int it=0; it<500; it++){
                        double x = (times[0][it] - tMCP_mean) * 1e+9;
                        double z = -1. * channel[ich][it];
                        h125[ich]->Fill( x, z);
                    }
                  }
              }
          }
      }

      // ----------------------------
      //    SiPM timing
      // ---------------------------
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
                  }
                  if(x>20) break;
              }
              
              double tsThr[nThr];
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
              
              for(int ithr=0; ithr<nThr; ithr++){
                  if(tsThr[ithr] > -5){
                    h201[ich][ithr]->Fill( 1./ ampI[ich], tsThr[ithr] );
                  }
              }
          }
      }
      
      
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
      
  }      

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
  fout->Close();
  
}




