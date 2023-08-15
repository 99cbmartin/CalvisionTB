//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 10 10:09:12 2023 by ROOT version 6.28/02
// from TTree pulse/Digitized waveforms
// found on file: data/run75094_info.root
//////////////////////////////////////////////////////////

#ifndef pulse_hh
#define pulse_hh

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <string>

class pulse {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          i_evt;
   Float_t         channel[8][502];
   Float_t         times[1][502];
   Float_t         baseline[8];
   Float_t         baseline_RMS[8];
   Float_t         noise[8];
   Float_t         amp[8];
   Float_t         t_peak[8];
   Float_t         integral[8];
   Float_t         intfull[8];
   Float_t         risetime[8];
   Float_t         decaytime[8];
   Float_t         LP2_5[8];
   Float_t         LP2_10[8];
   Float_t         LP2_15[8];
   Float_t         LP2_20[8];
   Float_t         LP2_25[8];
   Float_t         LP2_30[8];
   Float_t         LP2_35[8];
   Float_t         LP2_40[8];
   Float_t         LP2_50[8];
   Float_t         LP2_60[8];
   Float_t         LP2_70[8];
   Float_t         LP2_80[8];
   Float_t         LP2_30mV[8];
   Float_t         gaus_mean[8];
   Float_t         gaus_sigma[8];
   Float_t         gaus_chi2[8];
   Float_t         timeoffsets[8];
   Float_t         xIntercept;
   Float_t         yIntercept;
   Float_t         xSlope;
   Float_t         ySlope;
   Float_t         x_dut[12];
   Float_t         y_dut[12];
   Float_t         chi2;
   Float_t         xResidBack;
   Float_t         yResidBack;
   Float_t         xErrDUT;
   Float_t         yErrDUT;
   Float_t         xErr05;
   Float_t         yErr04;
   Float_t         xResid05;
   Float_t         yResid04;
   Int_t           ntracks;
   Int_t           ntracks_alt;
   Int_t           nplanes;
   Int_t           npix;
   Int_t           nback;
   Int_t           nClustersPix;
   Int_t           nClustersStripsX;
   Int_t           nClustersStripsY;
   Int_t           nStripsWith2Clusters;
   Int_t           run;
   Int_t           gconf;
   std::vector<std::string>  *sensors;
   std::vector<int>     *pads;
   std::vector<std::string>  *sensorsHV;
   std::vector<int>     *HVs;

   // List of branches
   TBranch        *b_i_evt;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_time;   //!
   TBranch        *b_baseline;   //!
   TBranch        *b_baseline_RMS;   //!
   TBranch        *b_noise;   //!
   TBranch        *b_amp;   //!
   TBranch        *b_t_peak;   //!
   TBranch        *b_integral;   //!
   TBranch        *b_intfull;   //!
   TBranch        *b_risetime;   //!
   TBranch        *b_decaytime;   //!
   TBranch        *b_LP2_5;   //!
   TBranch        *b_LP2_10;   //!
   TBranch        *b_LP2_15;   //!
   TBranch        *b_LP2_20;   //!
   TBranch        *b_LP2_25;   //!
   TBranch        *b_LP2_30;   //!
   TBranch        *b_LP2_35;   //!
   TBranch        *b_LP2_40;   //!
   TBranch        *b_LP2_50;   //!
   TBranch        *b_LP2_60;   //!
   TBranch        *b_LP2_70;   //!
   TBranch        *b_LP2_80;   //!
   TBranch        *b_LP2_30mV;   //!
   TBranch        *b_gaus_mean;   //!
   TBranch        *b_gaus_sigma;   //!
   TBranch        *b_gaus_chi2;   //!
   TBranch        *b_timeoffsets;   //!
   TBranch        *b_xIntercept;   //!
   TBranch        *b_yIntercept;   //!
   TBranch        *b_xSlope;   //!
   TBranch        *b_ySlope;   //!
   TBranch        *b_x_dut;   //!
   TBranch        *b_y_dut;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_xResidBack;   //!
   TBranch        *b_yResidBack;   //!
   TBranch        *b_xErrDUT;   //!
   TBranch        *b_yErrDUT;   //!
   TBranch        *b_xErr05;   //!
   TBranch        *b_yErr04;   //!
   TBranch        *b_xResid05;   //!
   TBranch        *b_yResid04;   //!
   TBranch        *b_ntracks;   //!
   TBranch        *b_ntracks_alt;   //!
   TBranch        *b_nplanes;   //!
   TBranch        *b_npix;   //!
   TBranch        *b_nback;   //!
   TBranch        *b_nClustersPix;   //!
   TBranch        *b_nClustersStripsX;   //!
   TBranch        *b_nClustersStripsY;   //!
   TBranch        *b_nStripsWith2Clusters;   //!
   TBranch        *b_run;   //!
   TBranch        *b_gconf;   //!
   TBranch        *b_sensors;   //!
   TBranch        *b_pads;   //!
   TBranch        *b_sensorsHV;   //!
   TBranch        *b_HVs;   //!

   pulse(TTree *tree=0);
   virtual ~pulse();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void Analysis();
};

#endif

#ifdef pulse_cxx
pulse::pulse(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data/run75094_info.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data/run75094_info.root");
      }
      f->GetObject("pulse",tree);

   }
   Init(tree);
}

