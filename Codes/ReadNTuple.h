#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif
class ReadNTuple{
public:
   ReadNTuple();
   ~ReadNTuple();
   void readOutNTuple(TNtuple *nTuple);
   std::vector<Float_t> peakEnergies(){
     return rPeaksEnergies;
   }
   std::vector<Float_t> peakWidths(){
     return rPeakWidths;
   }
   std::vector<Float_t> energiesLow(){
     return rEnergyLows;
   }
   std::vector<Float_t> energiesHigh(){
     return rEnergyHighs;
   }
protected:
  std::vector<Float_t> rPeaksEnergies;
  std::vector<Float_t> rPeakWidths;
  std::vector<Float_t> rEnergyLows;
  std::vector<Float_t> rEnergyHighs;
};
