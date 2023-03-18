#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif

class FitGaussians{
public:
  FitGaussians();
  ~FitGaussians();
  void mainFitter(TH1F* histogram,std::vector<Float_t> energyPeaks, std::vector<Float_t> energyLows,std::vector<Float_t> energyHighs,std::vector<Float_t> peakWidth,TString output);
  Double_t static gaussiansPlusBackground(Double_t *x, Double_t *par);
  Double_t static gaussFunction(Double_t *x,Double_t *par);
  Double_t static backgroundFunction(Double_t *x,Double_t *par);
  Double_t binning(TH1F *h1);
  void estimateBackgroundParameters(TH1F *h1,Float_t low,Float_t high);
  TString nameFunction(TString output, TString functionType, Int_t numberOfPeaks, Float_t low, Float_t high);
  void initialisationParameters(TF1 *function,TH1F *h1,std::vector<Float_t> peaksToFitted, Int_t numberOfPeaks,std::vector<Float_t> peaksToFittedWidth);
  void setParametersForGaussians(Int_t numberOfPeaks,TF1 **fGaussian,TF1 *fGaussianPlusBG);
  void setParametersForBackground(TF1 *fBackground,TF1 *fGaussianPlusBG);
  void fittingProcedure(TH1F *h1,std::vector<Float_t> peaksToFitted,Float_t low, Float_t high,Int_t numberOfPeaks,TString output,std::vector<Float_t>  peaksToFittedWidth);
  std::vector<TF1 *> fGaussians;
//  TF1 *fBackground;
//  TF1 *fGaussianPlusBG;
//  TF1 **fGaussian;
  std::vector<TF1 *> fBackgrounds;
  std::vector<TF1 *> fGaussianPlusBGs;
  std::vector<std::vector<TF1 *> > fGaussiansMatrix;
  std::vector<TString> fGaussiansPlusBackgroundNames;
  TFitResultPtr fFitParameters;
  std::vector<TFitResultPtr> fFitParametersVector;
private:
  Double_t fPolynomeLinearCoefficient;
  Double_t fPolynomeConstantCoefficient;
  TString number;

};
