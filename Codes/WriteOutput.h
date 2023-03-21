#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif

class WriteOutput{
public:
  WriteOutput();
  ~WriteOutput();
  void initialiseOutput(TString output);
  Double_t binning(TH1F *h1);
  void readFunction(TH1F* h1, std::vector<Float_t> lows,std::vector<Float_t> highs,std::vector<std::vector<TF1 *>> gaussianFunctions , std::vector<TF1 *> gaussianPlusBackground,std::vector<TF1 *> background,  std::vector<TFitResultPtr> fitParameters,TString output,Bool_t Debug);
  void calculateArea(TH1F* h1 ,TFitResultPtr fitParameters, Float_t low,Float_t	high, TF1* gaussianPlusBG,TF1* background,std::vector<TF1 *> gaussian,TString output ,Bool_t Debug);
  void doOutput(TString title,Double_t peakCalculatedIntegral,Double_t peakCalculatedIntegralError,Double_t peakNumericalIntegral,Double_t peakNumericalIntegralError,Double_t totalPeakArea,Double_t totalPeakAreaError,Double_t peakPlusBackgroundIntegral,Double_t peakPlusBackgroundIntegralError,Double_t histogramIntegral,Double_t backgroundIntegral,Double_t backgroundIntegralError,Double_t constant,Double_t constant_error,Double_t mean,Double_t mean_error,Double_t sigma,Double_t sigma_error,Double_t fwhm,Double_t fwhm_error,Double_t polynom_constant,Double_t polynom_linear,TString output,TFitResultPtr fitParameters,Bool_t Debug);
  void doRootFile(TString title,TString gate1,TString gate2,TString backgroundGate1, TString backgroundGate2,TString backgroundGate3, TString backgroundGate4, TH1F* histogram,TH1F* histogramTotal,TH1F* histogramBackground1,TH1F* histogramBackground2 );
  void writeRootOutputFile(TFile *file, TH1F *Raw, TH1F *Background1, TH1F *Background2,TH1F *substractedSpectrum, TH1F *BackgroundSubstactedTotal);
private:
  TString wOutputPath;
  TString wRootOutputPath;
};
