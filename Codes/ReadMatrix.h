#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif
class ReadMatrix{
public:
   ReadMatrix();
   ~ReadMatrix();
   void createHistogram(TH2F *matrix, TString axis, TString gate1Text,TString gate2Text,TString background1Text,TString background2Text,TString background3Text,TString background4Text);
   TH1F *histogramTotal;
   TH1F *backgroundHistogram1;
   TH1F *backgroundHistogram2;
   TH1F *histogram;
};