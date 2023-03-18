#ifndef _ReadFile_h_
#include "ReadFile.h"
#endif
ReadFile::ReadFile(){}
ReadFile::~ReadFile(){
  std::cout << "destructorReadFile" << '\n';
}
void ReadFile::readFile(TString inputPath){

  ifstream in;
  Float_t peakEnergy, lowEnergy, highEnergy,peakWidth;
  TNtuple* ntuple=new TNtuple("Peak information","data from ascii files","energyPeak:lowEnergy:highEnergy:peakWidth");;
  if(Debug){
    cout<<endl<<"ReadFile"<<endl;
    std::cout << "InputPath:" <<inputPath.Data()<< '\n';
  }
  TString inputPathRelative="../Input/";
  inputPathRelative.Append(inputPath);
	in.open (inputPathRelative.Data());
        while(1) // To get you all the lines.
        {
          in>> peakEnergy>> lowEnergy>> highEnergy>>peakWidth;
          if (!in.good()) break;
          if(Debug){
            std::cout <<"peakEnergy:"<< peakEnergy << '\n';
            std::cout << "lowEnergy:" <<lowEnergy<< '\n';
            std::cout << "highEnergy:" <<highEnergy<< '\n';
            std::cout << "peakWidth:" <<peakWidth<< '\n';
          }
	        ntuple->Fill(peakEnergy,lowEnergy,highEnergy,peakWidth);
         // Prints our STRING.
        }
	peakData=(TNtuple*)ntuple;
	in.close();
}

void ReadFile::readMatrixGatesFile(TString inputPath){
  ifstream in;
  TString inputPathRelative="../Input/";
  inputPathRelative.Append(inputPath);
  in.open (inputPathRelative.Data());
  TString gateGateOnAxis="GateOnAxis:";
  TString gate1Text="GateLow:";
  TString gate2Text="GateHigh:";
  TString background1Text="BackgroundLow_1:";
  TString background2Text="BackgroundHigh_1:";
  TString background3Text="BackgroundLow_2:";
  TString background4Text="BackgroundHigh_2:";
  Int_t checkThatGatesMatches=0;
  TString text,returnValue;
  std::cout << "InputPath:" <<inputPath.Data()<< '\n';
  while(!in.eof()) // To get you all the lines.
  {
    std::cout << "New Line" << '\n';
    in>>text>>returnValue;
    if(checkThatGatesMatches==0&&strcmp(text.Data(),gateGateOnAxis.Data())==0){
      gateOnAxis.push_back(returnValue);
      std::cout << "Gate On Axis:" <<returnValue.Data()<< '\n';
      checkThatGatesMatches=1;
      continue;
    }
    if(checkThatGatesMatches==1&&strcmp(text.Data(),gate1Text.Data())==0){
      gate1Vector.push_back(returnValue);
      std::cout << "Gate 1:" <<returnValue.Data()<< '\n';
      checkThatGatesMatches=2;
      continue;
    }
    if(checkThatGatesMatches==2&&strcmp(text.Data(),gate2Text.Data())==0){
      gate2Vector.push_back(returnValue);
      std::cout << "Gate 2:" <<returnValue.Data()<< '\n';
      checkThatGatesMatches=3;
      continue;
    }
    if(checkThatGatesMatches==3&&strcmp(text.Data(),background1Text.Data())==0){
      backgroundGate1Vector.push_back(returnValue);
      std::cout << "BgGate 1:" <<returnValue.Data()<< '\n';
      checkThatGatesMatches=4;
      continue;
    }
    if(checkThatGatesMatches==3&&strcmp(text.Data(),background1Text.Data())!=0){
      backgroundGate1Vector.push_back("-1");
      checkThatGatesMatches=4;
      continue;
    }
    if(checkThatGatesMatches==4&&strcmp(text.Data(),background2Text.Data())==0){
      backgroundGate2Vector.push_back(returnValue);
      std::cout << "BgGate 2:" <<returnValue.Data()<< '\n';
      checkThatGatesMatches=5;
      continue;
    }

    if(checkThatGatesMatches==4&&strcmp(text.Data(),background2Text.Data())!=0){
      backgroundGate2Vector.push_back("-1");
      checkThatGatesMatches=5;
      continue;
    }

    if(checkThatGatesMatches==5&&strcmp(text.Data(),background3Text.Data())==0){
      backgroundGate3Vector.push_back(returnValue);
      std::cout << "BgGate 3:" <<returnValue.Data()<< '\n';
      checkThatGatesMatches=6;
      continue;
    }
    if(checkThatGatesMatches==5&&strcmp(text.Data(),background3Text.Data())!=0){
      backgroundGate3Vector.push_back("-1");
      checkThatGatesMatches=6;
      continue;
    }
    if(checkThatGatesMatches==6&&strcmp(text.Data(),background4Text.Data())==0){
      backgroundGate4Vector.push_back(returnValue);
      std::cout << "BgGate 4:" <<returnValue.Data()<< '\n';
      checkThatGatesMatches=0;
      continue;
    }
    if(checkThatGatesMatches==6&&strcmp(text.Data(),background4Text.Data())!=0){
      backgroundGate4Vector.push_back("-1");
      checkThatGatesMatches=0;
      continue;
    }
    std::cout << "BgGate 4:" <<returnValue.Data()<< '\n';
    if (checkThatGatesMatches!=0){
      std::cout << "There is something wrong with 2D-MatrixGates" << '\n';
      
    }
  }
 in.close();
}


void ReadFile::readRootFile(TString rootFileName){
      TString rootPathRelative="../Input/";
      rootPathRelative.Append(rootFileName);
      rootFile = TFile::Open(rootPathRelative.Data());
      if(Debug){
        std::cout << "RootFile:"<< rootFile->GetName() << '\n';
      }
}
void ReadFile::readHistogram(TString histogramFileName){
  histogram=(TH1F*)rootFile->Get(histogramFileName.Data());
  if(Debug){
    std::cout << "Histogram:"<< histogram->GetTitle() << '\n';
  }
}

  void ReadFile::readMatrix(TString matrixFileName){
    matrix=(TH2F*)rootFile->Get(matrixFileName.Data());
    if(Debug){
      std::cout << "Histogram:"<< matrix->GetTitle() << '\n';
    }
}
