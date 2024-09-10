#include <iostream>
#include <string>
#include <fstream>
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "pulse.hh"



int main( int argc, char** argv)
{
  int runMin = 0;
  int runMax = 0;
  std::cout << "Start of " << argv[0] << std::endl;
  if(argc==2){
    runMin = std::stoi(argv[1]);
    runMax = runMin;
  }else if(argc==3){
    runMin = std::stoi(argv[1]);
    runMax = std::stoi(argv[2]);
  }
  std::cout << "From run " << runMin << "  to run " << runMax << std::endl;
  if ( runMin == 0 || runMax == 0 )
  {
      std::cerr << "[ERROR]: Please provide range of runs\n";
      return -1;
  }
   
  TChain *myTree = new TChain("pulse");
  for(int run=runMin; run<=runMax; run++){
      myTree->Add(Form("/store/shared/maxd/run%d_info.root",run));
  }  
  if(myTree != 0){
    pulse* myPulse = new pulse( myTree );
    myPulse->Analysis();
  }

  return 0;
}
