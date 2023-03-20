#ifndef __Settings_h__
#define __Settings_h__

#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif

class Settings {

public:
  Settings();//default constructor

  ~Settings();

  void setSettings(TString filename);
  void readSettings();
  TString readSettingsFile(TString line);
  std::vector<TString> readSettingsFileVector(TString line);
  std::vector<TString> readHistogramsFile(TString line);
  void printSettings();
  Bool_t checkIntpuFile(TString PathAndName);

  void setFile(TString filename){
    fSettingsFileName = filename;
  }
  const TString settingsFile(){
    return fSettingsFileName;
  };

  TString outputFileName(){
    return fOutputFileName;
  };
  
  std::vector<TString> inputFileName(){
    return fInputFileName;
  };
  std::vector<TString> matrixGateFileName(){
    return fMatrixGateFileName;
  };
  std::vector<TString> matrixHistogramName(){
    return fMatrixHistogramFileName;
  };
  TString inputRootFileName(){
    return fInputRootFileName;
  };
   TString outputRootFileName(){
    return fOutputRootFileName;
  };
  std::vector<TString> histogramFileName(){
    return fHistogramFileName;
  };
  std::vector<TString> matrixPeakFileName(){
    return fMatrixPeakFileName;
  };

  Bool_t debugOpt(){
    return fDebugOpt;
  }


protected:
  TString fSettingsFileName;
  TString fOutputFileName;
  TString fOutputRootFileName;
  TString fInputRootFileName;
  std::vector<TString> fMatrixGateFileName;
  std::vector<TString> fMatrixHistogramFileName;
  std::vector<TString>  fMatrixPeakFileName;
  std::vector<TString> fInputFileName;
  std::vector<TString> fHistogramFileName;

  Bool_t fDebugOpt;

  //EuBa Source Data Info


};

#endif