pulse::~pulse()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pulse::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pulse::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void pulse::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   sensors = 0;
   pads = 0;
   sensorsHV = 0;
   HVs = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("i_evt", &i_evt, &b_i_evt);
   fChain->SetBranchAddress("channel", channel, &b_channel);
   fChain->SetBranchAddress("time", times, &b_time);
   fChain->SetBranchAddress("baseline", baseline, &b_baseline);
   fChain->SetBranchAddress("baseline_RMS", baseline_RMS, &b_baseline_RMS);
   fChain->SetBranchAddress("noise", noise, &b_noise);
   fChain->SetBranchAddress("amp", amp, &b_amp);
   fChain->SetBranchAddress("t_peak", t_peak, &b_t_peak);
   fChain->SetBranchAddress("integral", integral, &b_integral);
   fChain->SetBranchAddress("intfull", intfull, &b_intfull);
   fChain->SetBranchAddress("risetime", risetime, &b_risetime);
   fChain->SetBranchAddress("decaytime", decaytime, &b_decaytime);
   fChain->SetBranchAddress("LP2_5", LP2_5, &b_LP2_5);
   fChain->SetBranchAddress("LP2_10", LP2_10, &b_LP2_10);
   fChain->SetBranchAddress("LP2_15", LP2_15, &b_LP2_15);
   fChain->SetBranchAddress("LP2_20", LP2_20, &b_LP2_20);
   fChain->SetBranchAddress("LP2_25", LP2_25, &b_LP2_25);
   fChain->SetBranchAddress("LP2_30", LP2_30, &b_LP2_30);
   fChain->SetBranchAddress("LP2_35", LP2_35, &b_LP2_35);
   fChain->SetBranchAddress("LP2_40", LP2_40, &b_LP2_40);
   fChain->SetBranchAddress("LP2_50", LP2_50, &b_LP2_50);
   fChain->SetBranchAddress("LP2_60", LP2_60, &b_LP2_60);
   fChain->SetBranchAddress("LP2_70", LP2_70, &b_LP2_70);
   fChain->SetBranchAddress("LP2_80", LP2_80, &b_LP2_80);
   fChain->SetBranchAddress("LP2_30mV", LP2_30mV, &b_LP2_30mV);
   fChain->SetBranchAddress("gaus_mean", gaus_mean, &b_gaus_mean);
   fChain->SetBranchAddress("gaus_sigma", gaus_sigma, &b_gaus_sigma);
   fChain->SetBranchAddress("gaus_chi2", gaus_chi2, &b_gaus_chi2);
   fChain->SetBranchAddress("timeoffsets", timeoffsets, &b_timeoffsets);
   fChain->SetBranchAddress("xIntercept", &xIntercept, &b_xIntercept);
   fChain->SetBranchAddress("yIntercept", &yIntercept, &b_yIntercept);
   fChain->SetBranchAddress("xSlope", &xSlope, &b_xSlope);
   fChain->SetBranchAddress("ySlope", &ySlope, &b_ySlope);
   fChain->SetBranchAddress("x_dut", x_dut, &b_x_dut);
   fChain->SetBranchAddress("y_dut", y_dut, &b_y_dut);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("xResidBack", &xResidBack, &b_xResidBack);
   fChain->SetBranchAddress("yResidBack", &yResidBack, &b_yResidBack);
   fChain->SetBranchAddress("xErrDUT", &xErrDUT, &b_xErrDUT);
   fChain->SetBranchAddress("yErrDUT", &yErrDUT, &b_yErrDUT);
   fChain->SetBranchAddress("xErr05", &xErr05, &b_xErr05);
   fChain->SetBranchAddress("yErr04", &yErr04, &b_yErr04);
   fChain->SetBranchAddress("xResid05", &xResid05, &b_xResid05);
   fChain->SetBranchAddress("yResid04", &yResid04, &b_yResid04);
   fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain->SetBranchAddress("ntracks_alt", &ntracks_alt, &b_ntracks_alt);
   fChain->SetBranchAddress("nplanes", &nplanes, &b_nplanes);
   fChain->SetBranchAddress("npix", &npix, &b_npix);
   fChain->SetBranchAddress("nback", &nback, &b_nback);
   fChain->SetBranchAddress("nClustersPix", &nClustersPix, &b_nClustersPix);
   fChain->SetBranchAddress("nClustersStripsX", &nClustersStripsX, &b_nClustersStripsX);
   fChain->SetBranchAddress("nClustersStripsY", &nClustersStripsY, &b_nClustersStripsY);
   fChain->SetBranchAddress("nStripsWith2Clusters", &nStripsWith2Clusters, &b_nStripsWith2Clusters);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("gconf", &gconf, &b_gconf);
   fChain->SetBranchAddress("sensors", &sensors, &b_sensors);
   fChain->SetBranchAddress("pads", &pads, &b_pads);
   fChain->SetBranchAddress("sensorsHV", &sensorsHV, &b_sensorsHV);
   fChain->SetBranchAddress("HVs", &HVs, &b_HVs);
   Notify();
}

Bool_t pulse::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pulse::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pulse::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef pulse_cxx
