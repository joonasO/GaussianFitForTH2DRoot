
#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif
#ifndef __Settings__
#include "Settings.h"
#endif
class ReadFile{
public:
  ReadFile();
  ~ReadFile();
  void readFile(TString inputName);
  void readRootFile(TString rootFileName);
  void readHistogram(TString histogramFileName);
  void readMatrix(TString matrixFileName);
  void readMatrixGatesFile(TString inputPath);
  void readMatrixEnergiesFile(TString inputPath,TString gate1, TString gate2,TString Background1,TString Background2,TString Background3,TString Background4 );
  TNtuple* peakData;
  std::vector<TNtuple*> peakDataMatrix;
  TFile* rootFile;
  TH1F* histogram;
  TH2F* matrix;
  std::vector<TString> gateOnAxis;
  std::vector<TString> gate1Vector;
  std::vector<TString> gate2Vector;
  std::vector<TString> backgroundGate1Vector;
  std::vector<TString> backgroundGate2Vector;
  std::vector<TString> backgroundGate3Vector;
  std::vector<TString> backgroundGate4Vector;



};
