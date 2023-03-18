#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif

class DrawFigure{
public:
  DrawFigure();
  ~DrawFigure();
  void makeToFigure(TH1F *h1,TString output,std::vector<TString>functionNames,std::vector<Float_t> lows, std::vector<Float_t> highs,std::vector<TF1 *> Backgrounds,std::vector<std::vector<TF1 *>> gaussians,std::vector<TF1 *> gaussianPlusBGs,std::vector<TFitResultPtr> fitParameters);
  void figureMain(TH1F *h1,TString output,TString functionName,Float_t low, Float_t high, Int_t numberOfPeaks,TF1 *Background,std::vector<TF1 *> gaussian,TF1 *gaussianPlusBG,TFitResultPtr fitParameters);
};
