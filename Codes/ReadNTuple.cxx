
#include"ReadNTuple.h"

ReadNTuple::ReadNTuple(){
}
ReadNTuple::~ReadNTuple(){
  std::cout << "destructor ReadNTuple" << '\n';
}

void ReadNTuple::readOutNTuple(TNtuple *nTuple){
  Float_t energyPeak, lowEnergy, highEnergy,peakWidth;
  nTuple->SetBranchAddress("energyPeak",&energyPeak);
  nTuple->SetBranchAddress("lowEnergy",&lowEnergy);
  nTuple->SetBranchAddress("highEnergy",&highEnergy);
  nTuple->SetBranchAddress("peakWidth",&peakWidth);
  Int_t nentries = (Int_t)nTuple->GetEntries();
  for (Int_t i = 0; i < nentries; i++) {
    nTuple->GetEntry(i);
    rPeaksEnergies.push_back(energyPeak);
    rPeakWidths.push_back(peakWidth);

    if(Debug){
    cout<<"TNtuple"<<endl;
    cout<<"Peak Energy:"<<energyPeak<<endl;
    cout<<"Peak Width:"<<peakWidth<<endl;
    //cout<<"Old Low energy"<<rEnergyLows.back()<<endl;
    cout<<"low energy"<<lowEnergy<<endl;
    }
    if(i!=0&&rEnergyHighs.back()>lowEnergy){
        rEnergyLows.push_back(rEnergyLows.back());
        if(Debug){
        cout<<"New Low energy"<<rEnergyLows.back()<<endl;
      }

        if (rEnergyHighs.back()>highEnergy){
          rEnergyHighs.push_back(rEnergyHighs.back());
        }
        else{
          rEnergyHighs.push_back(highEnergy);
          for(int i=rEnergyLows.size()-1;rEnergyLows[i]==rEnergyLows[i-1]; i--){
            rEnergyHighs[i-1]=highEnergy;
        }
        }
      }
      else{
        rEnergyLows.push_back(lowEnergy);
        rEnergyHighs.push_back(highEnergy);
      }
    }
}
